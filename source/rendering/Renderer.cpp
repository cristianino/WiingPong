// source/rendering/Renderer.cpp
// Enhanced with modern textures and visual effects:
// - Ball: 3D sphere with glow and highlight effects
// - Paddles: Gradient textures with themed colors and 3D depth
// - Court: Modern design with center circle and enhanced boundaries
// - Background: Subtle gradient for better visual appeal

#include "rendering/Renderer.h"
#include <gccore.h>      // For VIDEO_Init if needed
#include <stdio.h>       // For snprintf
#include <math.h>        // For sqrt, cos, sin
#include <algorithm>     // For std::remove_if
#include <wiiuse/wpad.h> // For WPAD button constants

Renderer::Renderer() : initialized(false), debugVisible(false), font(nullptr), currentTime(0.0f)
{
    // Initialize animation system variables
    cameraShake.intensity = 0.0f;
    cameraShake.duration = 0.0f;
    cameraShake.timeLeft = 0.0f;
    cameraShake.frequency = 15.0f;
    cameraShake.offsetX = 0.0f;
    cameraShake.offsetY = 0.0f;

    fadeTransition.type = FadeTransition::None;
    fadeTransition.duration = 0.0f;
    fadeTransition.timeLeft = 0.0f;
    fadeTransition.alpha = 0.0f;
    fadeTransition.color = 0x000000FF;
}

Renderer::~Renderer()
{
    if (initialized)
    {
        GRRLIB_Exit();
    }
}

void Renderer::init()
{
    if (initialized)
        return;

    VIDEO_Init();
    GRRLIB_Init();

    // Initialize background manager
    backgroundManager.init();

    // Setup effect configurations
    setupEffectConfigurations();

    initialized = true;
}

void Renderer::update(float deltaTime, const PhysicsEngine &physics)
{
    if (!initialized)
        return;

    // Update time for effects
    currentTime += deltaTime;

    // Update background animations
    backgroundManager.update(deltaTime, physics);

    // Update ball trail
    updateBallTrail(physics.positions[BALL], physics.velocities[BALL]);

    // Update animation systems
    updateParticles(deltaTime);
    updateCameraShake(deltaTime);
    updateFadeTransition(deltaTime);
    updateUIAnimations(deltaTime);
}

void Renderer::render(const PhysicsEngine &physics)
{
    if (!initialized)
        return;

    AssetManager &assets = AssetManager::getInstance();

    // Determine and set the appropriate atlas based on scores
    AtlasType requiredAtlas = assets.determineAtlasForScores(physics.playerScore, physics.cpuScore);
    if (requiredAtlas != assets.getCurrentAtlas())
    {
        assets.setCurrentAtlas(requiredAtlas);
        printf("Atlas switched due to score change: Player %d - CPU %d\n",
               physics.playerScore, physics.cpuScore);
    }

    GRRLIB_texImg *spritesheet = assets.getSpritesheet();

    if (spritesheet)
    {
        // Use spritesheet for rendering with parallax backgrounds

        // Clear screen with color based on atlas type
        if (assets.getCurrentAtlas() == AtlasType::Intense)
        {
            // Darker, more intense background for tied intense games
            GRRLIB_FillScreen(0x000a1aFF);
        }
        else
        {
            // Regular background
            GRRLIB_FillScreen(0x001122FF);
        }

        // Draw parallax background layers instead of static background
        backgroundManager.render(assets.getCurrentAtlas());

        // Get current effect configuration
        EffectConfig effectConfig = getCurrentEffectConfig(assets.getCurrentAtlas());

        // Draw shadows first (behind main elements)
        drawPaddleShadow(physics.positions[PLAYER_PADDLE], physics.sizes[PLAYER_PADDLE], true, effectConfig);
        drawPaddleShadow(physics.positions[CPU_PADDLE], physics.sizes[CPU_PADDLE], false, effectConfig);
        drawBallShadow(physics.positions[BALL], physics.sizes[BALL], effectConfig);

        // Draw ball trail (behind ball but in front of shadows)
        if (effectConfig.enableTrails)
        {
            drawBallTrail(physics.sizes[BALL], effectConfig);
        }

        // Draw entities using sprites (main elements)
        drawPaddleSprite(physics.positions[PLAYER_PADDLE], physics.sizes[PLAYER_PADDLE], true);
        drawPaddleSprite(physics.positions[CPU_PADDLE], physics.sizes[CPU_PADDLE], false);
        drawBallSprite(physics.positions[BALL], physics.sizes[BALL]);

        // Draw glow effects (on top of main elements for intense mode)
        if (effectConfig.glowIntensity > 0.0f)
        {
            u32 glowColor = (assets.getCurrentAtlas() == AtlasType::Intense) ? 0x00FFFFFF : 0xFFFFFFFF;
            drawGlowEffect(physics.positions[BALL], physics.sizes[BALL], effectConfig, glowColor);
        }

        // Draw scores using sprite digits
        // Get scores from physics engine
        if (physics.playerScore < 10)
        {
            drawScoreDigitSprite(physics.playerScore, 280, 50);
        }
        if (physics.cpuScore < 10)
        {
            drawScoreDigitSprite(physics.cpuScore, 360, 50);
        }

        // Add special effects for intense mode
        if (assets.getCurrentAtlas() == AtlasType::Intense)
        {
            drawIntenseEffects();
        }
    }
    else
    {
        // Fallback to original primitive rendering

        // Clear screen with modern gradient background
        GRRLIB_FillScreen(0x001122FF); // Dark blue base

        // Add subtle gradient effect
        for (int y = 0; y < 480; y += 20)
        {
            u8 intensity = 0x11 + (y * 0x11) / 480;
            u32 gradientColor = (intensity << 16) | (intensity << 8) | 0x22FF;
            GRRLIB_Rectangle(0, y, 640, 20, gradientColor, true);
        }

        // Draw court line
        drawCourt();

        // Draw entities
        // Player paddle (blue tint)
        drawPaddle(physics.positions[PLAYER_PADDLE], physics.sizes[PLAYER_PADDLE], 0x4488FFFF);
        // CPU paddle (red tint)
        drawPaddle(physics.positions[CPU_PADDLE], physics.sizes[CPU_PADDLE], 0xFF4488FF);
        // Ball
        drawBall(physics.positions[BALL], physics.sizes[BALL], 0xFFFFFFFF);

        // Draw scores (placeholder bars)
        drawScores(physics);
    }

    // Apply camera shake offset if active
    if (cameraShake.timeLeft > 0.0f)
    {
        // Apply shake offset to all subsequent rendering
        // This would typically be done at the beginning, but for simplicity we note it here
    }

    // Render particles (overlay on top of everything)
    renderParticles();

    // Render fade transition (final overlay)
    renderFadeTransition();
}

void Renderer::renderMenu(const GameStateManager &gameState)
{
    if (!initialized)
        return;

    // Clear screen with dark blue background for menu
    GRRLIB_FillScreen(0x001133FF);

    // Draw background decorations
    drawMenuBackground();

    // Try to get and draw the banner
    AssetManager &assets = AssetManager::getInstance();
    GRRLIB_texImg *banner = assets.getTexture("banner");

    if (banner)
    {
        printf("Banner loaded successfully! Size: %dx%d\n", banner->w, banner->h);
        // Center the banner at the top - scale down if it's too big
        int bannerX, bannerY;
        float scaleX = 1.0f, scaleY = 1.0f;

        // If banner is too wide (> 400px), scale it down
        if (banner->w > 400)
        {
            scaleX = 400.0f / banner->w;
            scaleY = scaleX; // Keep aspect ratio
        }

        int displayWidth = banner->w * scaleX;
        int displayHeight = banner->h * scaleY;
        bannerX = (640 - displayWidth) / 2;
        bannerY = 50;

        GRRLIB_DrawImg(bannerX, bannerY, banner, 0, scaleX, scaleY, 0xFFFFFFFF);

        // Menu items positioned lower to make room for banner
        const char *menuItems[] = {"Iniciar Juego", "Salir"};
        int startY = bannerY + displayHeight + 50;
        int itemHeight = 60;

        for (int i = 0; i < gameState.getMenuItemCount(); i++)
        {
            int y = startY + i * itemHeight;
            bool selected = (i == gameState.getMenuSelectedIndex());
            drawMenuItem(menuItems[i], 320, y, selected);
        }
    }
    else
    {
        printf("Banner NOT loaded - using fallback rectangles\n");
        // Fallback: text-based title if banner doesn't load
        GRRLIB_Rectangle(220, 100, 200, 50, 0x4444AAFF, true);
        GRRLIB_Rectangle(225, 105, 190, 40, 0x6666CCFF, true);

        // Menu items
        const char *menuItems[] = {"Iniciar Juego", "Salir"};
        int startY = 200;
        int itemHeight = 60;

        for (int i = 0; i < gameState.getMenuItemCount(); i++)
        {
            int y = startY + i * itemHeight;
            bool selected = (i == gameState.getMenuSelectedIndex());
            drawMenuItem(menuItems[i], 320, y, selected);
        }
    }

    // Instructions at bottom with visual indicators
    int instructY = 400;
    GRRLIB_Rectangle(100, instructY, 440, 50, 0x333366FF, true);
    GRRLIB_Rectangle(105, instructY + 5, 430, 40, 0x555588FF, true);

    // Draw control indicators
    // D-Pad up/down arrows
    int iconY = instructY + 15;
    GRRLIB_Rectangle(120, iconY - 5, 15, 3, 0xFFFFFFFF, true); // Up arrow
    GRRLIB_Rectangle(125, iconY - 8, 5, 9, 0xFFFFFFFF, true);

    GRRLIB_Rectangle(120, iconY + 15, 15, 3, 0xFFFFFFFF, true); // Down arrow
    GRRLIB_Rectangle(125, iconY + 12, 5, 9, 0xFFFFFFFF, true);

    // A button indicator
    GRRLIB_Rectangle(300, iconY, 20, 20, 0x00AA00FF, true);
    GRRLIB_Rectangle(305, iconY + 5, 10, 10, 0x00FF00FF, true);

    // Text placeholders (simple rectangles since we don't have font rendering)
    GRRLIB_Rectangle(150, iconY + 2, 80, 8, 0xAAAAAAFF, true); // "Navigate"
    GRRLIB_Rectangle(330, iconY + 2, 60, 8, 0xAAAAAAFF, true); // "Select"
}

