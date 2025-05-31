#ifndef AGENT_RANDOM
#define AGENT_RANDOM

#include <vector>

#include "AgentActions.h"
#include "IAgent.h"
#include "IGrid.h"

class Agent_Empty : public IAgent {
    private:
        

    public:
        Agent_Empty(IGrid* grid);
        AgentActions nextMove(bool trainning = false) override;
        void start() override;
        void stop() override;
        void generateDebugData(int trainningEpisode) override;
};

#endif