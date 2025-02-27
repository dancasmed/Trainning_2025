#ifndef IGRID
#define IGRID

#include <vector>

#include "AgentActions.h"
#include "Rewards.h"
#include "Position.h"

class IGrid {
    protected:
        const int _gridSize;
        Position _agentPosition;
        bool isTarget(Position position);
        Position _targetPosition;
        std::vector<Position> _currentPath;

    public:

        virtual Rewards moveAgent(AgentActions agentAction) = 0;

        IGrid(int gridSize);
        virtual ~IGrid() = default; 
        int getGridSize();
        Position getAgentPosition();
        Position positionIfDo(AgentActions agentAction);
        Rewards rewardIfDo(AgentActions agentAction);
        bool validAction(AgentActions action);

};

#endif