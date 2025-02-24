# Reinforcement Learning Grid Agent Project

This project implements a reinforcement learning environment where an agent learns to navigate a grid to reach a target position . The project includes two types of agents: `Agent_Qlearning` (a Q-learning-based agent) and `Agent_Random` (a random-action agent). The environment is defined by a grid with configurable difficulty levels, size, and speed of execution. Visit this page for more details about the puzzle to be solved, [Puzzle details](Puzzle.md).

## Table of Contents

- [Project Structure](#project-structure)
- [Dependencies](#dependencies)
- [Building the Project](#building-the-project)
- [Running the Project](#running-the-project)
- [Code Overview](#code-overview)
  - [Agent Classes](#agent-classes)
  - [Grid Class](#grid-class)
  - [Enums and Structs](#enums-and-structs)
- [Training and Testing](#training-and-testing)

---

## Project Structure

The project is organized into the following directories:
```
Puzzle_1/
├── build/            # Compiled binaries and object files
├── include/          # Header files (.h)
├── src/              # Source files (.cpp)
├── CMakeLists.txt    # CMake configuration file
└── README.md         # This documentation file
```

---

## Dependencies

- **C++17 or later**: The code uses features from the C++17 standard.
- **CMake**: Required for building the project.
- **Standard Libraries**: The project relies on the C++ Standard Library (`<vector>`, `<random>`, `<iostream>`, etc.).

---

## Building the Project (Mac OS)

To compile the project, follow these steps:

1. Navigate to the `build` directory:
   ```bash
   cd Proyecto/build
   ```
2. Generate the Makefile using CMake:
```
cmake ..
```
Compile the project using `gmake`:
```
gmake
```
The compiled binary will be located in the `build` directory.

## Running the Project 

After building the project, you can run the executable: 
```
./solution
```
The program performs the following steps: 
    
1. Trains the `Agent_Qlearning` agent over a specified number of episodes.
2. Tests the trained agent to evaluate its performance.
3. Displays the results in the terminal.
     

You can configure the grid size, training episodes, and other parameters in the `main.cpp` file. 

## Code Overview 
### Agent Classes 
`Agent_Qlearning`

- Implements a Q-learning algorithm to train the agent.
- Maintains a Q-table (`_qTable`) to store state-action values.
- Uses an epsilon-greedy strategy for exploration-exploitation.
   - Key methods:
       - `nextMove(bool trainning)`: Determines the next action based on the training mode.
       - `start()`: Initializes the agent's dynamic epsilon value.
       - `stop()`: Decreases the epsilon value after each episode.
         
     
`Agent_Random `

- Implements a random-action policy for comparison.
- Stores the best path found during training.
- Key methods:
    - nextMove(bool trainning): Selects a random action during training or follows the best path during testing.
    - start(): Resets the agent's state before training or testing.
    - stop(): Updates the best path and reward after training.
         
     

### Grid Class 

The `Grid` class defines the environment where the agent operates. It includes: 

- A grid of configurable size (`_gridSize`).
- Difficulty levels (`DifficultyLevel`) that determine the initial positions of the agent and target.
- Methods to move the agent (`moveAgent`) and update the grid state (`updateVisitsMatrix`).
- Visualization methods (`displayGrid`) to render the grid in the terminal.
     

### Enums and Structs 
`AgentActions`

Defines the possible actions an agent can take: 

```cpp
enum AgentActions {
    Move_up,
    Move_down,
    Move_left,
    Move_right
};
```

`Rewards`

Defines the rewards for different outcomes: 
```cpp
enum Rewards {
    Reached_target = 1000,
    Regular_move = -1,
    Invalid_move = -5,
};
```

`RGBColor`

Represents a color in RGB format: 
```cpp
struct RGBColor {
    int red;
    int green;
    int blue;
    RGBColor(int r, int g, int b) : red(r), green(g), blue(b) {}
};
```

`DifficultyLevel`

Specifies the difficulty levels for the grid: 
```cpp
enum DifficultyLevel {
    VeryEasy,
    Easy_1,
    Easy_2,
    Medium
};
```

### Training and Testing 
#### Training 

- The `trainAgent` method in the `Grid` class trains the agent over a specified number of episodes.
- During training, the grid can optionally display the agent's movements in real-time (`SHOW_GRID_WHILE_TRAINING`).
     

#### Testing 

- The `testAgent` method evaluates the trained agent's performance.
- The grid visualization is displayed at a configurable speed (`TEST_SPEED`).
     

#### Configuration 

You can modify the following parameters in `main.cpp`: 

- `GRID_SIZE`: Size of the grid (default: 7).
- `TRAINNING_EPISODES`: Number of training episodes (default: 1000).
- `TEST_SPEED`: Speed of grid updates during testing (in milliseconds, default: 250).
- `SHOW_GRID_WHILE_TRAINING`: Whether to display the grid during training (default: `true`).
     

### Notes 

- Ensure that all header files are placed in the `include` directory and source files in the `src` directory.
- Modify the `CMakeLists.txt` file if additional source files or libraries are added.
- The project is designed for educational purposes and can be extended to include more complex environments, obstacles, or algorithms.
     