#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unistd.h> // Para usleep

#include "Grid.h"
#include "Rewards.h"
#include "AgentActions.h"

void Grid :: resetColor()
{
    std::cout << "\033[0m";
}

void Grid :: drawCell(int x, int y, RGBColor rgbColor, char value)
{
    std::cout << "\033[" << y + 1 << ";" << x * 3 + 1 << "H"; // Ajuste para alinear correctamente
    std::cout << "\033[38;2;" << rgbColor.red << ";" << rgbColor.green << ";" << rgbColor.blue << "m"; //set text color
    std::cout << value;
}

Grid :: Grid(int gridSize, DifficultyLevel difficultyLevel, int actionSpeed) :
    _gridSize(gridSize), _difficultyLevel(difficultyLevel), _grid(std::vector<std::vector<int>>(gridSize, std::vector<int>(gridSize, 0))), _actionSpeed(actionSpeed)
{
    
}

void Grid :: Reset() {
    for (int i = 0; i < _gridSize; ++i) {
        for (int j = 0; j < _gridSize; ++j) {
            _grid[i][j] = 0;
        }
    }
}

void Grid :: initialize()
{
    srand(time(0));
    if (_difficultyLevel == DifficultyLevel::Easy_1) {
        _agentPosition = {rand() % _gridSize, rand() % _gridSize};
        _targetPosition = {_gridSize - 1, _gridSize - 1}; 
    } else if (_difficultyLevel == DifficultyLevel::Easy_2) {
        _agentPosition = {0, 0};
        _targetPosition = {rand() % _gridSize, rand() % _gridSize}; 
    } else if (_difficultyLevel == DifficultyLevel::Medium) {
        _agentPosition = {rand() % _gridSize, rand() % _gridSize};
        _targetPosition = {rand() % _gridSize, rand() % _gridSize}; 
    } else {
        //By default the difficulty is DifficultyLevel::VeryEasy
        _agentPosition = {0, 0};
        _targetPosition = {_gridSize - 1, _gridSize - 1}; 
    }
    _grid[_agentPosition.first][_agentPosition.second] = 1;           // Marcar la posición inicial como visitada
}

bool Grid :: isTarget(std::pair<int, int> Position)
{
    return _targetPosition.first == Position.first && _targetPosition.second == Position.second;
}

std::pair<int, int> Grid :: getAgentPosition() {
    return _agentPosition;
}

Rewards Grid :: moveAgent(AgentActions agentAction)
{
    int newX = _agentPosition.first, newY = _agentPosition.second;
    if (agentAction == AgentActions::move_up) newX--;
    else if (agentAction == AgentActions::move_down) newX++;
    else if (agentAction == AgentActions::move_left) newY--;
    else if (agentAction == AgentActions::move_right) newY++;

    // Verificar límites
    if (newX < 0 || newX >= _gridSize || newY < 0 || newY >= _gridSize) {
        return Rewards::invalid_move; // Movimiento inválido
    }

    // Actualizar la posición del agente
    _agentPosition = {newX, newY};
    _grid[newX][newY]++; // Incrementar el contador de visitas

    if (isTarget(_agentPosition)) {
        return Rewards::reached_target;
    }

    // Se asegura de que solo las casillas con mas de visitas sean visibles
    if (_grid[newX][newY] > 255) {
        for (int i = 0; i < _gridSize; ++i) {
            for (int j = 0; j < _gridSize; ++j) {
                _grid[i][j]--;
            }
        }
    }
    return Rewards::regular_move;
}

// Mostrar la cuadrícula
void Grid :: displayGrid(bool isTraining) {
    std::cout << "\033[2J\033[H"; // Limpiar la pantalla
    for (int i = 0; i < _gridSize; ++i) {
        for (int j = 0; j < _gridSize; ++j) {
            if (i == _agentPosition.first && j == _agentPosition.second) {
                drawCell(j, i, RGBColor{0, 255, 0}, 'A');
            } else if (i == _targetPosition.first && j == _targetPosition.second) {
                drawCell(j, i, RGBColor{255, 0, 0}, 'T');
            } else {
                int visits = _grid[i][j];
                if (visits == 0) {
                    drawCell(j, i, RGBColor{255, 255, 255}, '.');
                } else {
                    if (isTraining) {
                        int brightness = std::min(255, visits * 1); // Aumentar brillo con visitas
                        drawCell(j, i, RGBColor{0, brightness, 0}, '*');
                    } else {
                        drawCell(j, i, RGBColor{0, 0, 255}, '*');
                    }
                }
            }
            resetColor(); // Restablecer el color después de imprimir
        }
        std::cout << std::endl;
    }

    if (!isTraining) {
        usleep(_actionSpeed * 1000);
    }
}

void Grid :: TrainAgent(Agent *agent, int trainingEpisodes) {
    for (int episode = 0; episode < trainingEpisodes; ++episode) {
        initialize();
        displayGrid();
        while (!isTarget(getAgentPosition())) {
            agent->nextMove(true);
            displayGrid();
        }
    }
}

void Grid::TestAgent(Agent *agent)
{
    initialize();
    displayGrid(false);
    Reset();
    while (!isTarget(getAgentPosition())) {
        agent->nextMove();
        displayGrid(false);
    }
    std::cout << "Test finished." << std::endl;
}
