#ifndef GRID
#define GRID

#include <vector>
#include "RGBColor.h"
#include "DifficultyLevel.h"
#include "Agent.h"
#include "AgentActions.h"
#include "IGrid.h"
#include "Rewards.h"

class Grid : public IGrid {
    private:
        const int _gridSize;
        const DifficultyLevel _difficultyLevel;
        std::vector<std::vector<int>> _grid;
        int _actionSpeed; //milliseconds
        std::pair<int, int> _agentPosition;
        std::pair<int,int> _targetPosition;

        void resetColor();
        void drawCell(int x, int y, RGBColor rgbColor, char value);
        void Reset();
        void initialize();
        bool isTarget(std::pair<int, int> Position);
        std::pair<int, int> getAgentPosition();
        void displayGrid(bool isTraining = true);
        
    public:
        Grid(int gridSize, DifficultyLevel difficultyLevel, int actionSpeed);
        Rewards moveAgent(AgentActions agentAction) override;
        void TrainAgent(Agent *agent, int trainingEpisodes);
        void TestAgent(Agent* agent);
};

#endif
