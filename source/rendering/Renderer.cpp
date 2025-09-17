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
#if WIINGPONG_DEBUG_ENABLED
    if (!initialized || !debugVisible) return;

    // Draw debug background with enhanced styling
    GRRLIB_Rectangle(10, 400, 620, 70, 0x000000DD, true);  // Slightly more opaque
    GRRLIB_Rectangle(10, 400, 620, 70, 0xFFFFFFFF, false); // White border
    
    // Add inner shadow effect
    GRRLIB_Rectangle(11, 401, 618, 1, 0x00000066, true);   // Top inner shadow
    GRRLIB_Rectangle(11, 401, 1, 68, 0x00000066, true);    // Left inner shadow

    // Show initialization status with enhanced Wii-style indicator
    int statusSize = 24; // Larger status indicator
    if (input.isInitialized()) {
        // Green Wii-style "connected" indicator
        drawWiiButton(20, 408, statusSize, 0xE0FFE0FF, 0x40FF40FF, true, false);
        // Add a small "connected" symbol (circle with checkmark approximation)
        GRRLIB_Rectangle(28, 416, 8, 8, 0x006600FF, true); // Small circle
        GRRLIB_Rectangle(30, 418, 4, 2, 0xFFFFFFFF, true); // Checkmark horizontal
        GRRLIB_Rectangle(32, 420, 2, 2, 0xFFFFFFFF, true); // Checkmark vertical
    } else {
        // Red Wii-style "disconnected" indicator
        drawWiiButton(20, 408, statusSize, 0xFFE0E0FF, 0xFF4040FF, true, false);
        // Add a small "X" symbol
        GRRLIB_Rectangle(26, 414, 2, 12, 0xFFFFFFFF, true); // Diagonal 1
        GRRLIB_Rectangle(28, 416, 2, 8, 0xFFFFFFFF, true);  // Diagonal 1
        GRRLIB_Rectangle(30, 418, 2, 4, 0xFFFFFFFF, true);  // Diagonal 1
        GRRLIB_Rectangle(32, 414, 2, 12, 0xFFFFFFFF, true); // Diagonal 2
        GRRLIB_Rectangle(30, 416, 2, 8, 0xFFFFFFFF, true);  // Diagonal 2
        GRRLIB_Rectangle(28, 418, 2, 4, 0xFFFFFFFF, true);  // Diagonal 2
    }
    
    // Get button states
    u32 held = input.getHeldButtons();
    u32 pressed = input.getPressedButtons();
    
    // Visual representation of button states using Wii-style buttons
    int buttonY = 435;
    int buttonSize = 18; // Slightly larger for better visibility
    int spacing = 28;    // More spacing for better layout
    
    // Define Wii-style colors
    u32 wiiButtonBase = 0xF0F0F0FF;     // Light gray base (Wii remote style)
    
    // A Button indicator (Blue when pressed, like Wii remote)
    u32 colorA_base = wiiButtonBase;
    u32 colorA_active = 0x4080FFFF;  // Blue like Wii A button
    bool isAPressed = (held & WPAD_BUTTON_A) != 0;
    drawWiiButton(20, buttonY, buttonSize, colorA_base, colorA_active, isAPressed, true);
    u32 textColorA = isAPressed ? 0xFFFFFFFF : 0x000000FF;
    drawButtonSymbolA(20, buttonY, buttonSize, textColorA);
    
    // B Button indicator (Red when pressed, like Wii remote)
    u32 colorB_base = wiiButtonBase;
    u32 colorB_active = 0xFF4040FF;  // Red like Wii B button
    bool isBPressed = (held & WPAD_BUTTON_B) != 0;
    drawWiiButton(20 + spacing, buttonY, buttonSize, colorB_base, colorB_active, isBPressed, true);
    u32 textColorB = isBPressed ? 0xFFFFFFFF : 0x000000FF;
    drawButtonSymbolB(20 + spacing, buttonY, buttonSize, textColorB);
    
    // HOME Button indicator (Silver/gray when pressed)
    u32 colorHome_base = wiiButtonBase;
    u32 colorHome_active = 0x808080FF;  // Gray like Wii HOME button
    bool isHomePressed = (held & WPAD_BUTTON_HOME) != 0;
    drawWiiButton(20 + spacing * 2, buttonY, buttonSize, colorHome_base, colorHome_active, isHomePressed, true);
    u32 textColorHome = isHomePressed ? 0xFFFFFFFF : 0x000000FF;
    drawButtonSymbolHome(20 + spacing * 2, buttonY, buttonSize, textColorHome);
    
    // PLUS Button indicator (White/light when pressed)
    u32 colorPlus_base = wiiButtonBase;
    u32 colorPlus_active = 0xFFFFFFFF;  // White when active
    bool isPlusPressed = (held & WPAD_BUTTON_PLUS) != 0;
    drawWiiButton(20 + spacing * 3, buttonY, buttonSize, colorPlus_base, colorPlus_active, isPlusPressed, true);
    u32 textColorPlus = isPlusPressed ? 0x000000FF : 0x666666FF;
    drawButtonSymbolPlus(20 + spacing * 3, buttonY, buttonSize, textColorPlus);
    
    // MINUS Button indicator (White/light when pressed)
    u32 colorMinus_base = wiiButtonBase;
    u32 colorMinus_active = 0xFFFFFFFF;  // White when active
    bool isMinusPressed = (held & WPAD_BUTTON_MINUS) != 0;
    drawWiiButton(20 + spacing * 4, buttonY, buttonSize, colorMinus_base, colorMinus_active, isMinusPressed, true);
    u32 textColorMinus = isMinusPressed ? 0x000000FF : 0x666666FF;
    drawButtonSymbolMinus(20 + spacing * 4, buttonY, buttonSize, textColorMinus);
    
    // Enhanced hex value representation using bars with better styling
    int bitStartX = 200;
    int bitY1 = 410; // First row (held buttons)
    int bitY2 = 425; // Second row (pressed buttons)
    int bitWidth = 6;
    int bitHeight = 10;
    int bitSpacing = 8;
    
    // Add label background for bit display
    GRRLIB_Rectangle(bitStartX - 10, bitY1 - 5, 140, 25, 0x222222AA, true);
    GRRLIB_Rectangle(bitStartX - 10, bitY1 - 5, 140, 25, 0x666666FF, false);
    
    // Show held buttons as enhanced bit indicators (first row)
    for (int i = 0; i < 16; i++) {
        bool bitSet = (held & (1 << i)) != 0;
        u32 bitColor = bitSet ? 0xFFFFFFFF : 0x444444FF;
        u32 borderColor = bitSet ? 0xCCCCCCFF : 0x666666FF;
        
        // Draw bit indicator with border
        GRRLIB_Rectangle(bitStartX + i * bitSpacing, bitY1, bitWidth, bitHeight, bitColor, true);
        GRRLIB_Rectangle(bitStartX + i * bitSpacing, bitY1, bitWidth, bitHeight, borderColor, false);
        
        // Add small highlight for active bits
        if (bitSet) {
            GRRLIB_Rectangle(bitStartX + i * bitSpacing + 1, bitY1 + 1, bitWidth - 2, 2, 0xFFFFFF88, true);
        }
    }
    
    // Show pressed buttons as enhanced bit indicators (second row)
    for (int i = 0; i < 16; i++) {
        bool bitSet = (pressed & (1 << i)) != 0;
        u32 bitColor = bitSet ? 0xFFFF00FF : 0x333333FF; // Yellow for pressed, darker for inactive
        u32 borderColor = bitSet ? 0xCCCC00FF : 0x555555FF;
        
        // Draw bit indicator with border
        GRRLIB_Rectangle(bitStartX + i * bitSpacing, bitY2, bitWidth, bitHeight, bitColor, true);
        GRRLIB_Rectangle(bitStartX + i * bitSpacing, bitY2, bitWidth, bitHeight, borderColor, false);
        
        // Add small highlight for active bits
        if (bitSet) {
            GRRLIB_Rectangle(bitStartX + i * bitSpacing + 1, bitY2 + 1, bitWidth - 2, 2, 0xFFFF8888, true);
        }
    }
    
    // Enhanced button 1 indicator (toggle debug) - Wii-style
    bool isButton1Pressed = (held & WPAD_BUTTON_1) != 0;
    u32 button1_base = 0xF0F0F0FF;
    u32 button1_active = 0x80FF80FF; // Green when pressed
    drawWiiButton(590, 410, 12, button1_base, button1_active, isButton1Pressed, false);
    
    // Add small "1" indicator inside
    if (isButton1Pressed) {
        GRRLIB_Rectangle(593, 413, 2, 6, 0x000000FF, true); // Vertical line for "1"
        GRRLIB_Rectangle(592, 413, 2, 2, 0x000000FF, true); // Top angled part
    } else {
        GRRLIB_Rectangle(593, 413, 2, 6, 0x666666FF, true); // Vertical line for "1"
        GRRLIB_Rectangle(592, 413, 2, 2, 0x666666FF, true); // Top angled part
    }
