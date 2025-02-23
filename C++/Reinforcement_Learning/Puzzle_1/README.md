# Programming Puzzle: Grid Navigation Agent 1

## Problem Description

You are tasked with simulating an agent that learns to navigate a N x N grid to reach a target location. Each cell in the grid represents a state, and the agent can move in four possible directions: **up**, **down**, **left**, or **right**. The goal is for the agent to learn the optimal path from its starting position to the target position.

### Rules of the Grid
1. N >= 1 and N <= 30
2. The grid has dimensions **N x N** (indexed from `(0, 0)` to `(N, N)`).
3. The agent starts at a position on the grid depending on the puzzle difficulty level.
   - Very easy, Starts at (0, 0)
   - Easy 1, Starts at (Random, Random)
   - Easy 2, Starts at (0, 0)
   - Medium, Starts at (Random, Random)
4. The target is located at a position on the grid depending on the puzzle difficulty level.
   - Very easy, Agent starts at (0, 0) and Target is at (N, N)
   - Easy 1, Target is located at (N, N)
   - Easy 2, Target is located at (Random, Random)
   - Medium, Target is located at (Random, Random)
5. The agent can move one step in any of the four directions: **up**, **down**, **left**, or **right**.
6. If the agent attempts to move outside the boundaries of the grid, it remains in its current position.
7. The agent receives:
   - A reward of **+100** for reaching the target.
   - A reward of **-1** for each step taken to encourage efficiency.
   - A reward of **-5** for attempting to move outside the grid boundaries.

### Objective
Implement a program that allows the agent to learn how to navigate the grid using reinforcement learning techniques. The agent should eventually learn the shortest path to the target.

---

## Requirements

1. **Grid Representation**:
   - Represent the grid as a 2D array or matrix where each cell corresponds to a state.
   - Display the agent's current position and the target position visually (e.g., using symbols like `A` for the agent and `T` for the target).

2. **Agent Actions**:
   - Implement the four possible actions: **up**, **down**, **left**, and **right**.
   - Ensure boundary checks so the agent cannot move outside the grid.

3. **Reward System**:
   - Implement the reward system as described above.
   - Track the total reward accumulated by the agent during its journey.

4. **Learning Algorithm**:
   - Use one of the following reinforcement learning algorithms to train the agent:
     ### Q-Learning
     - **Description**: Q-Learning is a model-free RL algorithm that learns the value of actions in particular states without requiring a model of the environment. It updates the Q-value (expected future reward) for each state-action pair based on the observed reward and the maximum Q-value of the next state.
     - **Key Features**:
       - Model-free: Does not require knowledge of the environment's dynamics.
       - Off-policy: Can learn the optimal policy while following a different behavior policy.
     - **Advantages**:
       - Simple to implement.
       - Effective in discrete state and action spaces.
     
     ### Group Relative Policy Optimization (GRPO)
     - **Description**: GRPO is a variant of policy optimization algorithms designed for multi-agent reinforcement learning scenarios. It focuses on optimizing policies relative to the performance of other agents in the group, enabling better coordination and competition among agents.
     - **Key Features**:
       - Relative performance metrics for multi-agent systems.
       - Balances individual and group objectives.
     - **Advantages**:
       - Enhances cooperation or competition in multi-agent environments.
       - Scalable to large groups of agents.
     
     ### Proximal Policy Optimization (PPO)
     - **Description**: PPO is a policy optimization algorithm that improves upon earlier methods like TRPO (Trust Region Policy Optimization) by simplifying the optimization process while maintaining stability. It clips the objective function to prevent large policy updates, ensuring stable learning.
     - **Key Features**:
       - Clipped objective function for stable updates.
       - Simple and efficient implementation.
     - **Advantages**:
       - Balances simplicity and performance.
       - Widely used in practice due to its effectiveness.

5. **Simulation**:
   - Run multiple episodes of training where the agent starts at a random position and tries to reach the target.
   - Visualize the agent's progress after each episode (e.g., print the grid with the agent's position).

6. **Output**:
   - After training, display the learned optimal path from a given starting position to the target.
   - Print the total reward accumulated during the final episode.

---

## Example Input/Output

### Initial Grid
```
A . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . T
```


### Episode Progression
- Episode 1: Agent takes 20 steps, total reward = -20.
- Episode 100: Agent takes 12 steps, total reward = -12.
- Episode 500: Agent takes 10 steps (optimal path), total reward = -10 + 100 = 90.

### Final Output
```
Optimal Path:
(0, 0) -> (1, 0) -> (2, 0) -> ... -> (9, 9)
Total Reward: 90
```
