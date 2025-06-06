
#include <vector>

#include "Agent_QLearning.h"
#include "AgentActions.h"
#include "IAgent.h"
#include "IGrid.h"
#include "Position.h"
#include <random>

Agent_Qlearning ::Agent_Qlearning(IGrid *grid) :
    IAgent(grid),
    _qTable(std::vector<std::vector<double>>(_grid->getGridSize() * _grid->getGridSize(), std::vector<double>(AgentActions::NUM_ACTIONS, 0))),
    _exploreRandom(std::uniform_real_distribution<double>(0, 1.0)),
    _actionRandom(std::uniform_int_distribution<int>(0,3))
{
    _randomGenerator.seed(_randomDevice());
}

AgentActions Agent_Qlearning :: nextMove(bool trainning)
{
    if (trainning) {
        AgentActions action = SelectAction();
        Position oldPosition = _grid->getAgentPosition();
        Rewards reward = _grid->moveAgent(action);

        AgentActions bestNextAction = GetBestAction();

        int oldPositionIndex = oldPosition.x + _grid->getGridSize() * oldPosition.y;
        int newPositionIndex = _grid->getAgentPosition().x + _grid->getGridSize() * _grid->getAgentPosition().y;

        _qTable[oldPositionIndex][action] = _qTable[oldPositionIndex][action] + LEARNING_RATE * 
            (reward + DISCOUNT_FACTOR * _qTable[newPositionIndex][bestNextAction] - _qTable[oldPositionIndex][action]);
        return action;
    } 
    AgentActions action = GetBestAction();
    _grid->moveAgent(action);

    return action;
}

void Agent_Qlearning :: start()
{
    dynamicEpsilon = 1.0;
}

void Agent_Qlearning :: stop()
{
    if (dynamicEpsilon > EPSILON) {
        dynamicEpsilon -= 0.002;
    }
}
void Agent_Qlearning :: generateDebugData(int trainningEpisode)
{
    throw std::logic_error("Not implemented");
}


AgentActions Agent_Qlearning::SelectAction()
{
    if (_exploreRandom(_randomGenerator) < dynamicEpsilon) {
        return (AgentActions)_actionRandom(_randomGenerator);

    }
    return GetBestAction();
}

AgentActions Agent_Qlearning::GetBestAction()
{
    int bestAction = AgentActions::Move_up;
    double bestValue = std::numeric_limits<double>::min();

    int qTableIndex = _grid->getAgentPosition().x + _grid->getGridSize() * _grid->getAgentPosition().y;


    for (int i = 0; i < NUM_POSIBLE_ACTIONS; i++)
    {
        if (_qTable[qTableIndex][i] > bestValue)
        {
            bestValue = _qTable[qTableIndex][i];
            bestAction = i;
        }
    }
    return (AgentActions)bestAction;
}
