#pragma once

#include <grrlib.h>
#include <gctypes.h>
#include "../physics/PhysicsEngine.h"
#include "../physics/Components.h"
#include "../input/InputManager.h"
#include "../config.h"

// Renderer class for drawing game elements using GRRLIB
class Renderer {
public:
    Renderer();
    ~Renderer();

    void init();
    void render(const PhysicsEngine& physics);  // Draw from physics components
    void renderDebugInfo(const InputManager& input);  // Debug overlay
    void renderDebugToggleProgress(const InputManager& input); // Debug toggle progress indicator (A+B or PLUS+MINUS)
    void setDebugVisible(bool visible) { debugVisible = visible; }
    bool isDebugVisible() const { return debugVisible; }

private:
    bool initialized;
    bool debugVisible;  // Control debug visibility
    GRRLIB_texImg* font;  // For future text; placeholder for now

    void drawPaddle(const Position& pos, const Size& size, u32 color);
    void drawBall(const Position& pos, const Size& size, u32 color);
    void drawCourt();
    void drawScores(const PhysicsEngine& physics);
    void drawText(const char* text, int x, int y, u32 color);
    
    // Wii-style button rendering functions
    void drawWiiButton(int x, int y, int size, u32 baseColor, u32 activeColor, bool isPressed, bool hasSymbol = false);
    void drawButtonSymbolA(int x, int y, int size, u32 textColor);
    void drawButtonSymbolB(int x, int y, int size, u32 textColor);
    void drawButtonSymbolHome(int x, int y, int size, u32 textColor);
    void drawButtonSymbolPlus(int x, int y, int size, u32 textColor);
    void drawButtonSymbolMinus(int x, int y, int size, u32 textColor);
    void drawButtonSymbolUp(int x, int y, int size, u32 textColor);
    void drawButtonSymbolDown(int x, int y, int size, u32 textColor);
    void drawButtonSymbolLeft(int x, int y, int size, u32 textColor);
    void drawButtonSymbolRight(int x, int y, int size, u32 textColor);
    void drawButtonSymbol1(int x, int y, int size, u32 textColor);
    void drawButtonSymbol2(int x, int y, int size, u32 textColor);
    void drawDPadCross(int x, int y, int size, bool upPressed, bool downPressed, bool leftPressed, bool rightPressed);
};