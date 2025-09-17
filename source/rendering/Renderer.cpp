// source/rendering/Renderer.cpp
#include "rendering/Renderer.h"
#include <gccore.h>  // For VIDEO_Init if needed
#include <stdio.h>   // For snprintf
#include <math.h>    // For sqrt, cos, sin
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
    GRRLIB_Rectangle(10, 400, 700, 70, 0x000000CC, true);  // Wider background for more buttons
    GRRLIB_Rectangle(10, 400, 700, 70, 0xFFFFFFFF, false); // White border
    
    // Add inner shadow effect
    GRRLIB_Rectangle(11, 401, 698, 1, 0x00000066, true);   // Top inner shadow
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
    
    // D-PAD (Cruz de flechas) - Grouped together with cross design
    int dpadX = 20 + spacing * 5 + 10; // Extra spacing for visual separation
    int dpadY = buttonY;
    int dpadSize = buttonSize + 8; // Slightly larger for the D-pad
    bool isUpPressed = (held & WPAD_BUTTON_UP) != 0;
    bool isDownPressed = (held & WPAD_BUTTON_DOWN) != 0;
    bool isLeftPressed = (held & WPAD_BUTTON_LEFT) != 0;
    bool isRightPressed = (held & WPAD_BUTTON_RIGHT) != 0;
    
    // Draw D-pad as a cross with individual button states
    drawDPadCross(dpadX, dpadY, dpadSize, isUpPressed, isDownPressed, isLeftPressed, isRightPressed);
    
    // Button 1 indicator (Number 1)
    int btn1X = 20 + spacing * 6 + 40; // After D-pad
    bool isButton1Pressed = (held & WPAD_BUTTON_1) != 0;
    u32 button1_base = 0xF0F0F0FF;
    u32 button1_active = 0x80FF80FF; // Green when pressed
    drawWiiButton(btn1X, buttonY, buttonSize, button1_base, button1_active, isButton1Pressed, true);
    u32 textColor1 = isButton1Pressed ? 0xFFFFFFFF : 0x000000FF;
    drawButtonSymbol1(btn1X, buttonY, buttonSize, textColor1);
    
    // Button 2 indicator (Number 2)
    int btn2X = btn1X + spacing;
    bool isButton2Pressed = (held & WPAD_BUTTON_2) != 0;
    u32 button2_base = 0xF0F0F0FF;
    u32 button2_active = 0xFF8080FF; // Light red when pressed
    drawWiiButton(btn2X, buttonY, buttonSize, button2_base, button2_active, isButton2Pressed, true);
    u32 textColor2 = isButton2Pressed ? 0xFFFFFFFF : 0x000000FF;
    drawButtonSymbol2(btn2X, buttonY, buttonSize, textColor2);
    
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

void Renderer::drawButtonSymbolUp(int x, int y, int size, u32 textColor) {
    if (!initialized) return;
    
    // Draw up arrow (triangle pointing up)
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int arrowSize = size / 3;
    
    // Simple triangle using rectangles
    for (int i = 0; i < arrowSize/2; i++) {
        int lineWidth = (i + 1) * 2;
        GRRLIB_Rectangle(centerX - i, centerY - arrowSize/2 + i, lineWidth, 1, textColor, true);
    }
    
    // Arrow stem
    GRRLIB_Rectangle(centerX - 1, centerY, 2, arrowSize/2, textColor, true);
}

void Renderer::drawButtonSymbolDown(int x, int y, int size, u32 textColor) {
    if (!initialized) return;
    
    // Draw down arrow (triangle pointing down)
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int arrowSize = size / 3;
    
    // Arrow stem
    GRRLIB_Rectangle(centerX - 1, centerY - arrowSize/2, 2, arrowSize/2, textColor, true);
    
    // Simple triangle using rectangles
    for (int i = 0; i < arrowSize/2; i++) {
        int lineWidth = (arrowSize/2 - i) * 2;
        GRRLIB_Rectangle(centerX - (arrowSize/2 - i - 1), centerY + i, lineWidth, 1, textColor, true);
    }
}

void Renderer::drawButtonSymbolLeft(int x, int y, int size, u32 textColor) {
    if (!initialized) return;
    
    // Draw left arrow (triangle pointing left)
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int arrowSize = size / 3;
    
    // Simple triangle using rectangles
    for (int i = 0; i < arrowSize/2; i++) {
        int lineHeight = (i + 1) * 2;
        GRRLIB_Rectangle(centerX - arrowSize/2 + i, centerY - i, 1, lineHeight, textColor, true);
    }
    
    // Arrow stem
    GRRLIB_Rectangle(centerX, centerY - 1, arrowSize/2, 2, textColor, true);
}