#endif
}

void Renderer::renderDebugToggleProgress(const InputManager& input) {
#if WIINGPONG_DEBUG_ENABLED && (WIINGPONG_DEBUG_TOGGLE_METHOD == 1 || WIINGPONG_DEBUG_TOGGLE_METHOD == 2)
    if (!initialized) return;

    float progress = input.getDebugToggleProgress();
    
    if (progress > 0.0f) {
        // Draw progress bar background
        int barWidth = 200;
        int barHeight = 20;
        int barX = (640 - barWidth) / 2; // Center horizontally
        int barY = 100; // Near top of screen
        
        // Background
        GRRLIB_Rectangle(barX, barY, barWidth, barHeight, 0x000000CC, true);
        GRRLIB_Rectangle(barX, barY, barWidth, barHeight, 0xFFFFFFFF, false);
        
        // Progress fill
        int fillWidth = (int)(barWidth * progress);
        u32 fillColor = 0x00FF00FF; // Green
        if (progress > 0.8f) {
            fillColor = 0xFFFF00FF; // Yellow when almost ready
        }
        GRRLIB_Rectangle(barX + 2, barY + 2, fillWidth - 4, barHeight - 4, fillColor, true);
        
        #if WIINGPONG_DEBUG_TOGGLE_METHOD == 1
        // A+B indicators on the sides with Wii-style buttons
        bool isAHeld = (input.getHeldButtons() & WPAD_BUTTON_A) != 0;
        bool isBHeld = (input.getHeldButtons() & WPAD_BUTTON_B) != 0;
        
        // A button indicator (left) - Blue Wii-style
        u32 colorA_base = 0xF0F0F0FF;
        u32 colorA_active = 0x4080FFFF;
        drawWiiButton(barX - 30, barY, 20, colorA_base, colorA_active, isAHeld, true);
        u32 textColorA = isAHeld ? 0xFFFFFFFF : 0x000000FF;
        drawButtonSymbolA(barX - 30, barY, 20, textColorA);
        
        // B button indicator (right) - Red Wii-style
        u32 colorB_base = 0xF0F0F0FF;
        u32 colorB_active = 0xFF4040FF;
        drawWiiButton(barX + barWidth + 10, barY, 20, colorB_base, colorB_active, isBHeld, true);
        u32 textColorB = isBHeld ? 0xFFFFFFFF : 0x000000FF;
        drawButtonSymbolB(barX + barWidth + 10, barY, 20, textColorB);
        
        #elif WIINGPONG_DEBUG_TOGGLE_METHOD == 2
        // PLUS+MINUS indicators on the sides with Wii-style buttons
        bool isPlusHeld = (input.getHeldButtons() & WPAD_BUTTON_PLUS) != 0;
        bool isMinusHeld = (input.getHeldButtons() & WPAD_BUTTON_MINUS) != 0;
        
        // PLUS button indicator (left) - White Wii-style
        u32 colorPlus_base = 0xF0F0F0FF;
        u32 colorPlus_active = 0xFFFFFFFF;
        drawWiiButton(barX - 30, barY, 20, colorPlus_base, colorPlus_active, isPlusHeld, true);
        u32 textColorPlus = isPlusHeld ? 0x000000FF : 0x666666FF;
        drawButtonSymbolPlus(barX - 30, barY, 20, textColorPlus);
        
        // MINUS button indicator (right) - White Wii-style
        u32 colorMinus_base = 0xF0F0F0FF;
        u32 colorMinus_active = 0xFFFFFFFF;
        drawWiiButton(barX + barWidth + 10, barY, 20, colorMinus_base, colorMinus_active, isMinusHeld, true);
        u32 textColorMinus = isMinusHeld ? 0x000000FF : 0x666666FF;
        drawButtonSymbolMinus(barX + barWidth + 10, barY, 20, textColorMinus);
        #endif
    }
#endif
}

