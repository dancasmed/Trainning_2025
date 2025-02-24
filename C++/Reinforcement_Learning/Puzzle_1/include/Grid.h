#ifndef GRID
#define GRID

#include <vector>
#include "RGBColor.h"
#include "DifficultyLevel.h"
#include "IAgent.h"
#include "AgentActions.h"
#include "IGrid.h"
#include "Rewards.h"

class Grid : public IGrid {
    private:
        const DifficultyLevel _difficultyLevel;
        std::vector<std::vector<int>> _grid;
        int _actionSpeed; //milliseconds
        std::pair<int,int> _targetPosition;

        void resetColor();
        void drawCell(int x, int y, RGBColor rgbColor, char value);
        void resetVisitsMatrix();
        void initialize();
        bool isTarget(std::pair<int, int> Position);
        void displayGrid(bool isTraining = true);
        void updateVisitsMatrix();
        
    public:
        Grid(int gridSize, DifficultyLevel difficultyLevel, int actionSpeed);
        Rewards moveAgent(AgentActions agentAction) override;
        void trainAgent(IAgent* agent, int trainingEpisodes, bool showGrid);
        void testAgent(IAgent* agent);
};

#endif
