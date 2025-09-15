// source/rendering/Renderer.cpp
#include "rendering/Renderer.h"
#include <gccore.h>  // For VIDEO_Init if needed

Renderer::Renderer() : initialized(false), font(nullptr) {
}

Renderer::~Renderer() {
    if (initialized) {
        GRRLIB_Exit();
    }
}

void Renderer::init() {
    if (initialized) return;

    VIDEO_Init();
    GRRLIB_Init();
    initialized = true;
}

void Renderer::render(const PhysicsEngine& physics) {
    if (!initialized) return;

    // Clear screen (black background)
    GRRLIB_FillScreen(0x000000FF);

    // Draw court line
    drawCourt();

    // Draw entities
    // Player paddle
    drawPaddle(physics.positions[PLAYER_PADDLE], physics.sizes[PLAYER_PADDLE], 0xFFFFFFFF);
    // CPU paddle
    drawPaddle(physics.positions[CPU_PADDLE], physics.sizes[CPU_PADDLE], 0xFFFFFFFF);
    // Ball
    drawBall(physics.positions[BALL], physics.sizes[BALL], 0xFFFFFFFF);

    // Draw scores (placeholder bars)
    drawScores(physics);
}

void Renderer::drawPaddle(const Position& pos, const Size& size, u32 color) {
    GRRLIB_Rectangle(pos.x, pos.y, size.width, size.height, color, true);
}

void Renderer::drawBall(const Position& pos, const Size& size, u32 color) {
    GRRLIB_Rectangle(pos.x, pos.y, size.width, size.height, color, true);
}

void Renderer::drawCourt() {
    GRRLIB_Line(320, 0, 320, 480, 0xFFFFFFFF);
}

void Renderer::drawScores(const PhysicsEngine& physics) {
    // Player score bars (green)
    for (int i = 0; i < physics.playerScore; ++i) {
        GRRLIB_Rectangle(200 + i * 15, 40, 10, 20, 0x00FF00FF, true);
    }
    // CPU score bars (red)
    for (int i = 0; i < physics.cpuScore; ++i) {
        GRRLIB_Rectangle(400 + i * 15, 40, 10, 20, 0xFF0000FF, true);
    }
}