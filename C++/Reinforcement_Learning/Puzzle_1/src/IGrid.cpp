#include "IGrid.h"
#include <vector>
#include <ranges>

IGrid :: IGrid(int gridSize) : _gridSize(gridSize) {
    
}

int IGrid :: getGridSize()
{
    return _gridSize;
}

Position IGrid::getAgentPosition()
{
    return _agentPosition;
}

Position IGrid::positionIfDo(AgentActions agentAction)
{
    int newX = _agentPosition.x, newY = _agentPosition.y;
    if (agentAction == AgentActions::Move_up) newY--;
    else if (agentAction == AgentActions::Move_down) newY++;
    else if (agentAction == AgentActions::Move_left) newX--;
    else if (agentAction == AgentActions::Move_right) newX++;

    if (newX < 0) newX = 0;
    if (newX >= _gridSize) newX = _gridSize - 1;
    if (newY < 0) newY = 0;
    if (newY >= _gridSize) newY = _gridSize - 1;

    return Position(newX, newY);
}

Rewards IGrid :: rewardIfDo(AgentActions agentAction)
{
    Rewards result = Rewards::Regular_move;
    Position newPosition = positionIfDo(agentAction);
    bool revisitingPosition = std::ranges::find(_currentPath, newPosition) != _currentPath.end();
    
    // Verificar límites
    if (!validAction(agentAction) && !revisitingPosition) {
        result =  Rewards::Invalid_move; // Movimiento inválido
    } else if (isTarget(newPosition)) {
        result = Rewards::Reached_target;
    } else if (revisitingPosition) {
        result = Rewards::Revisiting_Position;
    }

    return result;
}

bool IGrid::validAction(AgentActions action)
{
    int newX = _agentPosition.x, newY = _agentPosition.y;
    if (action == AgentActions::Move_up) newY--;
    else if (action == AgentActions::Move_down) newY++;
    else if (action == AgentActions::Move_left) newX--;
    else if (action == AgentActions::Move_right) newX++;

    return (newX >= 0 && newX < _gridSize && newY >= 0 && newY < _gridSize);
}

bool IGrid :: isTarget(Position Position)
{
    return _targetPosition.x == Position.x && _targetPosition.y == Position.y;
}
