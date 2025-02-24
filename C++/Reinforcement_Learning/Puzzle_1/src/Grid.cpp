#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unistd.h> // Para usleep
#include <unordered_map>

#include "Grid.h"
#include "Rewards.h"
#include "AgentActions.h"
#include "IAgent.h"

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
    IGrid(gridSize),
    _difficultyLevel(difficultyLevel),
    _grid(std::vector<std::vector<int>>(gridSize, std::vector<int>(gridSize, 0))),
    _actionSpeed(actionSpeed)
{
    
}

void Grid :: resetVisitsMatrix() {
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

Rewards Grid :: moveAgent(AgentActions agentAction)
{
    int newX = _agentPosition.first, newY = _agentPosition.second;
    if (agentAction == AgentActions::Move_up) newX--;
    else if (agentAction == AgentActions::Move_down) newX++;
    else if (agentAction == AgentActions::Move_left) newY--;
    else if (agentAction == AgentActions::Move_right) newY++;

    // Verificar límites
    if (newX < 0 || newX >= _gridSize || newY < 0 || newY >= _gridSize) {
        return Rewards::Invalid_move; // Movimiento inválido
    }

    // Actualizar la posición del agente
    _agentPosition = {newX, newY};
    _grid[newX][newY]++; // Incrementar el contador de visitas

    if (isTarget(_agentPosition)) {
        return Rewards::Reached_target;
    }

    // Se asegura de que solo las casillas con mas de visitas sean visibles
    if (_grid[newX][newY] > 255) {
        /*for (int i = 0; i < _gridSize; ++i) {
            for (int j = 0; j < _gridSize; ++j) {
                _grid[i][j]--;
            }
        }*/
        updateVisitsMatrix();
    }
    
    return Rewards::Regular_move;
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
                        int brightness = std::min(255, visits); // Aumentar brillo con visitas
                        /*int brightness_1 = visits % 255 ;
                        int brightness_2 = (visits - brightness_1) % 255;
                        int brightness_3 = (visits - brightness_1 - brightness_2) % 255;
                        */
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

void Grid::updateVisitsMatrix()
{
    // Paso 1: Extraer todos los valores de la matriz
    std::vector<int> allValues;
    for (const auto& row : _grid) {
        for (int value : row) {
            allValues.push_back(value);
        }
    }

    // Paso 2: Ordenar los valores de forma descendente y seleccionar los 255 mayores
    std::sort(allValues.begin(), allValues.end(), std::greater<int>());
    int numTopValues = std::min(255, (int)allValues.size());
    std::vector<int> topValues(allValues.begin(), allValues.begin() + numTopValues);

    // Paso 3: Crear un mapa para verificar rápidamente si un valor está en la lista de máximos
    std::unordered_map<int, int> valueToIndexMap;
    for (size_t i = 0; i < topValues.size(); ++i) {
        valueToIndexMap[topValues[i]] = numTopValues - i; // Asignar índices empezando desde numTopValues
    }

    // Paso 4: Actualizar la matriz según las reglas dadas
    for (auto& row : _grid) {
        for (int& value : row) {
            if (valueToIndexMap.find(value) != valueToIndexMap.end()) {
                value = valueToIndexMap[value]; // Asignar el índice del valor en la lista
            } else {
                value = 0; // Asignar 0 si no está en la lista
            }
        }
    }
}

void Grid :: trainAgent(IAgent* agent, int trainingEpisodes, bool showGrid) {
    for (int episode = 0; episode < trainingEpisodes; ++episode) {
        initialize();
        agent->start();
        while (!isTarget(_agentPosition)) {
            agent->nextMove(true);
            if (showGrid) {
                displayGrid();
                usleep(1 * 500);
            }
        }
        agent->stop();
    }
}

void Grid :: testAgent(IAgent* agent)
{
    initialize();
    displayGrid(false);
    resetVisitsMatrix();
    agent->start();
    while (!isTarget(_agentPosition)) {
        agent->nextMove();
        displayGrid(false);
    }
    agent->stop();
    std::cout << "Test finished." << std::endl;
}
