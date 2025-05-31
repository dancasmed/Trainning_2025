cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -Wpedantic")

project(spirograph LANGUAGES CXX)

# Configuración de SFML
set(SFML_DIR "/opt/homebrew/Cellar/sfml/2.6.2/lib/cmake/SFML")
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# Directorios de inclusión
include_directories(${SFML_INCLUDE_DIR})
include_directories(/opt/homebrew/Cellar/sfml/2.6.2/include)

# Archivos fuente (ajusta según tus archivos)
set(SOURCES
    src/spirograph.cpp
    # Agrega aquí otros archivos .cpp
)

# Ejecutable
add_executable(${PROJECT_NAME} ${SOURCES})

# Enlazar SFML
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
    sfml-graphics
    sfml-window
    sfml-system
)

target_include_directories(${PROJECT_NAME} PRIVATE include)

# Opciones de compilación
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fdiagnostics-color=always")