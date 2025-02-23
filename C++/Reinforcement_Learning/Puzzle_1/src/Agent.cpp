#include <cstdlib>

#include "Agent.h"
#include "AgentActions.h"
#include "IGrid.h"

Agent :: Agent(IGrid* grid) : _grid(grid)
{
}

AgentActions Agent :: nextMove(bool trainning)
{
    if (trainning) {
        AgentActions nextAction = (AgentActions)(rand() % 4);
        _grid->moveAgent(nextAction);
        return nextAction;
    } else {
        AgentActions nextAction = (AgentActions)(rand() % 4);
        _grid->moveAgent(nextAction);
        return nextAction;
    }
}