cmake_minimum_required(VERSION 3.10)
project(GridNavigationAgent LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -Wpedantic")

# Agregar fuentes
add_executable(solution 
    include/AgentAction.h
    include/DifficultyLevel.h
    include/Rewards.h
    include/RGBColor.h

    include/Agent.h
    src/Agent.cpp
    include/PuzzleGrid.h
    src/PuzzleGrid.cpp
    
    src/main.cpp

)

# Incluir directorios
target_include_directories(solution PRIVATE include)
