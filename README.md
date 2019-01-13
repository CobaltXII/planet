# Planet

This repository contains source code for a simple procedural planet generator. The code is written in C++11. The code uses OpenGL 3.3 for hardware graphics acceleration and libnoise for coherent noise generation. The planets start off as an icosahedron. The icosahedron is then subdivided 8 times, after which the icosahedron becomes an icosphere. The vertices are then perturbed by a three-dimensional noise field which creates basins, mountain ranges and water. The elevation of the perturbed vertices is used to determine the color of the vertices. Further preprocessing is applied to create smooth water.

![Alt text](https://github.com/CobaltXII/planet/blob/master/img/screenshot_6.png?raw=true)

# Dependencies

The code in this repository requires OpenGL 3.3 Core or greater, GLM 0.9.9.3 or greater, libnoise 1.0.0 or greater, and SDL 2.0.9 or greater in order to compile and run successfully. Other dependencies such as GLAD, khrplatform and noiseutils are included in the repository.

# Compiling

Since this project is extremely small, no Makefile or CMakeLists.txt is provided. It should be trivial to compile, just link OpenGL 3.3 Core or greater, SDL 2.0.0 or greater, and libnoise. The source files planet.cpp, glad.c and noiseutils.cpp should be compiled. This command should suffice on most platforms:

```bash
clang++ -std=c++11 planet.cpp noiseutils.cpp glad.c -o planet.o -lGL -lSDL2 -llibnoise -Ofast && ./planet.o
```

# License

This repository and it's contents are licensed under the GNU GPLv3.