void Renderer::drawButtonSymbolRight(int x, int y, int size, u32 textColor) {
    if (!initialized) return;
    
    // Draw right arrow (triangle pointing right)
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int arrowSize = size / 3;
    
    // Arrow stem
    GRRLIB_Rectangle(centerX - arrowSize/2, centerY - 1, arrowSize/2, 2, textColor, true);
    
    // Simple triangle using rectangles
    for (int i = 0; i < arrowSize/2; i++) {
        int lineHeight = (arrowSize/2 - i) * 2;
        GRRLIB_Rectangle(centerX + i, centerY - (arrowSize/2 - i - 1), 1, lineHeight, textColor, true);
    }
}

void Renderer::drawButtonSymbol1(int x, int y, int size, u32 textColor) {
    if (!initialized) return;
    
    // Draw number "1"
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int numberSize = size / 3;
    
    // Vertical line for "1"
    GRRLIB_Rectangle(centerX, centerY - numberSize/2, 2, numberSize, textColor, true);
    
    // Top angled part
    GRRLIB_Rectangle(centerX - 2, centerY - numberSize/2, 2, 2, textColor, true);
    
    // Bottom base
    GRRLIB_Rectangle(centerX - 3, centerY + numberSize/2 - 2, 6, 2, textColor, true);
}

void Renderer::drawButtonSymbol2(int x, int y, int size, u32 textColor) {
    if (!initialized) return;
    
    // Draw number "2"
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int numberSize = size / 3;
    
    // Top horizontal line
    GRRLIB_Rectangle(centerX - numberSize/2, centerY - numberSize/2, numberSize, 2, textColor, true);
    
    // Top right vertical
    GRRLIB_Rectangle(centerX + numberSize/2 - 2, centerY - numberSize/2, 2, numberSize/2 - 1, textColor, true);
    
    // Middle horizontal line
    GRRLIB_Rectangle(centerX - numberSize/2, centerY - 1, numberSize, 2, textColor, true);
    
    // Bottom left vertical
    GRRLIB_Rectangle(centerX - numberSize/2, centerY + 1, 2, numberSize/2 - 1, textColor, true);
    
    // Bottom horizontal line
    GRRLIB_Rectangle(centerX - numberSize/2, centerY + numberSize/2 - 2, numberSize, 2, textColor, true);
}

void Renderer::drawDPadCross(int x, int y, int size, bool upPressed, bool downPressed, bool leftPressed, bool rightPressed) {
    if (!initialized) return;
    
    // Define colors for D-pad sections
    u32 baseColor = 0xF0F0F0FF;
    u32 activeColor = 0xFFD700FF; // Gold color when pressed
    u32 borderColor = 0x999999FF;
    
    int crossThickness = size / 3;
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    
    // Draw D-pad cross background
    // Vertical bar
    GRRLIB_Rectangle(centerX - crossThickness/2, y, crossThickness, size, baseColor, true);
    // Horizontal bar
    GRRLIB_Rectangle(x, centerY - crossThickness/2, size, crossThickness, baseColor, true);
    
    // Draw individual button sections with state-dependent colors
    // UP section
    u32 upColor = upPressed ? activeColor : baseColor;
    GRRLIB_Rectangle(centerX - crossThickness/2, y, crossThickness, size/2 - crossThickness/4, upColor, true);
    if (upPressed) drawButtonSymbolUp(centerX - crossThickness/2, y, crossThickness, 0x000000FF);
    
    // DOWN section
    u32 downColor = downPressed ? activeColor : baseColor;
    GRRLIB_Rectangle(centerX - crossThickness/2, centerY + crossThickness/4, crossThickness, size/2 - crossThickness/4, downColor, true);
    if (downPressed) drawButtonSymbolDown(centerX - crossThickness/2, centerY + crossThickness/4, crossThickness, 0x000000FF);
    
    // LEFT section
    u32 leftColor = leftPressed ? activeColor : baseColor;
    GRRLIB_Rectangle(x, centerY - crossThickness/2, size/2 - crossThickness/4, crossThickness, leftColor, true);
    if (leftPressed) drawButtonSymbolLeft(x, centerY - crossThickness/2, crossThickness, 0x000000FF);
    
    // RIGHT section
    u32 rightColor = rightPressed ? activeColor : baseColor;
    GRRLIB_Rectangle(centerX + crossThickness/4, centerY - crossThickness/2, size/2 - crossThickness/4, crossThickness, rightColor, true);
    if (rightPressed) drawButtonSymbolRight(centerX + crossThickness/4, centerY - crossThickness/2, crossThickness, 0x000000FF);
    
    // Draw borders for the cross
    // Vertical borders
    GRRLIB_Rectangle(centerX - crossThickness/2, y, crossThickness, size, borderColor, false);
    // Horizontal borders
    GRRLIB_Rectangle(x, centerY - crossThickness/2, size, crossThickness, borderColor, false);
    
    // Draw center junction
    GRRLIB_Rectangle(centerX - crossThickness/2, centerY - crossThickness/2, crossThickness, crossThickness, 0xCCCCCCFF, true);
    GRRLIB_Rectangle(centerX - crossThickness/2, centerY - crossThickness/2, crossThickness, crossThickness, borderColor, false);
}

