#ifndef AGENT
#define AGENT

#include "AgentActions.h"
#include "IGrid.h"

class Agent {
    private:
        IGrid* _grid;

    public:
        Agent(IGrid* grid);
        AgentActions nextMove(bool trainning = false);
};

#endif
