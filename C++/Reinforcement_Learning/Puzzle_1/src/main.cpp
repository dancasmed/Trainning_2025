#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unistd.h> // Para usleep

#include <PuzzleGrid.h>
#include "Agent.h"
#include "AgentAction.h"

using namespace std;

// Tamaño de la cuadrícula
const int GRID_SIZE = 5;
const int TRAINNING_EPISODES = 1000;
const int TEST_SPEED = 250; //milliseconds


// Simular el entrenamiento del agente
void trainAgent(PuzzleGrid& puzzleGrid, Agent& agent) {
    for (int episode = 0; episode < TRAINNING_EPISODES; ++episode) {
        puzzleGrid.initialize();
        puzzleGrid.displayGrid(true);
        while (!puzzleGrid.isTarget(puzzleGrid.getAgentPosition())) {
            agent.nextMove(true);
            puzzleGrid.displayGrid(true);
        }
    }
}

// Probar al agente
void testAgent(PuzzleGrid& puzzleGrid, Agent& agent) {
    puzzleGrid.initialize();
    puzzleGrid.displayGrid(false);
    puzzleGrid.Reset();
    while (!puzzleGrid.isTarget(puzzleGrid.getAgentPosition())) {
        agent.nextMove(true);
        puzzleGrid.displayGrid(true);
    }
}

int main() {
    PuzzleGrid puzzleGrid(GRID_SIZE, DifficultyLevel::VeryEasy, TEST_SPEED);
    Agent agent(&puzzleGrid);

    cout << "Entrenando al agente..." << endl;
    trainAgent(puzzleGrid, agent);

    cout << "Probando al agente..." << endl;
    testAgent(puzzleGrid, agent);

    cout << "¡Entrenamiento y prueba completados!" << endl;
    return 0;
}