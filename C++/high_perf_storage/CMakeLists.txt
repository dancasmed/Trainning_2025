cmake_minimum_required(VERSION 3.10)
project(HighPerfStorage LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -Wpedantic")

# Buscar la librería Zstd
find_package(PkgConfig REQUIRED)
pkg_check_modules(ZSTD REQUIRED libzstd)


# Incluir los headers de Zstd
include_directories(${ZSTD_INCLUDE_DIRS})

link_directories(${ZSTD_LIBRARY_DIRS})

# Agregar fuentes
add_executable(storage 
    src/main.cpp
    include/MarketData.h
    include/LiveDataGenerator.h
    src/LiveDataGenerator.cpp
    include/LiveDataCollector.h
    src/LiveDataCollector.cpp
    include/ThreadSafeQueue.h
    src/ThreadSafeQueue.cpp
    include/BufferedWriter.h
    src/BufferedWriter.cpp
    include/DataWriter.h
    src/DataWriter.cpp
    include/HistoricalDataReader.h
    src/HistoricalDataReader.cpp
)

# Incluir directorios
target_include_directories(storage PRIVATE include)

target_link_libraries(storage ${ZSTD_LIBRARIES})


