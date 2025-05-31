#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unistd.h> // Para usleep

#include <Grid.h>
#include "IAgent.h"
#include "Agent_Random.h"
#include "Agent_QLearning.h"
#include "Agent_GRPO.h"
#include "AgentActions.h"

using namespace std;

// Tamaño de la cuadrícula
const int GRID_SIZE = 7;
const int TRAINNING_EPISODES = 100;
const int TEST_SPEED = 250; //milliseconds
const bool SHOW_GRID_WHILE_TRAINING = false;

int main() {
    auto grid = std::make_unique<Grid>(GRID_SIZE, DifficultyLevel::VeryEasy, TEST_SPEED);

    auto agent_random = std::make_unique<Agent_Random>(grid.get());
    auto agent_Qlearning = std::make_unique<Agent_Qlearning>(grid.get());
    auto agent_GRPO = std::make_unique<Agent_GRPO>(grid.get());

    IAgent* agentToTest = agent_random.get();

    std::cout << "\033[2J\033[H"; // Limpiar la pantalla
    cout << "Entrenando al agente..." << endl;
    usleep(1000 * 1000);
    std::cout << "\033[2J\033[H"; // Limpiar la pantalla
    grid->trainAgent(agentToTest, TRAINNING_EPISODES, SHOW_GRID_WHILE_TRAINING);
    cout<< "Training finished." << endl;
    agentToTest->generateDebugData(1000);
    cout << "Probando al agente..." << endl;
    usleep(1000 * 1000);
    //std::cout << "\033[2J\033[H"; // Limpiar la pantalla
    //grid->testAgent(agentToTest);

    cout << "¡Entrenamiento y prueba completados!" << endl;
    return 0;
}