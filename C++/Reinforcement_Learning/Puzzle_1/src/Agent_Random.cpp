#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include <iostream>

#include "Agent_Random.h"

Agent_Random::Agent_Random(IGrid *grid) : IAgent(grid)
{
}

AgentActions Agent_Random :: nextMove(bool trainning)
{
    if (trainning) {
        AgentActions nextAction = (AgentActions)(rand() % AgentActions::NUM_ACTIONS);
        _currentReward += _grid->moveAgent(nextAction);
        _currentLearnedPath.emplace_back(nextAction);
        return nextAction;
    } else {
        _grid->moveAgent(_bestPath[_pathIterator]);
        return _bestPath[_pathIterator++];
    }
}

void Agent_Random :: start()
{
    _currentLearnedPath.clear();
    _pathIterator = 0;
}

void Agent_Random :: stop()
{
    if (_bestPath.size() == 0 || (_currentReward > _bestReward && _currentLearnedPath.size() <= _bestPath.size())) {
        _bestPath = _currentLearnedPath;
        _bestReward = _currentReward;
    }
}

void Agent_Random :: generateDebugData(int trainningEpisode)
{
    json j = json {
        {"trainning_episode", trainningEpisode},
        {"debug_data", {
            {"bestPath", _bestPath},
            {"currentLearnedPath", _currentLearnedPath},
            {"bestReward", _bestReward},
            {"currentReward", _currentReward}
        }}
    };
    std::cout << j.dump(4);
}
