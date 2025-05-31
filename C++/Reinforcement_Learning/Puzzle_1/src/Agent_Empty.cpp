#include "Agent_Empty.h"

Agent_Empty :: Agent_Empty(IGrid *grid) : IAgent(grid)
{
}

AgentActions Agent_Empty :: nextMove(bool trainning)
{
    if (trainning) {
        throw std::logic_error("Not implemented");
    } else {
        throw std::logic_error("Not implemented");
    }
}

void Agent_Empty :: start()
{
    throw std::logic_error("Not implemented");
}

void Agent_Empty :: stop()
{
    throw std::logic_error("Not implemented");
}

void Agent_Empty :: generateDebugData(int trainningEpisode)
{
    throw std::logic_error("Not implemented");
}
