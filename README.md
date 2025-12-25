# Rubik's Cube Simulator (Raylib UI + CLI)

A fun little Rubik's Cube simulator featuring:
- A **Raylib-based T-net UI** with clickable stickers  
- A complete **Cube class** supporting moves, scrambles, facelet conversion, and optional Kociemba solving  
- Manual color editing mode with validation  
- Support for universal/Kociemba move notation (`R U R' U' F2`)  

---

## 📦 Files in this Repository

### `main_ui.cpp`
The graphical front-end built with **raylib**, including:
- T-net cube rendering  
- Move buttons (R/L/U/D/F/B with CW, CCW, and 2x moves)  
- Scramble, Reset, Solve (console), Toggle Color Mode, Apply Moves, Apply Colors  
- Input box for move sequences  
- Console output area  
- Sticker-click color editing mode  

### `cube.cpp`
The cube engine:
- `grid[6][3][3]` representation  
- All face moves (R L U D F B and inverses)  
- Scramble logic  
- Universal-notation parser  
- Kociemba facelet conversion + solver wrapper  
- State validation helpers  

---

## 🚀 Prerequisites

### Required
- **C++17 compiler** (`g++`, `clang++`, etc.)
- **raylib** graphics library (and development headers)
- A font file named **`Inter-Black.otf`** placed at:
