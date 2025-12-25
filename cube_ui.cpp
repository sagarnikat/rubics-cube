#include <raylib.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <functional>
#include <algorithm>

// include your cube implementation (without main)
#include "cube.cpp"

// -------------------- Global font and sizes --------------------
Font gFont;
bool gFontLoaded = false;

// Centralized font sizes
const int UI_FONT_TITLE  = 26;
const int UI_FONT_LABEL  = 16;
const int UI_FONT_BUTTON = 18;
const int UI_FONT_INPUT  = 14;
const int UI_FONT_MSG    = 14;

// Path to font file
const char *FONT_PATH = "Inter-Black.otf";

// Helper wrappers for drawing/measuring text
void DrawTextUI(const char *text, int x, int y, int fontSize, Color color) {
    if (gFontLoaded && gFont.texture.id != 0) {
        DrawTextEx(gFont, text, (Vector2){(float)x, (float)y}, (float)fontSize, 0.0f, color);
    } else {
        DrawText(text, x, y, fontSize, color);
    }
}

int MeasureTextUI(const char *text, int fontSize) {
    if (gFontLoaded && gFont.texture.id != 0) {
        Vector2 sz = MeasureTextEx(gFont, text, (float)fontSize, 0.0f);
        return (int)sz.x;
    } else {
        return MeasureText(text, fontSize);
    }
}

// -------------------- Color mapping --------------------
Color colorForFaceletChar(char c) {
    switch (c) {
        case 'F': return WHITE;   // Front (White)
        case 'R': return RED;     // Right (Red)
        case 'B': return YELLOW;  // Back (Yellow)
        case 'L': return ORANGE;  // Left (Orange)
        case 'U': return BLUE;    // Up (Blue)
        case 'D': return GREEN;   // Down (Green)
        default: return GRAY;
    }
}

// -------------------- UI state --------------------
struct UIButton { 
    Rectangle rect; 
    std::string label; 
    std::function<void()> onClick; 
};

struct ColorPicker {
    bool active = false;
    int faceIndex = -1;
    int row = -1;
    int col = -1;
    std::vector<char> colors = {'F','R','B','L','U','D'};
};

// Function to perform basic cube state validation
bool validateCubeStateBasic(const std::string& facelets) {
    if (facelets.length() != 54) {
        return false;
    }
    
    // Count each color to ensure we have exactly 9 of each
    int colorCount[6] = {0}; // F, R, B, L, U, D
    for (char c : facelets) {
        switch (c) {
            case 'F': colorCount[0]++; break;
            case 'R': colorCount[1]++; break;
            case 'B': colorCount[2]++; break;
            case 'L': colorCount[3]++; break;
            case 'U': colorCount[4]++; break;
            case 'D': colorCount[5]++; break;
            default: return false; // Invalid character
        }
    }
    
    // Check if we have exactly 9 of each color
    for (int i = 0; i < 6; i++) {
        if (colorCount[i] != 9) {
            return false;
        }
    }
    
    return true;
}

// Function to validate if a cube state is solvable using Kociemba (for final validation)
bool validateCubeStateWithKociemba(const std::string& facelets) {
    if (!validateCubeStateBasic(facelets)) {
        return false;
    }
    
    // Try to validate with Kociemba
    Cube tempCube;
    try {
        tempCube.setFacelets(facelets);
        auto [out, code] = tempCube.runKociembaWithFacelets(facelets);
        return (code == 0); // Valid if Kociemba returns success
    } catch (...) {
        return false;
    }
}

