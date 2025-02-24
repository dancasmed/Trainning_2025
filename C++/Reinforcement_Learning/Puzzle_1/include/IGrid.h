#ifndef IGRID
#define IGRID

#include <vector>

#include "AgentActions.h"
#include "Rewards.h"

class IGrid {
    protected:
        const int _gridSize;
        std::pair<int, int> _agentPosition;
    public:

        virtual Rewards moveAgent(AgentActions agentAction) = 0;

        IGrid(int gridSize);
        int getGridSize();
        std::pair<int, int> getAgentPosition();
};

#endif