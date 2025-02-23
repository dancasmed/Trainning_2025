#ifndef AGENT
#define AGENT

#include "AgentAction.h"
#include "PuzzleGrid.h"

class Agent {
    private:
        PuzzleGrid* _puzzleGrid;

    public:
        Agent(PuzzleGrid* puzzleGrid);
        AgentAction nextMove(bool trainning = false);
};

#endif