void Renderer::drawPaddle(const Position &pos, const Size &size, u32 color)
{
    // Draw modern paddle with gradient and 3D effect
    int x = pos.x;
    int y = pos.y;
    int w = size.width;
    int h = size.height;

    // Extract RGB components from color parameter for themed coloring
    u8 r = (color >> 24) & 0xFF;
    u8 g = (color >> 16) & 0xFF;
    u8 b = (color >> 8) & 0xFF;

    // Draw outer glow with theme color
    u32 glowColor = ((r / 3) << 24) | ((g / 3) << 16) | ((b / 3) << 8) | 0x30;
    GRRLIB_Rectangle(x - 2, y - 2, w + 4, h + 4, glowColor, true);

    // Draw main paddle body with themed base color
    u32 baseColor = ((r / 2) << 24) | ((g / 2) << 16) | ((b / 2) << 8) | 0xFF;
    GRRLIB_Rectangle(x, y, w, h, baseColor, true);

    // Gradient layers for depth with theme color
    for (int i = 0; i < w / 4; i++)
    {
        u8 alpha = 255 - (i * 40);
        u8 gradR = r / 2 + (r / 4) * i / (w / 4);
        u8 gradG = g / 2 + (g / 4) * i / (w / 4);
        u8 gradB = b / 2 + (b / 4) * i / (w / 4);
        u32 gradientColor = (gradR << 24) | (gradG << 16) | (gradB << 8) | alpha;
        GRRLIB_Rectangle(x + i, y, 2, h, gradientColor, true);
    }

    // Highlight on left edge for 3D effect
    GRRLIB_Rectangle(x, y, 2, h, 0xFFFFFFBB, true);

    // Shadow on right edge
    GRRLIB_Rectangle(x + w - 2, y, 2, h, 0x00000060, true);

    // Add subtle pattern/texture lines
    for (int i = y + 5; i < y + h - 5; i += 8)
    {
        u32 lineColor = ((r * 3 / 4) << 24) | ((g * 3 / 4) << 16) | ((b * 3 / 4) << 8) | 0xAA;
        GRRLIB_Line(x + 2, i, x + w - 4, i, lineColor);
    }

    // Corner highlights for extra polish
    GRRLIB_Rectangle(x + 1, y + 1, 3, 3, 0xFFFFFF80, true);
    GRRLIB_Rectangle(x + 1, y + h - 4, 3, 3, 0xFFFFFF80, true);
}

void Renderer::drawBall(const Position &pos, const Size &size, u32 color)
{
    // Draw a modern-looking ball with gradient and glow effect
    float centerX = pos.x + size.width / 2.0f;
    float centerY = pos.y + size.height / 2.0f;
    float radius = size.width / 2.0f;

    // Draw outer glow (larger, more transparent)
    GRRLIB_Circle(centerX, centerY, radius + 4, 0x00FFFF30, true);
    GRRLIB_Circle(centerX, centerY, radius + 2, 0x00FFFF60, true);

    // Draw main ball with gradient effect
    GRRLIB_Circle(centerX, centerY, radius, 0xFFFFFFFF, true);

    // Add highlight for 3D effect (smaller white circle offset)
    GRRLIB_Circle(centerX - radius / 3, centerY - radius / 3, radius / 3, 0xFFFFFFCC, true);

    // Add subtle inner shadow for depth
    GRRLIB_Circle(centerX + radius / 4, centerY + radius / 4, radius / 4, 0x00000040, true);
}

void Renderer::drawCourt()
{
    // Draw modern court with enhanced center line and effects

    // Draw subtle court boundaries
    GRRLIB_Rectangle(0, 0, 640, 4, 0x444444FF, true);   // Top boundary
    GRRLIB_Rectangle(0, 476, 640, 4, 0x444444FF, true); // Bottom boundary

    // Draw center line with glow effect
    GRRLIB_Line(320, 0, 320, 480, 0x00FFFF60); // Glow
    GRRLIB_Line(319, 0, 319, 480, 0xFFFFFFFF); // Main line
    GRRLIB_Line(321, 0, 321, 480, 0xFFFFFFFF); // Main line (double width)

    // Draw center circle
    GRRLIB_Circle(320, 240, 50, 0xFFFFFF80, false);
    GRRLIB_Circle(320, 240, 48, 0x00FFFF40, false);

    // Draw corner indicators
    GRRLIB_Rectangle(0, 0, 20, 20, 0x00FFFF60, true);
    GRRLIB_Rectangle(620, 0, 20, 20, 0x00FFFF60, true);
    GRRLIB_Rectangle(0, 460, 20, 20, 0x00FFFF60, true);
    GRRLIB_Rectangle(620, 460, 20, 20, 0x00FFFF60, true);
}

void Renderer::drawScores(const PhysicsEngine &physics)
{
    // Player score bars (green)
    for (int i = 0; i < physics.playerScore; ++i)
    {
        GRRLIB_Rectangle(200 + i * 15, 40, 10, 20, 0x00FF00FF, true);
    }
    // CPU score bars (red)
    for (int i = 0; i < physics.cpuScore; ++i)
    {
        GRRLIB_Rectangle(400 + i * 15, 40, 10, 20, 0xFF0000FF, true);
    }
}

void Renderer::drawText(const char *text, int x, int y, u32 color)
{
    // Text function not available - using visual indicators instead
    // This function is kept for interface compatibility but does nothing
}

