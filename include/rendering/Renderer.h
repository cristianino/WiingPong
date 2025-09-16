#pragma once

#include <grrlib.h>
#include <gctypes.h>
#include "../physics/PhysicsEngine.h"
#include "../physics/Components.h"
#include "../input/InputManager.h"

// Renderer class for drawing game elements using GRRLIB
class Renderer {
public:
    Renderer();
    ~Renderer();

    void init();
    void render(const PhysicsEngine& physics);  // Draw from physics components
    void renderDebugInfo(const InputManager& input);  // Debug overlay
    void renderDebugToggleProgress(const InputManager& input); // A+B progress indicator
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
};