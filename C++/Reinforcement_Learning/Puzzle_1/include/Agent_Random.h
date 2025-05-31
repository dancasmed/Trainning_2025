#ifndef AGENT_RANDOM
#define AGENT_RANDOM

#include <vector>

#include "AgentActions.h"
#include "IAgent.h"
#include "IGrid.h"

class Agent_Random : public IAgent {
    private:
        std::vector<AgentActions> _bestPath;
        std::vector<AgentActions> _currentLearnedPath;
        int _bestReward;
        int _currentReward;
        int _pathIterator;

    public:
        Agent_Random(IGrid* grid);
        AgentActions nextMove(bool trainning = false) override;
        void start() override;
        void stop() override;
        void generateDebugData(int trainningEpisode) override;
};

#endif