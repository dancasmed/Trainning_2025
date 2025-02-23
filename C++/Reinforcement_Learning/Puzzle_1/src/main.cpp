#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unistd.h> // Para usleep

#include <Grid.h>
#include "Agent.h"
#include "AgentActions.h"

using namespace std;

// Tamaño de la cuadrícula
const int GRID_SIZE = 5;
const int TRAINNING_EPISODES = 1000;
const int TEST_SPEED = 250; //milliseconds

int main() {
    IGrid* grid = new Grid(GRID_SIZE, DifficultyLevel::VeryEasy, TEST_SPEED);
    Agent agent(grid);

    cout << "Entrenando al agente..." << endl;
    ((Grid*)grid)->TrainAgent(&agent, TRAINNING_EPISODES);
    

    cout << "Probando al agente..." << endl;
    ((Grid*)grid)->TestAgent(&agent);

    cout << "¡Entrenamiento y prueba completados!" << endl;
    return 0;
}