// source/rendering/Renderer.cpp
#include "rendering/Renderer.h"
#include <gccore.h>  // For VIDEO_Init if needed
#include <stdio.h>   // For snprintf
#include <wiiuse/wpad.h>  // For WPAD button constants

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

void Renderer::drawText(const char* text, int x, int y, u32 color) {
    // Simple text rendering using GRRLIB_Printf
    GRRLIB_Printf(x, y, font, color, 1, text);
}

void Renderer::renderDebugInfo(const InputManager& input) {
    if (!initialized) return;

    // Draw debug background
    GRRLIB_Rectangle(10, 400, 620, 70, 0x000000CC, true);
    GRRLIB_Rectangle(10, 400, 620, 70, 0xFFFFFFFF, false);

    // Debug text color
    u32 textColor = 0xFFFFFFFF;
    
    // Show connection status (simplified - just show if initialized)
    if (input.isInitialized()) {
        GRRLIB_Printf(15, 405, font, 0x00FF00FF, 1, "WPAD: INITIALIZED");
    } else {
        GRRLIB_Printf(15, 405, font, 0xFF0000FF, 1, "WPAD: NOT INIT");
    }
    
    // Show button states
    u32 held = input.getHeldButtons();
    u32 pressed = input.getPressedButtons();
    
    GRRLIB_Printf(15, 420, font, textColor, 1, "Held: 0x%08X", held);
    GRRLIB_Printf(15, 435, font, textColor, 1, "Pressed: 0x%08X", pressed);
    
    // Show specific button states with expected values
    char buttonStatus[128];
    snprintf(buttonStatus, sizeof(buttonStatus), "A:%d(0x0008) B:%d(0x0004) HOME:%d(0x0080) +:%d(0x0010) -:%d(0x1000)", 
             (held & WPAD_BUTTON_A) ? 1 : 0,
             (held & WPAD_BUTTON_B) ? 1 : 0, 
             (held & WPAD_BUTTON_HOME) ? 1 : 0,
             (held & WPAD_BUTTON_PLUS) ? 1 : 0,
             (held & WPAD_BUTTON_MINUS) ? 1 : 0);
    GRRLIB_Printf(15, 450, font, textColor, 1, buttonStatus);
}