#ifndef AGENT_QLEARNING
#define AGENT_QLEARNING

#include <vector>
#include <random>

#include "AgentActions.h"
#include "IAgent.h"
#include "IGrid.h"

class Agent_Qlearning : public IAgent {
    private:
        std::vector<std::vector<double>> _qTable; // Tabla de valores Q(s, a)
        std::random_device _randomDevice;           ///< Random device for seed generation
        std::mt19937 _randomGenerator;                ///< Mersenne Twister random number engine
        // Parámetros del algoritmo
        const int NUM_POSIBLE_ACTIONS = 4;      // Numero de posibles acciones que hace el agente
        const double LEARNING_RATE = 0.1;   // Tasa de aprendizaje (alpha)
        const double DISCOUNT_FACTOR = 0.9; // Factor de descuento (gamma)
        const double EPSILON = 0.2;        // Probabilidad de exploración (epsilon-greedy)
        std::uniform_real_distribution<double> _exploreRandom;
        std::uniform_int_distribution<> _actionRandom;
        double dynamicEpsilon = EPSILON;

        AgentActions SelectAction();
        AgentActions GetBestAction();

    public:
        Agent_Qlearning(IGrid* grid);
            AgentActions nextMove(bool trainning = false) override;
            void start() override;
            void stop() override;
};

#endif