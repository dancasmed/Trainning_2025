#include "IGrid.h"
#include <vector>

IGrid :: IGrid(int gridSize) : _gridSize(gridSize) {
    
}

int IGrid :: getGridSize()
{
    return _gridSize;
}

std::pair<int, int> IGrid::getAgentPosition()
{
    return _agentPosition;
}
