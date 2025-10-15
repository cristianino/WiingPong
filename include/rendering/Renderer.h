#pragma once

#include <grrlib.h>
#include <gctypes.h>
#include <vector>
#include "../physics/PhysicsEngine.h"
#include "../physics/Components.h"
#include "../input/InputManager.h"
#include "../gamestate/GameStateManager.h"
#include "../assets/AssetManager.h"
#include "../background/BackgroundManager.h"
#include "../config.h"

// Structure to store position history for trail effects
struct PositionHistory
{
    float x, y;
    float timestamp;
    
    PositionHistory() : x(0), y(0), timestamp(0) {}
    PositionHistory(float px, float py, float t) : x(px), y(py), timestamp(t) {}
};

// Configuration for shadow and trail effects
struct EffectConfig
{
    // Shadow settings
    float shadowOffsetX;
    float shadowOffsetY;
    float shadowOpacity;
    u32 shadowColor;
    
    // Trail settings
    int maxTrailLength;
    float trailFadeRate;
    float trailSpacing;
    bool enableTrails;
    
    // Dynamic effects
    bool intensityBasedEffects;
    float glowIntensity;
    
    EffectConfig() : shadowOffsetX(3.0f), shadowOffsetY(3.0f), shadowOpacity(0.4f), 
                     shadowColor(0x000000FF), maxTrailLength(5), trailFadeRate(0.3f),
                     trailSpacing(0.5f), enableTrails(true), intensityBasedEffects(true),
                     glowIntensity(0.0f) {}
};

// Renderer class for drawing game elements using GRRLIB
class Renderer
{
public:
    Renderer();
    ~Renderer();

    void init();
    void update(float deltaTime, const PhysicsEngine &physics); // Update animations
    void render(const PhysicsEngine &physics);                  // Draw from physics components
    void renderMenu(const GameStateManager &gameState);         // Draw menu screen
    void renderDebugInfo(const InputManager &input);            // Debug overlay
    void renderDebugSensorInfo(const InputManager &input);      // Sensor debug overlay
    void renderDebugToggleProgress(const InputManager &input);  // Debug toggle progress indicator (A+B or PLUS+MINUS)
    void setDebugVisible(bool visible) { debugVisible = visible; }
    bool isDebugVisible() const { return debugVisible; }

private:
    bool initialized;
    bool debugVisible;                   // Control debug visibility
    GRRLIB_texImg *font;                 // For future text; placeholder for now
    BackgroundManager backgroundManager; // Parallax background system
    
    // Shadow and trail effect system
    std::vector<PositionHistory> ballTrail;  // Ball position history for trails
    EffectConfig normalEffects;              // Effects config for normal atlas
    EffectConfig intenseEffects;             // Effects config for intense atlas
    float currentTime;                       // Current time for effect calculations

    void drawPaddle(const Position &pos, const Size &size, u32 color);
    void drawBall(const Position &pos, const Size &size, u32 color);
    void drawCourt();
    void drawScores(const PhysicsEngine &physics);
    void drawText(const char *text, int x, int y, u32 color);
    void drawMenuItem(const char *text, int x, int y, bool selected);
    void drawMenuBackground();
    void drawIntenseEffects(); // Special effects for intense atlas mode

    // New sprite-based drawing methods
    void drawPaddleSprite(const Position &pos, const Size &size, bool isLeftPaddle);
    void drawBallSprite(const Position &pos, const Size &size);
    void drawCourtSprite();
    void drawScoreDigitSprite(int digit, int x, int y);
    
    // Shadow and trail effect methods
    void updateBallTrail(const Position &ballPos, const Velocity &ballVel);
    void drawPaddleShadow(const Position &pos, const Size &size, bool isLeftPaddle, const EffectConfig &config);
    void drawBallShadow(const Position &pos, const Size &size, const EffectConfig &config);
    void drawBallTrail(const Size &ballSize, const EffectConfig &config);
    void drawGlowEffect(const Position &pos, const Size &size, const EffectConfig &config, u32 glowColor);
    EffectConfig getCurrentEffectConfig(AtlasType currentAtlas) const;
    void setupEffectConfigurations();

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

    // Sensor visualization functions
    void drawAccelerometerGraph(int x, int y, int width, int height, const vec3w_t &accel);
    void drawOrientationIndicator(int x, int y, int size, const orient_t &orient);
    void drawGForceIndicator(int x, int y, int size, const gforce_t &gforce);
    void drawIRDots(int x, int y, int width, int height, const ir_t &ir);
    void drawSensorBackground(int x, int y, int width, int height, const char *title);
};