// =============================================================================
// WII-STYLE BUTTON RENDERING FUNCTIONS
// =============================================================================

void Renderer::drawWiiButton(int x, int y, int size, u32 baseColor, u32 activeColor, bool isPressed, bool hasSymbol) {
    if (!initialized) return;
    
    // Calculate colors for 3D effect
    u32 buttonColor = isPressed ? activeColor : baseColor;
    u32 shadowColor = 0x000000AA;  // Semi-transparent black shadow
    u32 highlightColor = 0xFFFFFF88; // Semi-transparent white highlight
    u32 borderColor = 0x999999FF;   // Gray border
    
    // Draw shadow (offset down and right)
    GRRLIB_Rectangle(x + 2, y + 2, size, size, shadowColor, true);
    
    // Draw main button body with gradient effect
    if (isPressed) {
        // Pressed state: darker colors, inverted gradient
        GRRLIB_Rectangle(x, y, size, size, buttonColor, true);
        // Dark top edge for pressed effect
        GRRLIB_Rectangle(x, y, size, 2, 0x000000AA, true);
        GRRLIB_Rectangle(x, y, 2, size, 0x000000AA, true);
    } else {
        // Normal state: lighter colors with highlight
        GRRLIB_Rectangle(x, y, size, size, buttonColor, true);
        // Light highlight on top and left
        GRRLIB_Rectangle(x, y, size, 2, highlightColor, true);
        GRRLIB_Rectangle(x, y, 2, size, highlightColor, true);
        // Subtle shadow on bottom and right
        GRRLIB_Rectangle(x, y + size - 2, size, 2, 0x00000044, true);
        GRRLIB_Rectangle(x + size - 2, y, 2, size, 0x00000044, true);
    }
    
    // Draw border
    GRRLIB_Rectangle(x, y, size, size, borderColor, false);
    
    // Add inner border for more definition
    if (!isPressed) {
        GRRLIB_Rectangle(x + 1, y + 1, size - 2, size - 2, 0xFFFFFF66, false);
    }
}

