#include <algorithm>
#include <vector>
#include <random>
#include <ranges> 
#include <iostream>

#include "Agent_GRPO.h"
#include "AgentActions.h"
#include "IAgent.h"
#include "IGrid.h"
#include "Position.h"

Agent_GRPO :: Agent_GRPO(IGrid *grid) : IAgent(grid),
    _policy(std::vector< std::vector< std::vector<double>>>(
        _grid->getGridSize(),
        std::vector<std::vector<double>>(_grid->getGridSize(),
        std::vector<double>(AgentActions::NUM_ACTIONS, 0) ))),
        _randomGenerator(_randomDevice()),
    _selectActionRandom(std::uniform_real_distribution<double>(0, 1.0))
{
    InitializePolicy();
}

AgentActions Agent_GRPO::nextMove(bool trainning)
{
    AgentActions result = AgentActions::Move_down;
    if (trainning) {
        
        std::vector<int> candidateActions(0); // Grupo de acciones candidatas
        std::vector<double> rewards(0); // Recompensas obtenidas por cada acción
        int numCandidateActions = ((int)AgentActions::NUM_ACTIONS * PERCENTAGE_ACTIONS_TO_TRY + 50) / 100;

        // Selección de <numCandidateActions> acciones y evaluación de recompensas
        AgentActions newAction;
        for (int i = 0; i < numCandidateActions; i++) {
            int counter = 0;
            do {
                newAction = SelectAction();
                counter++;
                if (counter < numCandidateActions * 2) {
                    newAction = (AgentActions)(random() % AgentActions::NUM_ACTIONS);
                    break;
                }
            } while (std::ranges::find(candidateActions, newAction) != candidateActions.end()); // se asegura de no duplicar acciones
            candidateActions.emplace_back(newAction);
            rewards.emplace_back(_grid->rewardIfDo((AgentActions) newAction));
        }

        // Cálculo de ventaja relativa dentro del grupo de acciones

        double meanReward = 0;
        for (int i = 0; i < numCandidateActions; i++) {
            meanReward += rewards[i];
        }
        meanReward /= numCandidateActions;

        std::vector<double> advantages(numCandidateActions);
        for (int i = 0; i < numCandidateActions; i++) {
            advantages[i] = rewards[i] - meanReward;
        }

        // Actualización de política usando GRPO
        for (int i = 0; i < numCandidateActions; i++)
        {
            int action = candidateActions[i];
            double oldProb = _policy[_grid->getAgentPosition().x][_grid->getAgentPosition().y][action];

            // Normalizar las probabilidades para que sumen 1
            double totalProb = 0.0;
            for (int j = 0; j < numCandidateActions; j++)
            {
                totalProb += _policy[_grid->getAgentPosition().x][_grid->getAgentPosition().y][j];
            }
            for (int j = 0; j < numCandidateActions; j++)
            {
                _policy[_grid->getAgentPosition().x][_grid->getAgentPosition().y][j] /= totalProb;
            }

            // Calcular el ratio con la nueva probabilidad
            double newProb = _policy[_grid->getAgentPosition().x][_grid->getAgentPosition().y][action];

            double ratio = newProb / oldProb; // Compara la nueva probabilidad con la anterior

            double clippedRatio = std::clamp(ratio, 1 - CLIP_EPSILON, 1 + CLIP_EPSILON);

            double gradient = std::min(ratio * advantages[i], clippedRatio * advantages[i]);

            _policy[_grid->getAgentPosition().x][_grid->getAgentPosition().y][action] += LEARNING_RATE * gradient;
            _policy[_grid->getAgentPosition().x][_grid->getAgentPosition().y][action] = std::clamp(_policy[_grid->getAgentPosition().x][_grid->getAgentPosition().y][action], 0.01, 0.99);
        }
    }

    result = GetBestAction();
    _grid->moveAgent(result);

    return result;
}

void Agent_GRPO::start()
{
}

void Agent_GRPO::stop()
{
    
}

void Agent_GRPO :: generateDebugData(int trainningEpisode)
{
    throw std::logic_error("Not implemented");
}


void Agent_GRPO::InitializePolicy()
{
    for (int x = 0; x < _grid->getGridSize(); x++) {
        for (int y = 0; y < _grid->getGridSize(); y++) {
            for (int a = 0; a < AgentActions::NUM_ACTIONS; a++) {
                _policy[x][y][a] = 1.0 / AgentActions::NUM_ACTIONS; // Inicializa con probabilidades iguales
            }
        }
    }
}

AgentActions Agent_GRPO::SelectAction()
{
    double rand = _selectActionRandom(_randomGenerator);
    double cumulativeProbability = 0.0;

    for (int action = 0; action < AgentActions::NUM_ACTIONS; action++)
    {
        
        cumulativeProbability += _policy[_grid->getAgentPosition().x][_grid->getAgentPosition().y][action];
        if (rand < cumulativeProbability)
            return (AgentActions)action;
    }
    
    return (AgentActions)(random() % AgentActions::NUM_ACTIONS);

}

AgentActions Agent_GRPO::GetBestAction()
{
    int bestAction = -1;
    double bestValue = std::numeric_limits<double>::min();

    for (int a = 0; a < AgentActions::NUM_ACTIONS; a++)
    {
        if (_policy[_grid->getAgentPosition().x][_grid->getAgentPosition().y][a] > bestValue)
        {
            bestValue = _policy[_grid->getAgentPosition().x][_grid->getAgentPosition().y][a];
            bestAction = a;
        }
    }
    if (bestAction == -1) {
        bestAction = (AgentActions)(random() % AgentActions::NUM_ACTIONS);
    }
    return (AgentActions)bestAction;
}