void Renderer::renderDebugInfo(const InputManager &input)
{
#if WIINGPONG_DEBUG_ENABLED
    if (!initialized || !debugVisible)
        return;

    // Draw debug background with enhanced styling
    GRRLIB_Rectangle(10, 400, 700, 70, 0x000000CC, true);  // Wider background for more buttons
    GRRLIB_Rectangle(10, 400, 700, 70, 0xFFFFFFFF, false); // White border

    // Add inner shadow effect
    GRRLIB_Rectangle(11, 401, 698, 1, 0x00000066, true); // Top inner shadow
    GRRLIB_Rectangle(11, 401, 1, 68, 0x00000066, true);  // Left inner shadow

    // Show initialization status with enhanced Wii-style indicator
    int statusSize = 24; // Larger status indicator
    if (input.isInitialized())
    {
        // Green Wii-style "connected" indicator
        drawWiiButton(20, 408, statusSize, 0xE0FFE0FF, 0x40FF40FF, true, false);
        // Add a small "connected" symbol (circle with checkmark approximation)
        GRRLIB_Rectangle(28, 416, 8, 8, 0x006600FF, true); // Small circle
        GRRLIB_Rectangle(30, 418, 4, 2, 0xFFFFFFFF, true); // Checkmark horizontal
        GRRLIB_Rectangle(32, 420, 2, 2, 0xFFFFFFFF, true); // Checkmark vertical
    }
    else
    {
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
    u32 wiiButtonBase = 0xF0F0F0FF; // Light gray base (Wii remote style)

    // A Button indicator (Blue when pressed, like Wii remote)
    u32 colorA_base = wiiButtonBase;
    u32 colorA_active = 0x4080FFFF; // Blue like Wii A button
    bool isAPressed = (held & WPAD_BUTTON_A) != 0;
    drawWiiButton(20, buttonY, buttonSize, colorA_base, colorA_active, isAPressed, true);
    u32 textColorA = isAPressed ? 0xFFFFFFFF : 0x000000FF;
    drawButtonSymbolA(20, buttonY, buttonSize, textColorA);

    // B Button indicator (Red when pressed, like Wii remote)
    u32 colorB_base = wiiButtonBase;
    u32 colorB_active = 0xFF4040FF; // Red like Wii B button
    bool isBPressed = (held & WPAD_BUTTON_B) != 0;
    drawWiiButton(20 + spacing, buttonY, buttonSize, colorB_base, colorB_active, isBPressed, true);
    u32 textColorB = isBPressed ? 0xFFFFFFFF : 0x000000FF;
    drawButtonSymbolB(20 + spacing, buttonY, buttonSize, textColorB);

    // HOME Button indicator (Silver/gray when pressed)
    u32 colorHome_base = wiiButtonBase;
    u32 colorHome_active = 0x808080FF; // Gray like Wii HOME button
    bool isHomePressed = (held & WPAD_BUTTON_HOME) != 0;
    drawWiiButton(20 + spacing * 2, buttonY, buttonSize, colorHome_base, colorHome_active, isHomePressed, true);
    u32 textColorHome = isHomePressed ? 0xFFFFFFFF : 0x000000FF;
    drawButtonSymbolHome(20 + spacing * 2, buttonY, buttonSize, textColorHome);

    // PLUS Button indicator (White/light when pressed)
    u32 colorPlus_base = wiiButtonBase;
    u32 colorPlus_active = 0xFFFFFFFF; // White when active
    bool isPlusPressed = (held & WPAD_BUTTON_PLUS) != 0;
    drawWiiButton(20 + spacing * 3, buttonY, buttonSize, colorPlus_base, colorPlus_active, isPlusPressed, true);
    u32 textColorPlus = isPlusPressed ? 0x000000FF : 0x666666FF;
    drawButtonSymbolPlus(20 + spacing * 3, buttonY, buttonSize, textColorPlus);

    // MINUS Button indicator (White/light when pressed)
    u32 colorMinus_base = wiiButtonBase;
    u32 colorMinus_active = 0xFFFFFFFF; // White when active
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
    for (int i = 0; i < 16; i++)
    {
        bool bitSet = (held & (1 << i)) != 0;
        u32 bitColor = bitSet ? 0xFFFFFFFF : 0x444444FF;
        u32 borderColor = bitSet ? 0xCCCCCCFF : 0x666666FF;

        // Draw bit indicator with border
        GRRLIB_Rectangle(bitStartX + i * bitSpacing, bitY1, bitWidth, bitHeight, bitColor, true);
        GRRLIB_Rectangle(bitStartX + i * bitSpacing, bitY1, bitWidth, bitHeight, borderColor, false);

        // Add small highlight for active bits
        if (bitSet)
        {
            GRRLIB_Rectangle(bitStartX + i * bitSpacing + 1, bitY1 + 1, bitWidth - 2, 2, 0xFFFFFF88, true);
        }
    }

    // Show pressed buttons as enhanced bit indicators (second row)
    for (int i = 0; i < 16; i++)
    {
        bool bitSet = (pressed & (1 << i)) != 0;
        u32 bitColor = bitSet ? 0xFFFF00FF : 0x333333FF; // Yellow for pressed, darker for inactive
        u32 borderColor = bitSet ? 0xCCCC00FF : 0x555555FF;

        // Draw bit indicator with border
        GRRLIB_Rectangle(bitStartX + i * bitSpacing, bitY2, bitWidth, bitHeight, bitColor, true);
        GRRLIB_Rectangle(bitStartX + i * bitSpacing, bitY2, bitWidth, bitHeight, borderColor, false);

        // Add small highlight for active bits
        if (bitSet)
        {
            GRRLIB_Rectangle(bitStartX + i * bitSpacing + 1, bitY2 + 1, bitWidth - 2, 2, 0xFFFF8888, true);
        }
    }
#endif
}

void Renderer::renderDebugToggleProgress(const InputManager &input)
{
#if WIINGPONG_DEBUG_ENABLED && (WIINGPONG_DEBUG_TOGGLE_METHOD == 1 || WIINGPONG_DEBUG_TOGGLE_METHOD == 2)
    if (!initialized)
        return;

    float progress = input.getDebugToggleProgress();

    if (progress > 0.0f)
    {
        // Draw progress bar background
        int barWidth = 200;
        int barHeight = 20;
        int barX = (640 - barWidth) / 2; // Center horizontally
        int barY = 100;                  // Near top of screen

        // Background
        GRRLIB_Rectangle(barX, barY, barWidth, barHeight, 0x000000CC, true);
        GRRLIB_Rectangle(barX, barY, barWidth, barHeight, 0xFFFFFFFF, false);

        // Progress fill
        int fillWidth = (int)(barWidth * progress);
        u32 fillColor = 0x00FF00FF; // Green
        if (progress > 0.8f)
        {
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

void Renderer::drawWiiButton(int x, int y, int size, u32 baseColor, u32 activeColor, bool isPressed, bool hasSymbol)
{
    if (!initialized)
        return;

    // Calculate colors for 3D effect
    u32 buttonColor = isPressed ? activeColor : baseColor;
    u32 shadowColor = 0x000000AA;    // Semi-transparent black shadow
    u32 highlightColor = 0xFFFFFF88; // Semi-transparent white highlight
    u32 borderColor = 0x999999FF;    // Gray border

    // Draw shadow (offset down and right)
    GRRLIB_Rectangle(x + 2, y + 2, size, size, shadowColor, true);

    // Draw main button body with gradient effect
    if (isPressed)
    {
        // Pressed state: darker colors, inverted gradient
        GRRLIB_Rectangle(x, y, size, size, buttonColor, true);
        // Dark top edge for pressed effect
        GRRLIB_Rectangle(x, y, size, 2, 0x000000AA, true);
        GRRLIB_Rectangle(x, y, 2, size, 0x000000AA, true);
    }
    else
    {
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
    if (!isPressed)
    {
        GRRLIB_Rectangle(x + 1, y + 1, size - 2, size - 2, 0xFFFFFF66, false);
    }
}

void Renderer::drawButtonSymbolA(int x, int y, int size, u32 textColor)
{
    if (!initialized)
        return;

    // Draw letter "A" using rectangles
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int letterSize = size / 3;

    // Vertical lines
    GRRLIB_Rectangle(centerX - letterSize / 2, centerY - letterSize / 2, 2, letterSize, textColor, true);
    GRRLIB_Rectangle(centerX + letterSize / 2 - 2, centerY - letterSize / 2, 2, letterSize, textColor, true);

    // Top horizontal line
    GRRLIB_Rectangle(centerX - letterSize / 2, centerY - letterSize / 2, letterSize, 2, textColor, true);

    // Middle horizontal line
    GRRLIB_Rectangle(centerX - letterSize / 2 + 2, centerY - 2, letterSize - 4, 2, textColor, true);
}

void Renderer::drawButtonSymbolB(int x, int y, int size, u32 textColor)
{
    if (!initialized)
        return;

    // Draw letter "B" using rectangles
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int letterSize = size / 3;

    // Vertical line
    GRRLIB_Rectangle(centerX - letterSize / 2, centerY - letterSize / 2, 2, letterSize, textColor, true);

    // Top horizontal line
    GRRLIB_Rectangle(centerX - letterSize / 2, centerY - letterSize / 2, letterSize - 2, 2, textColor, true);

    // Middle horizontal line
    GRRLIB_Rectangle(centerX - letterSize / 2, centerY - 1, letterSize - 4, 2, textColor, true);

    // Bottom horizontal line
    GRRLIB_Rectangle(centerX - letterSize / 2, centerY + letterSize / 2 - 2, letterSize - 2, 2, textColor, true);

    // Right vertical segments
    GRRLIB_Rectangle(centerX + letterSize / 2 - 4, centerY - letterSize / 2 + 2, 2, letterSize / 2 - 3, textColor, true);
    GRRLIB_Rectangle(centerX + letterSize / 2 - 4, centerY + 1, 2, letterSize / 2 - 3, textColor, true);
}

void Renderer::drawButtonSymbolHome(int x, int y, int size, u32 textColor)
{
    if (!initialized)
        return;

    // Draw house symbol for HOME button
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int houseSize = size / 3;

    // House base (rectangle)
    GRRLIB_Rectangle(centerX - houseSize / 2, centerY, houseSize, houseSize / 2, textColor, false);

    // House roof (triangle approximation with rectangles)
    for (int i = 0; i < houseSize / 2; i++)
    {
        int roofWidth = (houseSize / 2 - i) * 2;
        GRRLIB_Rectangle(centerX - roofWidth / 2, centerY - houseSize / 2 + i, roofWidth, 1, textColor, true);
    }

    // Door
    int doorWidth = houseSize / 4;
    int doorHeight = houseSize / 3;
    GRRLIB_Rectangle(centerX - doorWidth / 2, centerY + houseSize / 2 - doorHeight, doorWidth, doorHeight, textColor, true);
}

void Renderer::drawButtonSymbolPlus(int x, int y, int size, u32 textColor)
{
    if (!initialized)
        return;

    // Draw "+" symbol
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int crossSize = size / 3;
    int thickness = 3;

    // Horizontal line
    GRRLIB_Rectangle(centerX - crossSize / 2, centerY - thickness / 2, crossSize, thickness, textColor, true);

    // Vertical line
    GRRLIB_Rectangle(centerX - thickness / 2, centerY - crossSize / 2, thickness, crossSize, textColor, true);
}

void Renderer::drawButtonSymbolMinus(int x, int y, int size, u32 textColor)
{
    if (!initialized)
        return;

    // Draw "-" symbol
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int lineSize = size / 3;
    int thickness = 3;

    // Horizontal line
    GRRLIB_Rectangle(centerX - lineSize / 2, centerY - thickness / 2, lineSize, thickness, textColor, true);
}

void Renderer::drawButtonSymbolUp(int x, int y, int size, u32 textColor)
{
    if (!initialized)
        return;

    // Draw up arrow (triangle pointing up)
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int arrowSize = size / 3;

    // Simple triangle using rectangles
    for (int i = 0; i < arrowSize / 2; i++)
    {
        int lineWidth = (i + 1) * 2;
        GRRLIB_Rectangle(centerX - i, centerY - arrowSize / 2 + i, lineWidth, 1, textColor, true);
    }

    // Arrow stem
    GRRLIB_Rectangle(centerX - 1, centerY, 2, arrowSize / 2, textColor, true);
}

void Renderer::drawButtonSymbolDown(int x, int y, int size, u32 textColor)
{
    if (!initialized)
        return;

    // Draw down arrow (triangle pointing down)
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int arrowSize = size / 3;

    // Arrow stem
    GRRLIB_Rectangle(centerX - 1, centerY - arrowSize / 2, 2, arrowSize / 2, textColor, true);

    // Simple triangle using rectangles
    for (int i = 0; i < arrowSize / 2; i++)
    {
        int lineWidth = (arrowSize / 2 - i) * 2;
        GRRLIB_Rectangle(centerX - (arrowSize / 2 - i - 1), centerY + i, lineWidth, 1, textColor, true);
    }
}

void Renderer::drawButtonSymbolLeft(int x, int y, int size, u32 textColor)
{
    if (!initialized)
        return;

    // Draw left arrow (triangle pointing left)
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int arrowSize = size / 3;

    // Simple triangle using rectangles
    for (int i = 0; i < arrowSize / 2; i++)
    {
        int lineHeight = (i + 1) * 2;
        GRRLIB_Rectangle(centerX - arrowSize / 2 + i, centerY - i, 1, lineHeight, textColor, true);
    }

    // Arrow stem
    GRRLIB_Rectangle(centerX, centerY - 1, arrowSize / 2, 2, textColor, true);
}

void Renderer::drawButtonSymbolRight(int x, int y, int size, u32 textColor)
{
    if (!initialized)
        return;

    // Draw right arrow (triangle pointing right)
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int arrowSize = size / 3;

    // Arrow stem
    GRRLIB_Rectangle(centerX - arrowSize / 2, centerY - 1, arrowSize / 2, 2, textColor, true);

    // Simple triangle using rectangles
    for (int i = 0; i < arrowSize / 2; i++)
    {
        int lineHeight = (arrowSize / 2 - i) * 2;
        GRRLIB_Rectangle(centerX + i, centerY - (arrowSize / 2 - i - 1), 1, lineHeight, textColor, true);
    }
}

void Renderer::drawButtonSymbol1(int x, int y, int size, u32 textColor)
{
    if (!initialized)
        return;

    // Draw number "1"
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int numberSize = size / 3;

    // Vertical line for "1"
    GRRLIB_Rectangle(centerX, centerY - numberSize / 2, 2, numberSize, textColor, true);

    // Top angled part
    GRRLIB_Rectangle(centerX - 2, centerY - numberSize / 2, 2, 2, textColor, true);

    // Bottom base
    GRRLIB_Rectangle(centerX - 3, centerY + numberSize / 2 - 2, 6, 2, textColor, true);
}

void Renderer::drawButtonSymbol2(int x, int y, int size, u32 textColor)
{
    if (!initialized)
        return;

    // Draw number "2"
    int centerX = x + size / 2;
    int centerY = y + size / 2;
    int numberSize = size / 3;

    // Top horizontal line
    GRRLIB_Rectangle(centerX - numberSize / 2, centerY - numberSize / 2, numberSize, 2, textColor, true);

    // Top right vertical
    GRRLIB_Rectangle(centerX + numberSize / 2 - 2, centerY - numberSize / 2, 2, numberSize / 2 - 1, textColor, true);

    // Middle horizontal line
    GRRLIB_Rectangle(centerX - numberSize / 2, centerY - 1, numberSize, 2, textColor, true);

    // Bottom left vertical
    GRRLIB_Rectangle(centerX - numberSize / 2, centerY + 1, 2, numberSize / 2 - 1, textColor, true);

    // Bottom horizontal line
    GRRLIB_Rectangle(centerX - numberSize / 2, centerY + numberSize / 2 - 2, numberSize, 2, textColor, true);
}

void Renderer::drawDPadCross(int x, int y, int size, bool upPressed, bool downPressed, bool leftPressed, bool rightPressed)
{
    if (!initialized)
        return;

    // Define colors for D-pad sections
    u32 baseColor = 0xF0F0F0FF;
    u32 activeColor = 0xFFD700FF; // Gold color when pressed
    u32 borderColor = 0x999999FF;

    int crossThickness = size / 3;
    int centerX = x + size / 2;
    int centerY = y + size / 2;

    // Draw D-pad cross background
    // Vertical bar
    GRRLIB_Rectangle(centerX - crossThickness / 2, y, crossThickness, size, baseColor, true);
    // Horizontal bar
    GRRLIB_Rectangle(x, centerY - crossThickness / 2, size, crossThickness, baseColor, true);

    // Draw individual button sections with state-dependent colors
    // UP section
    u32 upColor = upPressed ? activeColor : baseColor;
    GRRLIB_Rectangle(centerX - crossThickness / 2, y, crossThickness, size / 2 - crossThickness / 4, upColor, true);
    if (upPressed)
        drawButtonSymbolUp(centerX - crossThickness / 2, y, crossThickness, 0x000000FF);

    // DOWN section
    u32 downColor = downPressed ? activeColor : baseColor;
    GRRLIB_Rectangle(centerX - crossThickness / 2, centerY + crossThickness / 4, crossThickness, size / 2 - crossThickness / 4, downColor, true);
    if (downPressed)
        drawButtonSymbolDown(centerX - crossThickness / 2, centerY + crossThickness / 4, crossThickness, 0x000000FF);

    // LEFT section
    u32 leftColor = leftPressed ? activeColor : baseColor;
    GRRLIB_Rectangle(x, centerY - crossThickness / 2, size / 2 - crossThickness / 4, crossThickness, leftColor, true);
    if (leftPressed)
        drawButtonSymbolLeft(x, centerY - crossThickness / 2, crossThickness, 0x000000FF);

    // RIGHT section
    u32 rightColor = rightPressed ? activeColor : baseColor;
    GRRLIB_Rectangle(centerX + crossThickness / 4, centerY - crossThickness / 2, size / 2 - crossThickness / 4, crossThickness, rightColor, true);
    if (rightPressed)
        drawButtonSymbolRight(centerX + crossThickness / 4, centerY - crossThickness / 2, crossThickness, 0x000000FF);

    // Draw borders for the cross
    // Vertical borders
    GRRLIB_Rectangle(centerX - crossThickness / 2, y, crossThickness, size, borderColor, false);
    // Horizontal borders
    GRRLIB_Rectangle(x, centerY - crossThickness / 2, size, crossThickness, borderColor, false);

    // Draw center junction
    GRRLIB_Rectangle(centerX - crossThickness / 2, centerY - crossThickness / 2, crossThickness, crossThickness, 0xCCCCCCFF, true);
    GRRLIB_Rectangle(centerX - crossThickness / 2, centerY - crossThickness / 2, crossThickness, crossThickness, borderColor, false);
}

// =============================================================================
// SENSOR DEBUG RENDERING FUNCTIONS
// =============================================================================

void Renderer::renderDebugSensorInfo(const InputManager &input)
{
#if WIINGPONG_DEBUG_ENABLED
    if (!initialized || !debugVisible || !input.hasSensorData())
        return;

    // Position sensor panel on the right side of the screen
    int panelX = 450;
    int panelY = 50;
    int panelWidth = 180;
    int panelHeight = 320;

    // Main sensor panel background
    drawSensorBackground(panelX, panelY, panelWidth, panelHeight, "SENSOR DATA");

    // Get sensor data
    const vec3w_t &accel = input.getAccelerometerData();
    const orient_t &orient = input.getOrientationData();
    const gforce_t &gforce = input.getGForceData();
    const ir_t &ir = input.getIRData();

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

void Renderer::drawSensorBackground(int x, int y, int width, int height, const char *title)
{
    if (!initialized)
        return;

    // Background with transparency
    GRRLIB_Rectangle(x, y, width, height, 0x000000CC, true);
    GRRLIB_Rectangle(x, y, width, height, 0x66CCFFFF, false);

    // Title bar
    GRRLIB_Rectangle(x, y, width, 15, 0x0066CCAA, true);
    GRRLIB_Rectangle(x, y, width, 15, 0x66CCFFFF, false);

    // Simple title text (using rectangles for now)
    // TODO: Replace with actual text rendering when font is available
}

void Renderer::drawAccelerometerGraph(int x, int y, int width, int height, const vec3w_t &accel)
{
    if (!initialized)
        return;

    // Calculate center and scale values for visualization
    int centerX = x + width / 2;
    int centerY = y + height / 2;
    int maxBarLength = width / 3;

    // Scale accelerometer values (typically 0-1000 range)
    float scaleX = (float)(accel.x - 512) / 512.0f; // Center around 512
    float scaleY = (float)(accel.y - 512) / 512.0f;
    float scaleZ = (float)(accel.z - 512) / 512.0f;

    // Clamp values
    if (scaleX > 1.0f)
        scaleX = 1.0f;
    if (scaleX < -1.0f)
        scaleX = -1.0f;
    if (scaleY > 1.0f)
        scaleY = 1.0f;
    if (scaleY < -1.0f)
        scaleY = -1.0f;
    if (scaleZ > 1.0f)
        scaleZ = 1.0f;
    if (scaleZ < -1.0f)
        scaleZ = -1.0f;

    // Draw X axis (red)
    int barX = (int)(scaleX * maxBarLength);
    if (barX > 0)
    {
        GRRLIB_Rectangle(centerX, centerY - 8, barX, 4, 0xFF0000FF, true);
    }
    else
    {
        GRRLIB_Rectangle(centerX + barX, centerY - 8, -barX, 4, 0xFF0000FF, true);
    }

    // Draw Y axis (green)
    int barY = (int)(scaleY * maxBarLength);
    if (barY > 0)
    {
        GRRLIB_Rectangle(centerX, centerY - 4, barY, 4, 0x00FF00FF, true);
    }
    else
    {
        GRRLIB_Rectangle(centerX + barY, centerY - 4, -barY, 4, 0x00FF00FF, true);
    }

    // Draw Z axis (blue)
    int barZ = (int)(scaleZ * maxBarLength);
    if (barZ > 0)
    {
        GRRLIB_Rectangle(centerX, centerY, barZ, 4, 0x0000FFFF, true);
    }
    else
    {
        GRRLIB_Rectangle(centerX + barZ, centerY, -barZ, 4, 0x0000FFFF, true);
    }

    // Draw center line
    GRRLIB_Rectangle(centerX - 1, centerY - 10, 2, 20, 0xFFFFFFFF, true);

    // Draw axis labels (simple colored squares)
    GRRLIB_Rectangle(x + 2, y + 2, 8, 4, 0xFF0000FF, true);  // X - red
    GRRLIB_Rectangle(x + 2, y + 7, 8, 4, 0x00FF00FF, true);  // Y - green
    GRRLIB_Rectangle(x + 2, y + 12, 8, 4, 0x0000FFFF, true); // Z - blue
}

void Renderer::drawOrientationIndicator(int x, int y, int size, const orient_t &orient)
{
    if (!initialized)
        return;

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
    GRRLIB_Rectangle(rollX - 1, rollY - 1, 2, 2, 0xFFFF00FF, true);     // Roll point

    // Pitch indicator (vertical offset)
    int pitchOffset = (int)(radius * 0.5f * sin(pitchRad));
    GRRLIB_Rectangle(centerX - 8, centerY + pitchOffset - 1, 16, 2, 0xFF00FFFF, true);

    // Yaw indicator (simple value representation with small bars)
    int yawBars = (int)(orient.yaw / 15.0f); // One bar per 15 degrees
    if (yawBars > 6)
        yawBars = 6;
    if (yawBars < -6)
        yawBars = -6;

    for (int i = 0; i < abs(yawBars); i++)
    {
        int barX = centerX + (yawBars > 0 ? (radius + 5 + i * 3) : -(radius + 5 + i * 3));
        GRRLIB_Rectangle(barX, centerY + radius - 5, 2, 8, 0x00FFFFFF, true);
    }
}

void Renderer::drawGForceIndicator(int x, int y, int size, const gforce_t &gforce)
{
    if (!initialized)
        return;

    int centerX = x + size / 2;
    int centerY = y + size / 2;

    // Draw G-force as colored bars
    int maxLength = size / 2;

    // X G-force (horizontal red bar)
    int gx = (int)(gforce.x * maxLength);
    if (gx > maxLength)
        gx = maxLength;
    if (gx < -maxLength)
        gx = -maxLength;

    if (gx > 0)
    {
        GRRLIB_Rectangle(centerX, centerY - 6, gx, 4, 0xFF6666FF, true);
    }
    else
    {
        GRRLIB_Rectangle(centerX + gx, centerY - 6, -gx, 4, 0xFF6666FF, true);
    }

    // Y G-force (horizontal green bar)
    int gy = (int)(gforce.y * maxLength);
    if (gy > maxLength)
        gy = maxLength;
    if (gy < -maxLength)
        gy = -maxLength;

    if (gy > 0)
    {
        GRRLIB_Rectangle(centerX, centerY - 2, gy, 4, 0x66FF66FF, true);
    }
    else
    {
        GRRLIB_Rectangle(centerX + gy, centerY - 2, -gy, 4, 0x66FF66FF, true);
    }

    // Z G-force (horizontal blue bar)
    int gz = (int)(gforce.z * maxLength);
    if (gz > maxLength)
        gz = maxLength;
    if (gz < -maxLength)
        gz = -maxLength;

    if (gz > 0)
    {
        GRRLIB_Rectangle(centerX, centerY + 2, gz, 4, 0x6666FFFF, true);
    }
    else
    {
        GRRLIB_Rectangle(centerX + gz, centerY + 2, -gz, 4, 0x6666FFFF, true);
    }

    // Center reference line
    GRRLIB_Rectangle(centerX - 1, centerY - 8, 2, 16, 0xFFFFFFFF, true);

    // G-force magnitude indicator (circle size based on total force)
    float magnitude = sqrt(gforce.x * gforce.x + gforce.y * gforce.y + gforce.z * gforce.z);
    int magRadius = (int)(magnitude * 8);
    if (magRadius > 15)
        magRadius = 15;
    if (magRadius < 2)
        magRadius = 2;

    GRRLIB_Rectangle(centerX - magRadius / 2, centerY + 15 - magRadius / 2, magRadius, magRadius, 0xFFFF00AA, true);
    GRRLIB_Rectangle(centerX - magRadius / 2, centerY + 15 - magRadius / 2, magRadius, magRadius, 0xFFFF00FF, false);
}

void Renderer::drawIRDots(int x, int y, int width, int height, const ir_t &ir)
{
    if (!initialized)
        return;

    // IR sensor area background (simulating Wii screen)
    GRRLIB_Rectangle(x, y, width, height, 0x111111FF, true);
    GRRLIB_Rectangle(x, y, width, height, 0x888888FF, false);

    // Draw detected IR dots
    for (int i = 0; i < 4 && i < ir.num_dots; i++)
    {
        if (ir.dot[i].visible)
        {
            // Scale coordinates to fit our display area
            int dotX = x + (int)((float)ir.dot[i].rx / 1024.0f * width);
            int dotY = y + (int)((float)ir.dot[i].ry / 768.0f * height);

            // Ensure dots stay within bounds
            if (dotX >= x && dotX < x + width && dotY >= y && dotY < y + height)
            {
                // Draw dot with intensity-based size
                int dotSize = 3 + ir.dot[i].size / 4;
                if (dotSize > 8)
                    dotSize = 8;

                u32 dotColor = 0xFF0000FF; // Red for IR dots
                GRRLIB_Rectangle(dotX - dotSize / 2, dotY - dotSize / 2, dotSize, dotSize, dotColor, true);
                GRRLIB_Rectangle(dotX - dotSize / 2, dotY - dotSize / 2, dotSize, dotSize, 0xFFFFFFFF, false);
            }
        }
    }

    // Draw cursor position if valid
    if (ir.raw_valid)
    {
        int cursorX = x + (int)(ir.ax / 1024.0f * width);
        int cursorY = y + (int)(ir.ay / 768.0f * height);

        if (cursorX >= x && cursorX < x + width && cursorY >= y && cursorY < y + height)
        {
            // Draw crosshair cursor
            GRRLIB_Rectangle(cursorX - 5, cursorY - 1, 10, 2, 0x00FF00FF, true);
            GRRLIB_Rectangle(cursorX - 1, cursorY - 5, 2, 10, 0x00FF00FF, true);
        }
    }

    // Status indicators
    if (ir.num_dots > 0)
    {
        // Show number of detected dots
        for (int i = 0; i < ir.num_dots && i < 4; i++)
        {
            GRRLIB_Rectangle(x + 2 + i * 6, y + height - 8, 4, 4, 0x00FF00FF, true);
        }
    }
    else
    {
        // No dots detected
        GRRLIB_Rectangle(x + 2, y + height - 8, 4, 4, 0xFF0000FF, true);
    }
}

void Renderer::drawMenuItem(const char *text, int x, int y, bool selected)
{
    u32 backgroundColor = selected ? 0x6666CCFF : 0x333366FF;
    u32 borderColor = selected ? 0xFFFFFFFF : 0x666699FF;
    u32 textColor = selected ? 0xFFFFFFFF : 0xAAAAAAFF;

    // Draw menu item background
    int width = 200;
    int height = 40;
    int itemX = x - width / 2;
    int itemY = y - height / 2;

    GRRLIB_Rectangle(itemX, itemY, width, height, backgroundColor, true);
    GRRLIB_Rectangle(itemX, itemY, width, height, borderColor, false);

    // Since we don't have text rendering, draw simple symbols/indicators
    // For "Iniciar Juego" - draw play triangle
    // For "Salir" - draw X
    if (text && text[0] == 'I')
    { // "Iniciar Juego"
        // Draw play triangle
        int triX = x - 10;
        int triY = y;
        GRRLIB_Rectangle(triX - 8, triY - 8, 4, 16, textColor, true);
        GRRLIB_Rectangle(triX - 4, triY - 6, 4, 12, textColor, true);
        GRRLIB_Rectangle(triX, triY - 4, 4, 8, textColor, true);
        GRRLIB_Rectangle(triX + 4, triY - 2, 4, 4, textColor, true);
    }
    else
    { // "Salir" - draw X
        int xSize = 12;
        for (int i = 0; i < xSize; i++)
        {
            GRRLIB_Rectangle(x - xSize / 2 + i, y - xSize / 2 + i, 2, 2, textColor, true);
            GRRLIB_Rectangle(x - xSize / 2 + i, y + xSize / 2 - i, 2, 2, textColor, true);
        }
    }
}

void Renderer::drawMenuBackground()
{
    // Draw some decorative elements for the menu background
    // Top border
    GRRLIB_Rectangle(0, 0, 640, 10, 0x4444AAFF, true);
    // Bottom border
    GRRLIB_Rectangle(0, 470, 640, 10, 0x4444AAFF, true);

    // Side decorations
    for (int i = 0; i < 10; i++)
    {
        int y = 50 + i * 40;
        GRRLIB_Rectangle(20, y, 10, 20, 0x2222AAFF, true);
        GRRLIB_Rectangle(610, y, 10, 20, 0x2222AAFF, true);
    }
}

// New sprite-based drawing methods

void Renderer::drawPaddleSprite(const Position &pos, const Size &size, bool isLeftPaddle)
{
    AssetManager &assets = AssetManager::getInstance();

    // Calculate scale to fit the paddle size
    SpriteInfo paddleSprite = assets.getSprite(SpriteID::LeftPaddle);
    if (paddleSprite.width == 0)
        return;

    float scaleX = size.width / (float)paddleSprite.width;
    float scaleY = size.height / (float)paddleSprite.height;

    // For right paddle, we might want to flip horizontally
    if (!isLeftPaddle)
    {
        scaleX = -scaleX; // Flip horizontally
    }

    assets.drawSprite(SpriteID::LeftPaddle, pos.x, pos.y, scaleX, scaleY, 0xFFFFFFFF);
}

void Renderer::drawBallSprite(const Position &pos, const Size &size)
{
    AssetManager &assets = AssetManager::getInstance();

    SpriteInfo ballSprite = assets.getSprite(SpriteID::Ball);
    if (ballSprite.width == 0)
        return;

    float scaleX = size.width / (float)ballSprite.width;
    float scaleY = size.height / (float)ballSprite.height;

    // Center the ball sprite
    int drawX = pos.x - (ballSprite.width * scaleX) / 2;
    int drawY = pos.y - (ballSprite.height * scaleY) / 2;

    assets.drawSprite(SpriteID::Ball, drawX, drawY, scaleX, scaleY, 0xFFFFFFFF);
}

void Renderer::drawCourtSprite()
{
    AssetManager &assets = AssetManager::getInstance();

    // Draw background court area
    SpriteInfo bgSprite = assets.getSprite(SpriteID::Background);
    if (bgSprite.width > 0)
    {
        float scaleX = 640.0f / bgSprite.width;
        float scaleY = 480.0f / bgSprite.height;
        assets.drawSprite(SpriteID::Background, 0, 0, scaleX, scaleY, 0xFFFFFFFF);
    }

    // Draw center line
    SpriteInfo centerSprite = assets.getSprite(SpriteID::CenterLine);
    if (centerSprite.width > 0)
    {
        int centerX = 320 - (centerSprite.width / 2);
        int centerY = 150; // Position it vertically centered in playing area
        assets.drawSprite(SpriteID::CenterLine, centerX, centerY, 1.0f, 1.0f, 0xFFFFFFFF);
    }
}

void Renderer::drawScoreDigitSprite(int digit, int x, int y)
{
    if (digit < 0 || digit > 9)
        return;

    AssetManager &assets = AssetManager::getInstance();

    // Map digit to corresponding SpriteID
    SpriteID spriteId;
    switch (digit)
    {
    case 0:
        spriteId = SpriteID::Digit0;
        break;
    case 1:
        spriteId = SpriteID::Digit1;
        break;
    case 2:
        spriteId = SpriteID::Digit2;
        break;
    case 3:
        spriteId = SpriteID::Digit3;
        break;
    case 4:
        spriteId = SpriteID::Digit4;
        break;
    case 5:
        spriteId = SpriteID::Digit5;
        break;
    case 6:
        spriteId = SpriteID::Digit6;
        break;
    case 7:
        spriteId = SpriteID::Digit7;
        break;
    case 8:
        spriteId = SpriteID::Digit8;
        break;
    case 9:
        spriteId = SpriteID::Digit9;
        break;
    default:
        return;
    }

    assets.drawSprite(spriteId, x, y, 1.0f, 1.0f, 0xFFFFFFFF);
}

void Renderer::drawIntenseEffects()
{
    // Add pulsing border effect for intense mode
    static int pulseCounter = 0;
    pulseCounter++;

    // Create pulsing cyan border
    u8 intensity = 0x88 + (sin(pulseCounter * 0.1f) + 1.0f) * 0x40;
    u32 pulseColor = (intensity << 16) | (0xFF << 8) | 0xFFFF; // Cyan pulsing

    // Top and bottom borders
    GRRLIB_Rectangle(0, 0, 640, 8, pulseColor, true);
    GRRLIB_Rectangle(0, 472, 640, 8, pulseColor, true);

    // Side borders
    GRRLIB_Rectangle(0, 0, 8, 480, pulseColor, true);
    GRRLIB_Rectangle(632, 0, 8, 480, pulseColor, true);

    // Add corner glow effects
    int glowSize = 30 + sin(pulseCounter * 0.15f) * 10;
    u32 glowColor = (0x44 << 24) | (0xFF << 16) | (0xFF << 8) | 0xFF; // Semi-transparent cyan

    // Corner glows
    GRRLIB_Rectangle(0, 0, glowSize, glowSize, glowColor, true);
    GRRLIB_Rectangle(640 - glowSize, 0, glowSize, glowSize, glowColor, true);
    GRRLIB_Rectangle(0, 480 - glowSize, glowSize, glowSize, glowColor, true);
    GRRLIB_Rectangle(640 - glowSize, 480 - glowSize, glowSize, glowSize, glowColor, true);

    // Add center line glow effect
    u32 centerGlow = (0x66 << 24) | (0x00 << 16) | (0xFF << 8) | 0xFF; // Semi-transparent cyan
    GRRLIB_Rectangle(316, 0, 8, 480, centerGlow, true);
}

// Shadow and Trail Effect System Implementation

void Renderer::setupEffectConfigurations()
{
    // Normal atlas effects - subtle and elegant
    normalEffects.shadowOffsetX = 3.0f;
    normalEffects.shadowOffsetY = 3.0f;
    normalEffects.shadowOpacity = 0.4f;
    normalEffects.shadowColor = 0x000000FF; // Black shadow
    normalEffects.maxTrailLength = 4;
    normalEffects.trailFadeRate = 0.25f;
    normalEffects.trailSpacing = 0.7f;
    normalEffects.enableTrails = true;
    normalEffects.intensityBasedEffects = false;
    normalEffects.glowIntensity = 0.0f;

    // Intense atlas effects - dramatic and dynamic
    intenseEffects.shadowOffsetX = 5.0f;
    intenseEffects.shadowOffsetY = 4.0f;
    intenseEffects.shadowOpacity = 0.6f;
    intenseEffects.shadowColor = 0x004444FF; // Dark cyan shadow
    intenseEffects.maxTrailLength = 6;
    intenseEffects.trailFadeRate = 0.2f;
    intenseEffects.trailSpacing = 0.5f;
    intenseEffects.enableTrails = true;
    intenseEffects.intensityBasedEffects = true;
    intenseEffects.glowIntensity = 0.3f;
}

EffectConfig Renderer::getCurrentEffectConfig(AtlasType currentAtlas) const
{
    return (currentAtlas == AtlasType::Intense) ? intenseEffects : normalEffects;
}

void Renderer::updateBallTrail(const Position &ballPos, const Velocity &ballVel)
{
    // Calculate ball speed to determine if we should add trail points
    float speed = sqrt(ballVel.dx * ballVel.dx + ballVel.dy * ballVel.dy);

    // Only add trail points when ball is moving significantly
    if (speed > 5.0f)
    {
        // Add current position to trail
        ballTrail.insert(ballTrail.begin(), PositionHistory(ballPos.x, ballPos.y, currentTime));

        // Limit trail length based on current effect config
        EffectConfig config = getCurrentEffectConfig(AssetManager::getInstance().getCurrentAtlas());
        if ((int)ballTrail.size() > config.maxTrailLength)
        {
            ballTrail.resize(config.maxTrailLength);
        }
    }

    // Remove old trail points (older than 1 second)
    ballTrail.erase(
        std::remove_if(ballTrail.begin(), ballTrail.end(),
                       [this](const PositionHistory &point)
                       {
                           return (currentTime - point.timestamp) > 1.0f;
                       }),
        ballTrail.end());
}

void Renderer::drawPaddleShadow(const Position &pos, const Size &size, bool isLeftPaddle, const EffectConfig &config)
{
    AssetManager &assets = AssetManager::getInstance();

    // Calculate shadow offset with slight variation for realism
    float shadowX = pos.x + config.shadowOffsetX;
    float shadowY = pos.y + config.shadowOffsetY;

    // Add subtle movement to shadow in intense mode
    if (config.intensityBasedEffects)
    {
        shadowX += sin(currentTime * 3.0f) * 1.0f;
        shadowY += cos(currentTime * 2.5f) * 0.5f;
    }

    // Calculate shadow alpha
    u8 shadowAlpha = (u8)(config.shadowOpacity * 255.0f);
    u32 shadowColor = (shadowAlpha << 24) | (config.shadowColor & 0x00FFFFFF);

    // Calculate scale to fit the paddle size
    SpriteInfo paddleSprite = assets.getSprite(SpriteID::LeftPaddle);
    if (paddleSprite.width == 0)
        return;

    float scaleX = size.width / (float)paddleSprite.width;
    float scaleY = size.height / (float)paddleSprite.height;

    // For right paddle, flip horizontally
    if (!isLeftPaddle)
    {
        scaleX = -scaleX;
    }

    // Draw shadow sprite
    assets.drawSprite(SpriteID::LeftPaddle, (int)shadowX, (int)shadowY, scaleX, scaleY, shadowColor);
}

void Renderer::drawBallShadow(const Position &pos, const Size &size, const EffectConfig &config)
{
    AssetManager &assets = AssetManager::getInstance();

    // Calculate shadow offset
    float shadowX = pos.x + config.shadowOffsetX;
    float shadowY = pos.y + config.shadowOffsetY;

    // Add dynamic movement in intense mode
    if (config.intensityBasedEffects)
    {
        shadowX += sin(currentTime * 4.0f) * 1.5f;
        shadowY += cos(currentTime * 3.0f) * 1.0f;
    }

    // Calculate shadow alpha
    u8 shadowAlpha = (u8)(config.shadowOpacity * 255.0f);
    u32 shadowColor = (shadowAlpha << 24) | (config.shadowColor & 0x00FFFFFF);

    SpriteInfo ballSprite = assets.getSprite(SpriteID::Ball);
    if (ballSprite.width == 0)
        return;

    float scaleX = size.width / (float)ballSprite.width;
    float scaleY = size.height / (float)ballSprite.height;

    // Center the shadow sprite
    int drawX = (int)(shadowX - (ballSprite.width * scaleX) / 2);
    int drawY = (int)(shadowY - (ballSprite.height * scaleY) / 2);

    // Draw shadow sprite
    assets.drawSprite(SpriteID::Ball, drawX, drawY, scaleX, scaleY, shadowColor);
}

void Renderer::drawBallTrail(const Size &ballSize, const EffectConfig &config)
{
    if (ballTrail.empty())
        return;

    AssetManager &assets = AssetManager::getInstance();
    SpriteInfo ballSprite = assets.getSprite(SpriteID::Ball);
    if (ballSprite.width == 0)
        return;

    float scaleX = ballSize.width / (float)ballSprite.width;
    float scaleY = ballSize.height / (float)ballSprite.height;

    // Draw trail points from oldest to newest (back to front)
    for (int i = (int)ballTrail.size() - 1; i >= 0; i--)
    {
        const PositionHistory &point = ballTrail[i];

        // Calculate alpha based on position in trail and time
        float ageRatio = (float)i / (float)ballTrail.size();
        float timeRatio = (currentTime - point.timestamp);

        // Base alpha decreases with distance from current position
        float alpha = 1.0f - (ageRatio * config.trailFadeRate);

        // Also fade based on time
        alpha *= (1.0f - (timeRatio * 0.5f));
        alpha = fmax(alpha, 0.1f); // Minimum visibility

        // Calculate trail color
        u32 trailColor;
        if (config.intensityBasedEffects)
        {
            // Cyan trail for intense mode
            u8 alphaValue = (u8)(alpha * 255.0f);
            trailColor = (alphaValue << 24) | (0x44 << 16) | (0xFF << 8) | 0xFF;
        }
        else
        {
            // White/normal trail for regular mode
            u8 alphaValue = (u8)(alpha * 255.0f);
            trailColor = (alphaValue << 24) | (0xFF << 16) | (0xFF << 8) | 0xFF;
        }

        // Calculate scale reduction for trail (smaller towards the back)
        float trailScale = 0.7f + (ageRatio * 0.3f); // Scale from 0.7 to 1.0

        // Center the trail sprite
        int drawX = (int)(point.x - (ballSprite.width * scaleX * trailScale) / 2);
        int drawY = (int)(point.y - (ballSprite.height * scaleY * trailScale) / 2);

        // Draw trail sprite
        assets.drawSprite(SpriteID::Ball, drawX, drawY,
                          scaleX * trailScale, scaleY * trailScale, trailColor);
    }
}

void Renderer::drawGlowEffect(const Position &pos, const Size &size, const EffectConfig &config, u32 glowColor)
{
    if (config.glowIntensity <= 0.0f)
        return;

    AssetManager &assets = AssetManager::getInstance();
    SpriteInfo ballSprite = assets.getSprite(SpriteID::Ball);
    if (ballSprite.width == 0)
        return;

    // Calculate pulsing glow intensity
    float pulseIntensity = (sin(currentTime * 8.0f) + 1.0f) * 0.5f; // 0 to 1
    float currentIntensity = config.glowIntensity * (0.5f + pulseIntensity * 0.5f);

    float scaleX = size.width / (float)ballSprite.width;
    float scaleY = size.height / (float)ballSprite.height;

    // Draw multiple glow layers for better effect
    for (int layer = 0; layer < 3; layer++)
    {
        float layerScale = 1.0f + (layer * 0.3f);          // Increasingly larger
        float layerAlpha = currentIntensity / (layer + 1); // Decreasing intensity

        u8 layerAlphaValue = (u8)(layerAlpha * 255.0f);
        u32 layerColor = (layerAlphaValue << 24) | (glowColor & 0x00FFFFFF);

        int drawX = (int)(pos.x - (ballSprite.width * scaleX * layerScale) / 2);
        int drawY = (int)(pos.y - (ballSprite.height * scaleY * layerScale) / 2);

        assets.drawSprite(SpriteID::Ball, drawX, drawY,
                          scaleX * layerScale, scaleY * layerScale, layerColor);
    }
}

// Animation system implementation

void Renderer::updateParticles(float deltaTime)
{
    // Update existing particles
    for (auto it = particles.begin(); it != particles.end();)
    {
        Particle &p = *it;
        p.life -= deltaTime;

        if (p.life <= 0.0f)
        {
            it = particles.erase(it);
        }
        else
        {
            // Update position
            p.x += p.vx * deltaTime;
            p.y += p.vy * deltaTime;

            // Update rotation
            p.rotation += 180.0f * deltaTime; // Rotate 180 degrees per second

            // Fade out over time
            float lifeRatio = p.life / p.maxLife;
            p.color = (p.color & 0x00FFFFFF) | ((u8)(lifeRatio * 255.0f) << 24);

            ++it;
        }
    }
}

void Renderer::updateCameraShake(float deltaTime)
{
    if (cameraShake.timeLeft > 0.0f)
    {
        cameraShake.timeLeft -= deltaTime;

        if (cameraShake.timeLeft <= 0.0f)
        {
            // Shake finished
            cameraShake.offsetX = 0.0f;
            cameraShake.offsetY = 0.0f;
        }
        else
        {
            // Calculate shake offset
            float time = currentTime * cameraShake.frequency;
            cameraShake.offsetX = sin(time) * cameraShake.intensity;
            cameraShake.offsetY = cos(time * 1.5f) * cameraShake.intensity;
        }
    }
}

void Renderer::updateFadeTransition(float deltaTime)
{
    if (fadeTransition.timeLeft > 0.0f)
    {
        fadeTransition.timeLeft -= deltaTime;

        float progress = 1.0f - (fadeTransition.timeLeft / fadeTransition.duration);

        switch (fadeTransition.type)
        {
        case FadeTransition::FadeIn:
            fadeTransition.alpha = 1.0f - progress;
            break;
        case FadeTransition::FadeOut:
            fadeTransition.alpha = progress;
            break;
        case FadeTransition::FadeInOut:
            if (progress < 0.5f)
                fadeTransition.alpha = progress * 2.0f;
            else
                fadeTransition.alpha = (1.0f - progress) * 2.0f;
            break;
        case FadeTransition::None:
            fadeTransition.alpha = 0.0f;
            break;
        }

        if (fadeTransition.timeLeft <= 0.0f)
        {
            fadeTransition.type = FadeTransition::None;
            fadeTransition.alpha = 0.0f;
        }
    }
}

void Renderer::updateUIAnimations(float deltaTime)
{
    for (auto it = uiAnimations.begin(); it != uiAnimations.end();)
    {
        UIAnimation &anim = *it;
        anim.timeLeft -= deltaTime;

        if (anim.timeLeft <= 0.0f)
        {
            it = uiAnimations.erase(it);
        }
        else
        {
            float progress = 1.0f - (anim.timeLeft / anim.duration);

            switch (anim.type)
            {
            case UIAnimation::Slide:
                anim.currentValue = anim.startValue + (progress * (anim.endValue - anim.startValue));
                break;
            case UIAnimation::Scale:
                anim.currentValue = anim.startValue + (progress * (anim.endValue - anim.startValue));
                break;
            case UIAnimation::Rotate:
                anim.currentValue = anim.startValue + (progress * (anim.endValue - anim.startValue));
                break;
            case UIAnimation::Pulse:
                anim.currentValue = anim.startValue + sin(progress * 3.14159f * 2.0f) * anim.endValue;
                break;
            }

            ++it;
        }
    }
}

// Public API implementations

void Renderer::startScreenFadeIn(float duration, u32 color)
{
    fadeTransition.startFadeIn(duration, color);
}

void Renderer::startScreenFadeOut(float duration, u32 color)
{
    fadeTransition.startFadeOut(duration, color);
}

void Renderer::startScreenFadeInOut(float duration, u32 color)
{
    fadeTransition.startFadeInOut(duration, color);
}

void Renderer::triggerCameraShake(float intensity, float duration)
{
    cameraShake.start(intensity, duration);
}

void Renderer::spawnImpactParticles(float x, float y, u32 color, int count)
{
    for (int i = 0; i < count; i++)
    {
        Particle p;
        p.x = x;
        p.y = y;

        // Random velocity in circle
        float angle = (float)(rand() % 360) * 3.14159f / 180.0f;
        float speed = 50.0f + (rand() % 100); // 50-150 pixels per second
        p.vx = cos(angle) * speed;
        p.vy = sin(angle) * speed;

        p.life = 0.5f + (float)(rand() % 50) / 100.0f; // 0.5-1.0 seconds
        p.maxLife = p.life;
        p.size = 2.0f + (float)(rand() % 4); // 2-6 pixels
        p.rotation = (float)(rand() % 360);
        p.color = color;

        particles.push_back(p);
    }
}

bool Renderer::isTransitionActive() const
{
    return fadeTransition.isActive() || cameraShake.isActive() || !particles.empty() || !uiAnimations.empty();
}

void Renderer::resetAllAnimations()
{
    particles.clear();
    uiAnimations.clear();
    cameraShake = CameraShake();
    fadeTransition = FadeTransition();
}

// Rendering methods for animation system

void Renderer::renderParticles()
{
    for (const Particle &p : particles)
    {
        // Draw simple colored rectangles for particles
        GRRLIB_Rectangle((int)(p.x - p.size / 2), (int)(p.y - p.size / 2),
                         (int)p.size, (int)p.size, p.color, true);
    }
}

void Renderer::renderFadeTransition()
{
    if (fadeTransition.isActive())
    {
        u8 alpha = (u8)(fadeTransition.getCurrentAlpha() * 255.0f);
        u32 fadeColor = (alpha << 24) | (fadeTransition.color & 0x00FFFFFF);

        // Draw full screen overlay
        GRRLIB_Rectangle(0, 0, 640, 480, fadeColor, true);
    }
}

// Structure method implementations

void Particle::update(float deltaTime)
{
    // Update position
    x += vx * deltaTime;
    y += vy * deltaTime;

    // Update life
    life -= deltaTime;

    // Fade color based on life remaining
    if (maxLife > 0.0f)
    {
        float lifeRatio = life / maxLife;
        color = (color & 0x00FFFFFF) | ((u8)(lifeRatio * 255.0f) << 24);
    }
}

void CameraShake::start(float newIntensity, float newDuration)
{
    intensity = newIntensity;
    duration = newDuration;
    timeLeft = newDuration;
    offsetX = 0.0f;
    offsetY = 0.0f;
}

void CameraShake::update(float deltaTime)
{
    if (timeLeft > 0.0f)
    {
        timeLeft -= deltaTime;

        if (timeLeft <= 0.0f)
        {
            offsetX = 0.0f;
            offsetY = 0.0f;
        }
        else
        {
            // Calculate shake offset using time
            float time = (duration - timeLeft) * frequency;
            offsetX = sin(time) * intensity;
            offsetY = cos(time * 1.5f) * intensity;
        }
    }
}

void FadeTransition::startFadeIn(float newDuration, u32 fadeColor)
{
    type = FadeIn;
    duration = newDuration;
    timeLeft = newDuration;
    color = fadeColor;
    alpha = 1.0f;
}

void FadeTransition::startFadeOut(float newDuration, u32 fadeColor)
{
    type = FadeOut;
    duration = newDuration;
    timeLeft = newDuration;
    color = fadeColor;
    alpha = 0.0f;
}

void FadeTransition::startFadeInOut(float newDuration, u32 fadeColor)
{
    type = FadeInOut;
    duration = newDuration;
    timeLeft = newDuration;
    color = fadeColor;
    alpha = 0.0f;
}

void FadeTransition::update(float deltaTime)
{
    if (timeLeft > 0.0f)
    {
        timeLeft -= deltaTime;

        float progress = 1.0f - (timeLeft / duration);

        switch (type)
        {
        case FadeIn:
            alpha = 1.0f - progress;
            break;
        case FadeOut:
            alpha = progress;
            break;
        case FadeInOut:
            if (progress < 0.5f)
                alpha = progress * 2.0f;
            else
                alpha = (1.0f - progress) * 2.0f;
            break;
        case None:
            alpha = 0.0f;
            break;
        }

        if (timeLeft <= 0.0f)
        {
            type = None;
            alpha = 0.0f;
        }
    }
}

float FadeTransition::getCurrentAlpha() const
{
    return alpha;
}