// =============================================================================
// SENSOR DEBUG RENDERING FUNCTIONS
// =============================================================================

void Renderer::renderDebugSensorInfo(const InputManager& input) {
#if WIINGPONG_DEBUG_ENABLED
    if (!initialized || !debugVisible || !input.hasSensorData()) return;

    // Position sensor panel on the right side of the screen
    int panelX = 450;
    int panelY = 50;
    int panelWidth = 180;
    int panelHeight = 320;
    
    // Main sensor panel background
    drawSensorBackground(panelX, panelY, panelWidth, panelHeight, "SENSOR DATA");
    
    // Get sensor data
    const vec3w_t& accel = input.getAccelerometerData();
    const orient_t& orient = input.getOrientationData();
    const gforce_t& gforce = input.getGForceData();
    const ir_t& ir = input.getIRData();
    
    int sectionHeight = 70;
    int currentY = panelY + 25;
    
    // 1. Accelerometer section
    drawSensorBackground(panelX + 5, currentY, panelWidth - 10, sectionHeight, "ACCEL");
    drawAccelerometerGraph(panelX + 10, currentY + 15, panelWidth - 20, sectionHeight - 20, accel);
    currentY += sectionHeight + 5;
    
    // 2. Orientation section
    drawSensorBackground(panelX + 5, currentY, panelWidth - 10, sectionHeight, "ORIENT");
    drawOrientationIndicator(panelX + 10, currentY + 15, sectionHeight - 20, orient);
    currentY += sectionHeight + 5;
    
    // 3. G-Force section
    drawSensorBackground(panelX + 5, currentY, panelWidth - 10, sectionHeight, "G-FORCE");
    drawGForceIndicator(panelX + 10, currentY + 15, sectionHeight - 20, gforce);
    currentY += sectionHeight + 5;
    
    // 4. IR section
    drawSensorBackground(panelX + 5, currentY, panelWidth - 10, sectionHeight, "IR/SENSOR");
    drawIRDots(panelX + 10, currentY + 15, panelWidth - 20, sectionHeight - 20, ir);
#endif
}

void Renderer::drawSensorBackground(int x, int y, int width, int height, const char* title) {
    if (!initialized) return;
    
    // Background with transparency
    GRRLIB_Rectangle(x, y, width, height, 0x000000CC, true);
    GRRLIB_Rectangle(x, y, width, height, 0x66CCFFFF, false);
    
    // Title bar
    GRRLIB_Rectangle(x, y, width, 15, 0x0066CCAA, true);
    GRRLIB_Rectangle(x, y, width, 15, 0x66CCFFFF, false);
    
    // Simple title text (using rectangles for now)
    // TODO: Replace with actual text rendering when font is available
}

