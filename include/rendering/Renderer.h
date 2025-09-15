#pragma once

#include <grrlib.h>
#include <gctypes.h>
#include "../physics/PhysicsEngine.h"
#include "../physics/Components.h"

// Renderer class for drawing game elements using GRRLIB
class Renderer {
public:
    Renderer();
    ~Renderer();

    void init();
    void render(const PhysicsEngine& physics);  // Draw from physics components

private:
    bool initialized;
    GRRLIB_texImg* font;  // For future text; placeholder for now

    void drawPaddle(const Position& pos, const Size& size, u32 color);
    void drawBall(const Position& pos, const Size& size, u32 color);
    void drawCourt();
    void drawScores(const PhysicsEngine& physics);
};