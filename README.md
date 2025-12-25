# Rubik’s Cube Simulator and Solver using C++ and Raylib

This project is a Rubik’s Cube simulator and solver written entirely in C++. The program models a standard 3×3×3 Rubik’s Cube using a 6 × 3 × 3 array representation and provides both logical cube operations and a graphical user interface using the Raylib library. The project also integrates the Kociemba two-phase solving algorithm by invoking the external Kociemba solver installed on the system.

The project consists mainly of two source files: cube.cpp and cube_ui.cpp. The file cube.cpp contains all core cube logic. It stores the cube state as a 6 × 3 × 3 integer array, where each face corresponds to a fixed color index. This file implements all fundamental cube operations including clockwise and counter-clockwise face rotations (R, L, U, D, F, B), scrambling logic, cube state printing for debugging, and conversion of the cube state into a facelet string compatible with the Kociemba solver. The solver is not embedded directly in the code; instead, cube.cpp generates a valid facelet string and calls the Kociemba solver through a system command, then reads and processes the solution returned by it. For this reason, the Kociemba solver must already be installed and accessible in the system PATH.

The file cube_ui.cpp implements a graphical user interface using the Raylib graphics library. It visually displays the cube in a 2D net layout, showing all six faces simultaneously. User interactions such as button presses trigger cube moves, and the displayed cube updates immediately after each operation. All cube transformations performed in the UI are delegated to the logic defined in cube.cpp, ensuring a clean separation between logic and presentation. The UI uses a custom font file (Inter-Black.otf) for rendering text and supports window resizing for better usability.

To build and run this project, a C++ compiler, Raylib, and the Kociemba solver must be installed on the system. On Arch Linux, a compiler can be installed using `sudo pacman -S gcc`, and Raylib can be installed using `sudo pacman -S raylib`. The Kociemba solver should be installed separately using pipx by running `sudo pacman -S python-pipx` followed by `pipx install kociemba`. The installation can be verified by running the command `kociemba "UUUUUUUUURRRRRRRRRFFFFFFFFFDDDDDDDDDLLLLLLLLLBBBBBBBBB"` and checking that a valid solution string is printed.

Once the dependencies are installed, the program can be compiled from the project directory using the following command:

`g++ -std=c++17 -O2 cube_ui.cpp cube.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -o cube_ui`

After successful compilation, the application can be run using:

`./cube_ui`

This project is primarily intended for educational and experimental purposes, focusing on understanding cube representation, transformations, and solver integration rather than performance optimization. It is best tested on Linux systems and works well on Arch Linux environments using either X11 or Wayland.