void Renderer::drawAccelerometerGraph(int x, int y, int width, int height, const vec3w_t& accel) {
    if (!initialized) return;
    
    // Calculate center and scale values for visualization
    int centerX = x + width / 2;
    int centerY = y + height / 2;
    int maxBarLength = width / 3;
    
    // Scale accelerometer values (typically 0-1000 range)
    float scaleX = (float)(accel.x - 512) / 512.0f; // Center around 512
    float scaleY = (float)(accel.y - 512) / 512.0f;
    float scaleZ = (float)(accel.z - 512) / 512.0f;
    
    // Clamp values
    if (scaleX > 1.0f) scaleX = 1.0f;
    if (scaleX < -1.0f) scaleX = -1.0f;
    if (scaleY > 1.0f) scaleY = 1.0f;
    if (scaleY < -1.0f) scaleY = -1.0f;
    if (scaleZ > 1.0f) scaleZ = 1.0f;
    if (scaleZ < -1.0f) scaleZ = -1.0f;
    
    // Draw X axis (red)
    int barX = (int)(scaleX * maxBarLength);
    if (barX > 0) {
        GRRLIB_Rectangle(centerX, centerY - 8, barX, 4, 0xFF0000FF, true);
    } else {
        GRRLIB_Rectangle(centerX + barX, centerY - 8, -barX, 4, 0xFF0000FF, true);
    }
    
    // Draw Y axis (green)
    int barY = (int)(scaleY * maxBarLength);
    if (barY > 0) {
        GRRLIB_Rectangle(centerX, centerY - 4, barY, 4, 0x00FF00FF, true);
    } else {
        GRRLIB_Rectangle(centerX + barY, centerY - 4, -barY, 4, 0x00FF00FF, true);
    }
    
    // Draw Z axis (blue)
    int barZ = (int)(scaleZ * maxBarLength);
    if (barZ > 0) {
        GRRLIB_Rectangle(centerX, centerY, barZ, 4, 0x0000FFFF, true);
    } else {
        GRRLIB_Rectangle(centerX + barZ, centerY, -barZ, 4, 0x0000FFFF, true);
    }
    
    // Draw center line
    GRRLIB_Rectangle(centerX - 1, centerY - 10, 2, 20, 0xFFFFFFFF, true);
    
    // Draw axis labels (simple colored squares)
    GRRLIB_Rectangle(x + 2, y + 2, 8, 4, 0xFF0000FF, true); // X - red
    GRRLIB_Rectangle(x + 2, y + 7, 8, 4, 0x00FF00FF, true); // Y - green
    GRRLIB_Rectangle(x + 2, y + 12, 8, 4, 0x0000FFFF, true); // Z - blue
}

void Renderer::drawOrientationIndicator(int x, int y, int size, const orient_t& orient) {
    if (!initialized) return;
    
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int radius = size / 3;
    
    // Draw orientation circle background
    GRRLIB_Rectangle(centerX - radius, centerY - radius, radius * 2, radius * 2, 0x333333AA, true);
    GRRLIB_Rectangle(centerX - radius, centerY - radius, radius * 2, radius * 2, 0xAAAAAAFF, false);
    
    // Calculate roll and pitch indicators
    float rollRad = orient.roll * 3.14159f / 180.0f;
    float pitchRad = orient.pitch * 3.14159f / 180.0f;
    
    // Roll indicator (rotation line)
    int rollX = centerX + (int)(radius * 0.8f * cos(rollRad));
    int rollY = centerY + (int)(radius * 0.8f * sin(rollRad));
    GRRLIB_Rectangle(centerX - 1, centerY - 1, 2, 2, 0xFFFF00FF, true); // Center
    GRRLIB_Rectangle(rollX - 1, rollY - 1, 2, 2, 0xFFFF00FF, true); // Roll point
    
    // Pitch indicator (vertical offset)
    int pitchOffset = (int)(radius * 0.5f * sin(pitchRad));
    GRRLIB_Rectangle(centerX - 8, centerY + pitchOffset - 1, 16, 2, 0xFF00FFFF, true);
    
    // Yaw indicator (simple value representation with small bars)
    int yawBars = (int)(orient.yaw / 15.0f); // One bar per 15 degrees
    if (yawBars > 6) yawBars = 6;
    if (yawBars < -6) yawBars = -6;
    
    for (int i = 0; i < abs(yawBars); i++) {
        int barX = centerX + (yawBars > 0 ? (radius + 5 + i * 3) : -(radius + 5 + i * 3));
        GRRLIB_Rectangle(barX, centerY + radius - 5, 2, 8, 0x00FFFFFF, true);
    }
}

