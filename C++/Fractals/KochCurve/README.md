# Koch Curve Fractal Generator

![Koch Curve](https://github.com/dancasmed/Trainning_2025/blob/main/C%2B%2B/Fractals/KochCurve/KochCurve.png)

## Description
A C++ implementation of the Koch curve fractal using SFML graphics library. This program demonstrates recursive fractal generation through iterative line subdivision.

## Flow
```mermaid
graph TD
    A[Start] --> B[Initialize Window & Initial Line]
    B --> C{Main Loop}
    C -->|Lines Exist| D[Process Front Line]
    D --> E[Calculate Line Length]
    E --> F{Length > 10?}
    F -->|Yes| G[Subdivide into 3 Segments]
    G --> H[Calculate Rotation Point]
    H --> I[Create 4 New Lines]
    I --> J[Replace Original Line]
    J --> C
    F -->|No| K[Remove Line]
    K --> C
    C -->|No Lines Left| L[End]

    style A fill:#ffffff,stroke:#333
    style B fill:#f0f0f0,stroke:#333
    style C fill:#e0f0ff,stroke:#333
    style D fill:#f0f0f0,stroke:#333
    style E fill:#f0f0f0,stroke:#333
    style F fill:#ffffcc,stroke:#333
    style G fill:#f0f0f0,stroke:#333
    style H fill:#f0f0f0,stroke:#333
    style I fill:#f0f0f0,stroke:#333
    style J fill:#f0f0f0,stroke:#333
    style K fill:#f0f0f0,stroke:#333
    style L fill:#ffffff,stroke:#333
```

## Folder Structure
```
KochCurve/
├── CMakeLists.txt
├── KochCurve.png
├── include/
│   └── Line.h
├── src/
│   ├── Line.cpp
│   └── main.cpp
└── build/
```

## Prerequisites
- CMake 3.10+
- SFML 2.5+ (Graphics module)
- C++ compiler (g++/clang++)

## Build Instructions (macOS)
1. Open terminal and navigate to project directory:
```bash
cd Trainning_2025/C++/Fractals/KochCurve
```
2. Create build directory and compile:
```
mkdir -p build && cd build
cmake ..
gmake
```
3. Run the executable:
```
./KochCurve
```
## Key Features 

- Interactive fractal generation
- Recursive line subdivision algorithm
- SFML-based graphical rendering
- Adjustable iteration depth
     

## Controls 

- The program runs automatically
- Close the window to exit
     