// -------------------- Main --------------------
int main() {
    // Increased window size for better spacing
    const int screenW = 1400;
    const int screenH = 900;

    InitWindow(screenW, screenH, "Rubik's Cube - UI (T-net)");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    // Load font
    if (FileExists(FONT_PATH)) {
        gFont = LoadFontEx(FONT_PATH, 96, nullptr, 0);
        if (gFont.texture.id != 0) gFontLoaded = true;
        else gFont = GetFontDefault();
    } else {
        gFont = GetFontDefault();
    }

    Cube cube;

    // Input text for custom move sequence
    std::string inputMoves;
    bool inputActive = false;

    // Messages area
    std::string displayMessage;

    // Color input
    bool colorInputMode = false;
    ColorPicker colorPicker;
    std::string manualColors(54, 'F');

    // UI layout
    const int stickerSize = 40;
    const int stickerGap = 4;
    const int faceSpacing = 12;
    // MOVED DOWN to avoid overlap with instructions + console
    Vector2 netOrigin = {40.0f, 200.0f};

    // Buttons container
    std::vector<UIButton> buttons;
    auto addButton = [&](int x, int y, int w, int h, const std::string &label, std::function<void()> cb) {
        buttons.push_back(UIButton{ Rectangle{(float)x,(float)y,(float)w,(float)h}, label, cb });
    };

    // Add move buttons and utilities with increased spacing
    int bx = 900;  // Moved further right
    int by = 80;
    const int bw = 100, bh = 40;
    const int gap = 15;  // Increased gap between buttons

    auto pushMove = [&](char c){ 
        if (!colorInputMode) {
            cube.move(c);
            // Update manual colors if we're in color mode to stay in sync
            if (colorInputMode) {
                try { manualColors = cube.getFacelets(); } catch(...) {}
            }
        }
    };

    // R L U D F B groups with proper spacing
    addButton(bx, by, bw, bh, "R",   [&]() { pushMove('r'); });
    addButton(bx + (bw + gap), by, bw, bh, "R'", [&]() { pushMove('R'); });
    addButton(bx + 2*(bw + gap), by, bw, bh, "R2", [&]() { pushMove('r'); pushMove('r'); });

    addButton(bx, by + (bh + gap), bw, bh, "L",   [&]() { pushMove('l'); });
    addButton(bx + (bw + gap), by + (bh + gap), bw, bh, "L'", [&]() { pushMove('L'); });
    addButton(bx + 2*(bw + gap), by + (bh + gap), bw, bh, "L2", [&]() { pushMove('l'); pushMove('l'); });

    addButton(bx, by + 2*(bh + gap), bw, bh, "U",   [&]() { pushMove('u'); });
    addButton(bx + (bw + gap), by + 2*(bh + gap), bw, bh, "U'", [&]() { pushMove('U'); });
    addButton(bx + 2*(bw + gap), by + 2*(bh + gap), bw, bh, "U2", [&]() { pushMove('u'); pushMove('u'); });

    addButton(bx, by + 3*(bh + gap), bw, bh, "D",   [&]() { pushMove('d'); });
    addButton(bx + (bw + gap), by + 3*(bh + gap), bw, bh, "D'", [&]() { pushMove('D'); });
    addButton(bx + 2*(bw + gap), by + 3*(bh + gap), bw, bh, "D2", [&]() { pushMove('d'); pushMove('d'); });

    addButton(bx, by + 4*(bh + gap), bw, bh, "F",   [&]() { pushMove('f'); });
    addButton(bx + (bw + gap), by + 4*(bh + gap), bw, bh, "F'", [&]() { pushMove('F'); });
    addButton(bx + 2*(bw + gap), by + 4*(bh + gap), bw, bh, "F2", [&]() { pushMove('f'); pushMove('f'); });

    addButton(bx, by + 5*(bh + gap), bw, bh, "B",   [&]() { pushMove('b'); });
    addButton(bx + (bw + gap), by + 5*(bh + gap), bw, bh, "B'", [&]() { pushMove('B'); });
    addButton(bx + 2*(bw + gap), by + 5*(bh + gap), bw, bh, "B2", [&]() { pushMove('b'); pushMove('b'); });

    // Utilities with extra spacing between groups
    const int extraGap = 20;  // Extra space between move buttons and utility buttons
    
    addButton(bx, by + 6*(bh + gap) + extraGap, bw*3 + gap*2, bh, "Scramble", [&]() {
        if (!colorInputMode) {
            std::stringstream buffer;
            std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
            cube.move('s');
            std::cout.rdbuf(old);
            displayMessage = buffer.str();
            // Update manual colors if we're in color mode
            if (colorInputMode) {
                try { manualColors = cube.getFacelets(); } catch(...) {}
            }
        }
    });

    addButton(bx, by + 7*(bh + gap) + extraGap, bw*3 + gap*2, bh, "solution (console)", [&]() {
        if (!colorInputMode) {
            std::stringstream buffer;
            std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
            cube.move('a');
            std::cout.rdbuf(old);
            displayMessage = buffer.str();
            // Update manual colors if we're in color mode
            if (colorInputMode) {
                try { manualColors = cube.getFacelets(); } catch(...) {}
            }
        }
    });

    addButton(bx, by + 8*(bh + gap) + extraGap, bw*3 + gap*2, bh, "Reset", [&]() {
        if (!colorInputMode) {
            cube.move('w');
            displayMessage = "Cube reset to solved state";
            // Update manual colors if we're in color mode
            if (colorInputMode) {
                try { manualColors = cube.getFacelets(); } catch(...) {}
            }
        }
    });

    // Color Mode toggle with extra spacing
    addButton(bx, by + 9*(bh + gap) + extraGap, bw*3 + gap*2, bh, "Toggle Color Mode", [&]() {
        colorInputMode = !colorInputMode;
        if (colorInputMode) {
            try { 
                manualColors = cube.getFacelets(); 
                displayMessage = "Color Input Mode enabled - Click stickers to change colors";
            } catch(...) { 
                manualColors = std::string(54, 'F'); 
                displayMessage = "Color Input Mode enabled - Using default colors";
            }
        } else {
            displayMessage = "Color Input Mode disabled";
        }
    });

    // Apply moves and apply colors with ample spacing from above
    const int actionGap = 40;  // Extra space before action buttons
    
    addButton(bx, by + 10*(bh + gap) + extraGap + actionGap, bw*2 + gap, bh, "Apply Moves", [&]() {
        if (!colorInputMode && !inputMoves.empty()) {
            auto atomic = cube.convertUniversalToLocal(inputMoves);
            for (char c : atomic) cube.move(c);
            inputMoves.clear();
            displayMessage = "Moves applied";
            // Update manual colors if we're in color mode
            if (colorInputMode) {
                try { manualColors = cube.getFacelets(); } catch(...) {}
            }
        }
    });

    // Make Apply Colors button wide enough with proper spacing
    addButton(bx + (bw*2 + gap + 15), by + 10*(bh + gap) + extraGap + actionGap, bw + 48, bh, "Apply Colors", [&]() {
        if (!colorInputMode) {
            displayMessage = "Enable Color Mode first";
            return;
        }
        
        if ((int)manualColors.size() != 54) {
            displayMessage = "Invalid manual colors (must be 54 characters)";
            return;
        }
        
        // First check basic validation (color counts)
        if (!validateCubeStateBasic(manualColors)) {
            displayMessage = "ERROR: Invalid cube state! Each color must appear exactly 9 times.";
            return;
        }
        
        // Then check with Kociemba for solvability
        if (!validateCubeStateWithKociemba(manualColors)) {
            displayMessage = "WARNING: Cube state may not be solvable. Apply anyway?";
            // For now, we'll allow it but warn the user
            // In a more advanced version, you could add a confirmation dialog
        }
        
        try {
            cube.setFacelets(manualColors);
            displayMessage = "Cube state applied successfully!";
            colorInputMode = false; // Exit color mode after successful application
        } catch (...) {
            displayMessage = "Error applying colors to cube";
        }
    });

    // Input area parameters - moved further down with ample space
    const int inputBoxW = 420;
    const int inputBoxH = bh;
    int inputBoxY = by + 11*(bh + gap) + extraGap + actionGap + 30;  // Moved further down

    // Main loop
    while (!WindowShouldClose()) {
        // --- input handling ---
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 m = GetMousePosition();
            for (auto &b : buttons) {
                if (CheckCollisionPointRec(m, b.rect) && b.onClick) b.onClick();
            }

            int inputBoxX = bx;
            if (inputBoxX + inputBoxW > screenW - 20) inputBoxX = screenW - inputBoxW - 20;
            Rectangle inputRect = { (float)inputBoxX, (float)inputBoxY, (float)inputBoxW, (float)inputBoxH };
            inputActive = CheckCollisionPointRec(m, inputRect);
        }

        // keyboard input for input field
        if (inputActive) {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125)) inputMoves.push_back((char)key);
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && !inputMoves.empty()) inputMoves.pop_back();
        }

        float dt = GetFrameTime();

        BeginDrawing();
        ClearBackground(Color{33,33,33,255});

        // Title
        DrawTextUI("Rubik's Cube Simulator", 20, 18, UI_FONT_TITLE, LIGHTGRAY);

        // Instructions - moved up to make space for cube
        if (colorInputMode) {
            DrawTextUI("COLOR INPUT MODE", 20, 60, UI_FONT_LABEL, RED);
            DrawTextUI("Click a sticker to change its color, then press 'Apply Colors' to commit", 20, 84, 12, LIGHTGRAY);
            
            // Show validation status
            bool isValidBasic = validateCubeStateBasic(manualColors);
            bool isValidFull = validateCubeStateWithKociemba(manualColors);
            
            DrawTextUI("Cube State:", 20, 110, UI_FONT_LABEL, LIGHTGRAY);
            
            if (!isValidBasic) {
                DrawTextUI("INVALID - Color count wrong", 120, 110, UI_FONT_LABEL, RED);
                DrawTextUI("(Each color must appear exactly 9 times)", 20, 130, 12, RED);
            } else if (!isValidFull) {
                DrawTextUI("VALID - But may not be solvable", 120, 110, UI_FONT_LABEL, YELLOW);
                DrawTextUI("(Cube can be applied but might not have a solution)", 20, 130, 12, YELLOW);
            } else {
                DrawTextUI("VALID", 120, 110, UI_FONT_LABEL, GREEN);
                DrawTextUI("(Solvable configuration)", 20, 130, 12, GREEN);
            }
        }

        // Ensure manualColors initialized when entering color mode
        if (colorInputMode && manualColors == std::string(54, 'F')) {
            try { manualColors = cube.getFacelets(); } catch(...) { manualColors = std::string(54, 'F'); }
        }

        // Get facelets - FIX: Always use the actual cube state for display, not manualColors when not in color mode
        std::string facelets;
        try { 
            facelets = (colorInputMode ? manualColors : cube.getFacelets()); 
        } catch (...) { 
            facelets = std::string(54, 'F'); 
        }

        // FIXED: Correct T-net layout with proper face mapping
        auto drawFace = [&](int faceIndex, int originX, int originY) {
            int base = faceIndex * 9;
            for (int r = 0; r < 3; ++r) {
                for (int c = 0; c < 3; ++c) {
                    int idx = base + r*3 + c;
                    if (idx >= (int)facelets.size()) continue;
                    
                    char ch = facelets[idx];
                    Color col = colorForFaceletChar(ch);
                    int x = originX + c*(stickerSize + stickerGap);
                    int y = originY + r*(stickerSize + stickerGap);
                    Rectangle st = {(float)x, (float)y, (float)stickerSize, (float)stickerSize};

                    // Highlight on hover in color mode
                    if (colorInputMode && CheckCollisionPointRec(GetMousePosition(), st)) {
                        DrawRectangleRec(st, Fade(YELLOW, 0.14f));
                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                            colorPicker.active = true;
                            colorPicker.faceIndex = faceIndex;
                            colorPicker.row = r;
                            colorPicker.col = c;
                        }
                    }

                    DrawRectangleRec(st, col);
                    DrawRectangleLinesEx(st, 2, BLACK);
                }
            }
        };

        // FIXED: Correct T-net positions with proper face arrangement
        int ox = (int)netOrigin.x;
        int oy = (int)netOrigin.y;
        int faceW = 3*stickerSize + 2*stickerGap;
        int faceH = faceW;

        // CORRECTED T-NET LAYOUT:
        // The cube.getFacelets() returns faces in order: U, R, F, D, L, B
        // So face indices are:
        // 0: U (Up/Blue), 1: R (Right/Red), 2: F (Front/White), 
        // 3: D (Down/Green), 4: L (Left/Orange), 5: B (Back/Yellow)
        
        // Top: U (face 0)
        drawFace(0, ox + faceW + faceSpacing, oy);
        
        // Middle row: L (4), F (2), R (1), B (5)
        int midY = oy + faceH + faceSpacing;
        drawFace(4, ox, midY);                       // Left
        drawFace(2, ox + faceW + faceSpacing, midY); // Front (center)
        drawFace(1, ox + 2*(faceW + faceSpacing), midY); // Right
        drawFace(5, ox + 3*(faceW + faceSpacing), midY); // Back
        
        // Bottom: D (face 3)
        drawFace(3, ox + faceW + faceSpacing, oy + 2*(faceH + faceSpacing));

        // REMOVED: Face labels (U, L, F, R, B, D) are no longer drawn

        // Draw buttons
        for (auto &b : buttons) {
            DrawRectangleRec(b.rect, LIGHTGRAY);
            DrawRectangleLinesEx(b.rect, 1, BLACK);
            int tw = MeasureTextUI(b.label.c_str(), UI_FONT_BUTTON);
            int tx = (int)(b.rect.x + (b.rect.width - tw)/2);
            int ty = (int)(b.rect.y + (b.rect.height - UI_FONT_BUTTON)/2);
            DrawTextUI(b.label.c_str(), tx, ty, UI_FONT_BUTTON, BLACK);
        }

        // Input box - positioned with ample space from bottom
        int inputBoxX = bx;
        if (inputBoxX + inputBoxW > screenW - 20) inputBoxX = screenW - inputBoxW - 20;
        Rectangle inputRect = { (float)inputBoxX, (float)inputBoxY, (float)inputBoxW, (float)inputBoxH };
        DrawTextUI("Enter moves (e.g. R U R' U'):", inputBoxX, inputBoxY - 22, UI_FONT_LABEL, LIGHTGRAY);
        DrawRectangleRec(inputRect, (inputActive ? SKYBLUE : LIGHTGRAY));
        DrawRectangleLinesEx(inputRect, 1, BLACK);

        // Clip input text to fit inside inputRect
        std::string shownInput = inputMoves;
        int maxTextW = (int)inputRect.width - 12;
        while (!shownInput.empty() && MeasureTextUI(shownInput.c_str(), UI_FONT_INPUT) > maxTextW) {
            shownInput.erase(0, 1);
        }
        DrawTextUI(shownInput.c_str(), (int)inputRect.x + 8, (int)inputRect.y + 8, UI_FONT_INPUT, BLACK);

        // Status - positioned with space from input box
        DrawTextUI("Status:", inputBoxX, inputBoxY + 48, UI_FONT_LABEL, LIGHTGRAY);
        DrawTextUI(cube.isSolved() ? "Solved" : "Not solved", inputBoxX + 64, inputBoxY + 48, UI_FONT_LABEL, cube.isSolved() ? GREEN : RED);

        // Message panel - positioned to avoid overlap with ample space
        Rectangle msgBox = { 
            (float)ox, 
            (float)(oy + 3*(faceH + faceSpacing) + 100),  // SHIFTED DOWN to avoid overlap with cube/UI
            (float)(faceW*4 + 3*faceSpacing), 
            140.0f  // Slightly taller for better message display
        };
        DrawRectangleRec(msgBox, Fade(BLACK, 0.75f));
        DrawRectangleLinesEx(msgBox, 1, LIGHTGRAY);
        DrawTextUI("Console Output:", (int)msgBox.x + 8, (int)msgBox.y + 8, UI_FONT_MSG, LIGHTGRAY);

        if (!displayMessage.empty()) {
            int maxW = (int)msgBox.width - 20;
            std::string wrapped;
            std::string line;
            std::istringstream iss(displayMessage);
            std::string word;
            while (iss >> word) {
                std::string test = line.empty() ? word : line + " " + word;
                if (MeasureTextUI(test.c_str(), UI_FONT_MSG) <= maxW) {
                    line = test;
                } else {
                    if (!wrapped.empty()) wrapped += "\n";
                    wrapped += line;
                    line = word;
                }
            }
            if (!line.empty()) {
                if (!wrapped.empty()) wrapped += "\n";
                wrapped += line;
            }
            DrawTextUI(wrapped.c_str(), (int)msgBox.x + 10, (int)msgBox.y + 34, UI_FONT_MSG, RAYWHITE);
        }

        // Color picker modal
        if (colorPicker.active) {
            int paletteX = 600;  // Centered better in larger window
            int paletteY = 400;
            int colorSize = 46;
            
            DrawRectangle(paletteX - 16, paletteY - 48, 420, 180, Fade(DARKGRAY, 0.95f));
            DrawRectangleLines(paletteX - 16, paletteY - 48, 420, 180, LIGHTGRAY);
            DrawTextUI("Select Color:", paletteX, paletteY - 36, UI_FONT_LABEL, LIGHTGRAY);
            
            for (size_t i = 0; i < colorPicker.colors.size(); i++) {
                Rectangle pr = {(float)(paletteX + i*(colorSize + 14)), (float)paletteY, (float)colorSize, (float)colorSize};
                Color col = colorForFaceletChar(colorPicker.colors[i]);
                DrawRectangleRec(pr, col);
                DrawRectangleLinesEx(pr, 2, BLACK);
                
                const char* name = nullptr;
                switch (colorPicker.colors[i]) {
                    case 'F': name = "White"; break;
                    case 'R': name = "Red"; break;
                    case 'B': name = "Yellow"; break;
                    case 'L': name = "Orange"; break;
                    case 'U': name = "Blue"; break;
                    case 'D': name = "Green"; break;
                    default: name = "Unknown"; break;
                }
                int textW = MeasureTextUI(name, 12);
                DrawTextUI(name, (int)(pr.x + (colorSize - textW)/2), (int)(pr.y + colorSize + 6), 12, LIGHTGRAY);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), pr)) {
                    int fi = colorPicker.faceIndex;
                    if (fi >= 0 && fi < 6) {
                        int idx = fi * 9 + colorPicker.row * 3 + colorPicker.col;
                        if (idx >= 0 && idx < 54) manualColors[idx] = colorPicker.colors[i];
                    }
                    colorPicker.active = false;
                }
            }
            
            Rectangle cancel = {(float)paletteX + 320, (float)paletteY + 108, 80, 40};
            DrawRectangleRec(cancel, LIGHTGRAY);
            DrawRectangleLinesEx(cancel, 1, BLACK);
            DrawTextUI("Cancel", (int)cancel.x + 14, (int)cancel.y + 8, 14, BLACK);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), cancel))
                colorPicker.active = false;
        }

        EndDrawing();
    }

    if (gFontLoaded) UnloadFont(gFont);
    CloseWindow();
    return 0;
}
