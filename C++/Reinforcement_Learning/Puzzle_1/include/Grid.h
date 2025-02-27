#ifndef GRID
#define GRID

#include <vector>
#include "RGBColor.h"
#include "DifficultyLevel.h"
#include "IAgent.h"
#include "AgentActions.h"
#include "IGrid.h"
#include "Rewards.h"
#include "Position.h"

class Grid : public IGrid {
    private:
        const DifficultyLevel _difficultyLevel;
        std::vector<std::vector <int> > _grid;
        int _actionSpeed; //milliseconds
        std::vector<Position> _path;
        Position _lastPosition = { -1, -1 };

        void resetColor();
        void drawCell(int x, int y, RGBColor rgbColor, char value);
        void resetVisitsMatrix();
        void initialize();
        void displayGrid(bool isTraining = true);
        void remarkPath();
        void updateVisitsMatrix();
        
    public:
        Grid(int gridSize, DifficultyLevel difficultyLevel, int actionSpeed);
        void trainAgent(IAgent* agent, int trainingEpisodes, bool showGrid);
        void testAgent(IAgent* agent);

        Rewards moveAgent(AgentActions agentAction) override;
};

#endif
