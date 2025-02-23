#include "Agent.h"
#include "AgentAction.h"

Agent :: Agent(PuzzleGrid* puzzleGrid) : _puzzleGrid(puzzleGrid)
{
}

AgentAction Agent :: nextMove(bool trainning)
{
    if (trainning) {
        AgentAction nextAction = (AgentAction)(rand() % 4);
        _puzzleGrid->moveAgent(nextAction);
        return nextAction;
    } else {
        AgentAction nextAction = (AgentAction)(rand() % 4);
        _puzzleGrid->moveAgent(nextAction);
        return nextAction;
    }
}