void Renderer::drawGForceIndicator(int x, int y, int size, const gforce_t& gforce) {
    if (!initialized) return;
    
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    
    // Draw G-force as colored bars
    int maxLength = size / 2;
    
    // X G-force (horizontal red bar)
    int gx = (int)(gforce.x * maxLength);
    if (gx > maxLength) gx = maxLength;
    if (gx < -maxLength) gx = -maxLength;
    
    if (gx > 0) {
        GRRLIB_Rectangle(centerX, centerY - 6, gx, 4, 0xFF6666FF, true);
    } else {
        GRRLIB_Rectangle(centerX + gx, centerY - 6, -gx, 4, 0xFF6666FF, true);
    }
    
    // Y G-force (horizontal green bar)
    int gy = (int)(gforce.y * maxLength);
    if (gy > maxLength) gy = maxLength;
    if (gy < -maxLength) gy = -maxLength;
    
    if (gy > 0) {
        GRRLIB_Rectangle(centerX, centerY - 2, gy, 4, 0x66FF66FF, true);
    } else {
        GRRLIB_Rectangle(centerX + gy, centerY - 2, -gy, 4, 0x66FF66FF, true);
    }
    
    // Z G-force (horizontal blue bar)
    int gz = (int)(gforce.z * maxLength);
    if (gz > maxLength) gz = maxLength;
    if (gz < -maxLength) gz = -maxLength;
    
    if (gz > 0) {
        GRRLIB_Rectangle(centerX, centerY + 2, gz, 4, 0x6666FFFF, true);
    } else {
        GRRLIB_Rectangle(centerX + gz, centerY + 2, -gz, 4, 0x6666FFFF, true);
    }
    
    // Center reference line
    GRRLIB_Rectangle(centerX - 1, centerY - 8, 2, 16, 0xFFFFFFFF, true);
    
    // G-force magnitude indicator (circle size based on total force)
    float magnitude = sqrt(gforce.x * gforce.x + gforce.y * gforce.y + gforce.z * gforce.z);
    int magRadius = (int)(magnitude * 8);
    if (magRadius > 15) magRadius = 15;
    if (magRadius < 2) magRadius = 2;
    
    GRRLIB_Rectangle(centerX - magRadius/2, centerY + 15 - magRadius/2, magRadius, magRadius, 0xFFFF00AA, true);
    GRRLIB_Rectangle(centerX - magRadius/2, centerY + 15 - magRadius/2, magRadius, magRadius, 0xFFFF00FF, false);
}

void Renderer::drawIRDots(int x, int y, int width, int height, const ir_t& ir) {
    if (!initialized) return;
    
    // IR sensor area background (simulating Wii screen)
    GRRLIB_Rectangle(x, y, width, height, 0x111111FF, true);
    GRRLIB_Rectangle(x, y, width, height, 0x888888FF, false);
    
    // Draw detected IR dots
    for (int i = 0; i < 4 && i < ir.num_dots; i++) {
        if (ir.dot[i].visible) {
            // Scale coordinates to fit our display area
            int dotX = x + (int)((float)ir.dot[i].rx / 1024.0f * width);
            int dotY = y + (int)((float)ir.dot[i].ry / 768.0f * height);
            
            // Ensure dots stay within bounds
            if (dotX >= x && dotX < x + width && dotY >= y && dotY < y + height) {
                // Draw dot with intensity-based size
                int dotSize = 3 + ir.dot[i].size / 4;
                if (dotSize > 8) dotSize = 8;
                
                u32 dotColor = 0xFF0000FF; // Red for IR dots
                GRRLIB_Rectangle(dotX - dotSize/2, dotY - dotSize/2, dotSize, dotSize, dotColor, true);
                GRRLIB_Rectangle(dotX - dotSize/2, dotY - dotSize/2, dotSize, dotSize, 0xFFFFFFFF, false);
            }
        }
    }
    
    // Draw cursor position if valid
    if (ir.raw_valid) {
        int cursorX = x + (int)(ir.ax / 1024.0f * width);
        int cursorY = y + (int)(ir.ay / 768.0f * height);
        
        if (cursorX >= x && cursorX < x + width && cursorY >= y && cursorY < y + height) {
            // Draw crosshair cursor
            GRRLIB_Rectangle(cursorX - 5, cursorY - 1, 10, 2, 0x00FF00FF, true);
            GRRLIB_Rectangle(cursorX - 1, cursorY - 5, 2, 10, 0x00FF00FF, true);
        }
    }
    
    // Status indicators
    if (ir.num_dots > 0) {
        // Show number of detected dots
        for (int i = 0; i < ir.num_dots && i < 4; i++) {
            GRRLIB_Rectangle(x + 2 + i * 6, y + height - 8, 4, 4, 0x00FF00FF, true);
        }
    } else {
        // No dots detected
        GRRLIB_Rectangle(x + 2, y + height - 8, 4, 4, 0xFF0000FF, true);
    }
}