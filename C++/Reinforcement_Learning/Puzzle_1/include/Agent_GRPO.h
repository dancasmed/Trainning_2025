#ifndef AGENT_GRPO
#define AGENT_GRPO

#include <random>

#include "IAgent.h"
#include "IGrid.h"
#include "AgentActions.h"
#include "Position.h"

class Agent_GRPO : public IAgent {    
    private:
        const int PERCENTAGE_ACTIONS_TO_TRY = 75; // Porcentaje de acciones que se van a probar
        const double LEARNING_RATE = 0.1;
        const double CLIP_EPSILON = 0.2;

        std::vector< std::vector< std::vector<double>>> _policy;

        std::random_device _randomDevice;           ///< Random device for seed generation
        std::mt19937 _randomGenerator;                ///< Mersenne Twister random number engine
        std::uniform_real_distribution<double> _selectActionRandom;

        void InitializePolicy();
        AgentActions SelectAction();
        AgentActions GetBestAction();

    public:
        Agent_GRPO(IGrid* grid);
        AgentActions nextMove(bool trainning = false) override;
        void start() override;
        void stop() override;
        void generateDebugData(int trainningEpisode) override;
};

#endif
