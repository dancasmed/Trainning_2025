cmake_minimum_required(VERSION 3.10)
project(GridNavigationAgent LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -Wpedantic")

# Para Homebrew/Conan/vcpkg
find_package(nlohmann_json REQUIRED)

# Agregar fuentes
add_executable(solution 
    src/IAgent.cpp
    src/Agent_Empty
    src/Agent_GRPO.cpp
    src/Agent_QLearning.cpp
    src/Agent_Random.cpp
    src/Grid.cpp
    src/IGrid.cpp
    src/main.cpp
)

# Incluir directorios
target_include_directories(solution PRIVATE include)
target_link_libraries(solution PRIVATE nlohmann_json::nlohmann_json)