void Renderer::drawButtonSymbolA(int x, int y, int size, u32 textColor) {
    if (!initialized) return;
    
    // Draw letter "A" using rectangles
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int letterSize = size / 3;
    
    // Vertical lines
    GRRLIB_Rectangle(centerX - letterSize/2, centerY - letterSize/2, 2, letterSize, textColor, true);
    GRRLIB_Rectangle(centerX + letterSize/2 - 2, centerY - letterSize/2, 2, letterSize, textColor, true);
    
    // Top horizontal line
    GRRLIB_Rectangle(centerX - letterSize/2, centerY - letterSize/2, letterSize, 2, textColor, true);
    
    // Middle horizontal line
    GRRLIB_Rectangle(centerX - letterSize/2 + 2, centerY - 2, letterSize - 4, 2, textColor, true);
}

void Renderer::drawButtonSymbolB(int x, int y, int size, u32 textColor) {
    if (!initialized) return;
    
    // Draw letter "B" using rectangles
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int letterSize = size / 3;
    
    // Vertical line
    GRRLIB_Rectangle(centerX - letterSize/2, centerY - letterSize/2, 2, letterSize, textColor, true);
    
    // Top horizontal line
    GRRLIB_Rectangle(centerX - letterSize/2, centerY - letterSize/2, letterSize - 2, 2, textColor, true);
    
    // Middle horizontal line
    GRRLIB_Rectangle(centerX - letterSize/2, centerY - 1, letterSize - 4, 2, textColor, true);
    
    // Bottom horizontal line
    GRRLIB_Rectangle(centerX - letterSize/2, centerY + letterSize/2 - 2, letterSize - 2, 2, textColor, true);
    
    // Right vertical segments
    GRRLIB_Rectangle(centerX + letterSize/2 - 4, centerY - letterSize/2 + 2, 2, letterSize/2 - 3, textColor, true);
    GRRLIB_Rectangle(centerX + letterSize/2 - 4, centerY + 1, 2, letterSize/2 - 3, textColor, true);
}

void Renderer::drawButtonSymbolHome(int x, int y, int size, u32 textColor) {
    if (!initialized) return;
    
    // Draw house symbol for HOME button
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int houseSize = size / 3;
    
    // House base (rectangle)
    GRRLIB_Rectangle(centerX - houseSize/2, centerY, houseSize, houseSize/2, textColor, false);
    
    // House roof (triangle approximation with rectangles)
    for (int i = 0; i < houseSize/2; i++) {
        int roofWidth = (houseSize/2 - i) * 2;
        GRRLIB_Rectangle(centerX - roofWidth/2, centerY - houseSize/2 + i, roofWidth, 1, textColor, true);
    }
    
    // Door
    int doorWidth = houseSize / 4;
    int doorHeight = houseSize / 3;
    GRRLIB_Rectangle(centerX - doorWidth/2, centerY + houseSize/2 - doorHeight, doorWidth, doorHeight, textColor, true);
}

void Renderer::drawButtonSymbolPlus(int x, int y, int size, u32 textColor) {
    if (!initialized) return;
    
    // Draw "+" symbol
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int crossSize = size / 3;
    int thickness = 3;
    
    // Horizontal line
    GRRLIB_Rectangle(centerX - crossSize/2, centerY - thickness/2, crossSize, thickness, textColor, true);
    
    // Vertical line
    GRRLIB_Rectangle(centerX - thickness/2, centerY - crossSize/2, thickness, crossSize, textColor, true);
}

void Renderer::drawButtonSymbolMinus(int x, int y, int size, u32 textColor) {
    if (!initialized) return;
    
    // Draw "-" symbol
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int lineSize = size / 3;
    int thickness = 3;
    
    // Horizontal line
    GRRLIB_Rectangle(centerX - lineSize/2, centerY - thickness/2, lineSize, thickness, textColor, true);
}