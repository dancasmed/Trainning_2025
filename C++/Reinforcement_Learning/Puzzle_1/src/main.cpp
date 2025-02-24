#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unistd.h> // Para usleep

#include <Grid.h>
#include "IAgent.h"
#include "Agent_Random.h"
#include "Agent_QLearning.h"
#include "AgentActions.h"

using namespace std;

// Tamaño de la cuadrícula
const int GRID_SIZE = 7;
const int TRAINNING_EPISODES = 1000;
const int TEST_SPEED = 250; //milliseconds
const bool SHOW_GRID_WHILE_TRAINING = true;

int main() {
    Grid* grid = new Grid(GRID_SIZE, DifficultyLevel::VeryEasy, TEST_SPEED);
    IAgent* agent_random = new Agent_Random(grid); 
    IAgent* agent_Qlearning = new Agent_Qlearning(grid);    

    cout << "Entrenando al agente..." << endl;
    grid->trainAgent(agent_Qlearning, TRAINNING_EPISODES, SHOW_GRID_WHILE_TRAINING);

    cout << "Probando al agente..." << endl;
    grid->testAgent(agent_Qlearning);

    cout << "¡Entrenamiento y prueba completados!" << endl;
    return 0;
}