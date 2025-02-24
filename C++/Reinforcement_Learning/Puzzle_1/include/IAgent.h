#ifndef AGENT_BASE
#define AGENT_BASE

#include <vector>

#include "AgentActions.h"
#include "IGrid.h"

class IAgent {
    protected:
        IGrid* _grid;

    public:
        IAgent(IGrid* grid);

        virtual AgentActions nextMove(bool trainning = false) = 0;
        virtual void start() = 0;
        virtual void stop() = 0;
        
};

#endif
