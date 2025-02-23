#ifndef GRID
#define GRID

#include <vector>
#include "RGBColor.h"
#include "DifficultyLevel.h"
#include "AgentAction.h"
#include "Rewards.h"

class PuzzleGrid {
    private:
        const int _gridSize;
        const DifficultyLevel _difficultyLevel;
        std::vector<std::vector<int>> _grid;
        int _actionSpeed; //milliseconds
        std::pair<int, int> _agentPosition;
        std::pair<int,int> _targetPosition;

        void resetColor();
        void drawCell(int x, int y, RGBColor rgbColor, char value);
        
    public:
        PuzzleGrid(int gridSize, DifficultyLevel difficultyLevel, int actionSpeed);
        void Reset();
        void initialize();
        bool isTarget(std::pair<int, int> Position);
        std::pair<int, int> getAgentPosition();
        Rewards moveAgent(AgentAction agentAction);
        void displayGrid(bool isTraining = true);
};

#endif
