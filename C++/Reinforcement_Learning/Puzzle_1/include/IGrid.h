#ifndef IGRID
#define IGRID

#include "AgentActions.h"
#include "Rewards.h"

class IGrid {
    public:
        virtual Rewards moveAgent(AgentActions agentAction) = 0;
};

#endif