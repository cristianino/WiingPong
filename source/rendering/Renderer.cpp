// source/rendering/Renderer.cpp
#include "rendering/Renderer.h"
#include <gccore.h>  // For VIDEO_Init if needed
#include <stdio.h>   // For snprintf
#include <wiiuse/wpad.h>  // For WPAD button constants

Renderer::Renderer() : initialized(false), debugVisible(false), font(nullptr) {
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
    // Text function not available - using visual indicators instead
    // This function is kept for interface compatibility but does nothing
}

void Renderer::renderDebugInfo(const InputManager& input) {
    if (!initialized || !debugVisible) return;

    // Draw debug background
    GRRLIB_Rectangle(10, 400, 620, 70, 0x000000CC, true);
    GRRLIB_Rectangle(10, 400, 620, 70, 0xFFFFFFFF, false);

    // Show initialization status with colored indicator
    if (input.isInitialized()) {
        GRRLIB_Rectangle(20, 410, 20, 20, 0x00FF00FF, true); // Green square = INITIALIZED
    } else {
        GRRLIB_Rectangle(20, 410, 20, 20, 0xFF0000FF, true); // Red square = NOT INIT
    }
    
    // Get button states
    u32 held = input.getHeldButtons();
    u32 pressed = input.getPressedButtons();
    
    // Visual representation of button states using colored rectangles
    int buttonY = 435;
    int buttonSize = 15;
    int spacing = 25;
    
    // A Button indicator
    u32 colorA = (held & WPAD_BUTTON_A) ? 0x00FF00FF : 0x333333FF;
    GRRLIB_Rectangle(20, buttonY, buttonSize, buttonSize, colorA, true);
    
    // B Button indicator  
    u32 colorB = (held & WPAD_BUTTON_B) ? 0x0000FFFF : 0x333333FF;
    GRRLIB_Rectangle(20 + spacing, buttonY, buttonSize, buttonSize, colorB, true);
    
    // HOME Button indicator
    u32 colorHome = (held & WPAD_BUTTON_HOME) ? 0xFF0000FF : 0x333333FF;
    GRRLIB_Rectangle(20 + spacing * 2, buttonY, buttonSize, buttonSize, colorHome, true);
    
    // PLUS Button indicator
    u32 colorPlus = (held & WPAD_BUTTON_PLUS) ? 0xFFFF00FF : 0x333333FF;
    GRRLIB_Rectangle(20 + spacing * 3, buttonY, buttonSize, buttonSize, colorPlus, true);
    
    // MINUS Button indicator
    u32 colorMinus = (held & WPAD_BUTTON_MINUS) ? 0xFF00FFFF : 0x333333FF;
    GRRLIB_Rectangle(20 + spacing * 4, buttonY, buttonSize, buttonSize, colorMinus, true);
    
    // Hex value representation using bars (each bit as a small rectangle)
    // Show held buttons as a series of small indicators
    for (int i = 0; i < 16; i++) {
        u32 bitColor = (held & (1 << i)) ? 0xFFFFFFFF : 0x111111FF;
        GRRLIB_Rectangle(200 + i * 8, 410, 6, 10, bitColor, true);
    }
    
    // Show pressed buttons as a series of small indicators (second row)
    for (int i = 0; i < 16; i++) {
        u32 bitColor = (pressed & (1 << i)) ? 0xFFFFFFFF : 0x111111FF;
        GRRLIB_Rectangle(200 + i * 8, 425, 6, 10, bitColor, true);
    }
    
    // Add a small indicator for button 1 (toggle debug) - small white dot in corner
    u32 button1Color = (held & WPAD_BUTTON_1) ? 0xFFFFFFFF : 0x666666FF;
    GRRLIB_Rectangle(590, 410, 8, 8, button1Color, true);
    // Small border around the toggle indicator
    GRRLIB_Rectangle(590, 410, 8, 8, 0xFFFFFFFF, false);
}