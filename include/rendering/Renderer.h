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

// Particle for impact effects
struct Particle
{
    float x, y;     // Current position
    float vx, vy;   // Velocity
    float life;     // Remaining life (0.0 to 1.0)
    float maxLife;  // Maximum life duration
    float size;     // Current size
    float rotation; // Current rotation
    u32 color;      // Current color

    Particle() : x(0), y(0), vx(0), vy(0), life(0), maxLife(1.0f),
                 size(1.0f), rotation(0), color(0xFFFFFFFF) {}

    bool isAlive() const { return life > 0.0f; }
    void update(float deltaTime);
};

// Camera shake effect
struct CameraShake
{
    float intensity;        // Shake intensity
    float duration;         // Total duration
    float timeLeft;         // Time remaining
    float frequency;        // Shake frequency
    float offsetX, offsetY; // Current offset

    CameraShake() : intensity(0), duration(0), timeLeft(0), frequency(15.0f),
                    offsetX(0), offsetY(0) {}

    void start(float newIntensity, float newDuration);
    void update(float deltaTime);
    bool isActive() const { return timeLeft > 0.0f; }
};

// Fade transition effect
struct FadeTransition
{
    enum FadeType
    {
        None,
        FadeIn,
        FadeOut,
        FadeInOut
    };

    FadeType type;
    float duration; // Total duration
    float timeLeft; // Time remaining
    float alpha;    // Current alpha value
    u32 color;      // Fade color

    FadeTransition() : type(None), duration(0), timeLeft(0), alpha(0), color(0x000000FF) {}

    void startFadeIn(float newDuration, u32 fadeColor = 0x000000FF);
    void startFadeOut(float newDuration, u32 fadeColor = 0x000000FF);
    void startFadeInOut(float newDuration, u32 fadeColor = 0x000000FF);
    void update(float deltaTime);
    bool isActive() const { return type != None && timeLeft > 0.0f; }
    float getCurrentAlpha() const;
};

// UI Animation system
struct UIAnimation
{
    enum AnimType
    {
        Slide,
        Scale,
        Rotate,
        Pulse
    };

    AnimType type;
    float duration;
    float timeLeft;
    float startValue;
    float endValue;
    float currentValue;
    bool looping;

    UIAnimation() : type(Slide), duration(0), timeLeft(0), startValue(0),
                    endValue(0), currentValue(0), looping(false) {}

    void start(AnimType animType, float animDuration, float start, float end, bool loop = false);
    void update(float deltaTime);
    bool isActive() const { return timeLeft > 0.0f; }
    float getValue() const { return currentValue; }
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

    // Animation and transition system public API
    void startScreenFadeIn(float duration = 1.0f, u32 color = 0x000000FF);
    void startScreenFadeOut(float duration = 1.0f, u32 color = 0x000000FF);
    void startScreenFadeInOut(float duration = 2.0f, u32 color = 0x000000FF);
    void triggerCameraShake(float intensity = 5.0f, float duration = 0.5f);
    void spawnImpactParticles(float x, float y, u32 color = 0xFFFFFFFF, int count = 5);
    bool isTransitionActive() const;
    void resetAllAnimations();

    // Performance monitoring API
    float getCurrentFPS() const { return averageFPS; }
    int getActiveParticleCount() const { return activeParticles; }
    bool areBackgroundsPrerendered() const { return backgroundsPrerendered; }

private:
    bool initialized;
    bool debugVisible;                   // Control debug visibility
    GRRLIB_texImg *font;                 // For future text; placeholder for now
    BackgroundManager backgroundManager; // Parallax background system

    // Shadow and trail effect system
    std::vector<PositionHistory> ballTrail; // Ball position history for trails
    EffectConfig normalEffects;             // Effects config for normal atlas
    EffectConfig intenseEffects;            // Effects config for intense atlas
    float currentTime;                      // Current time for effect calculations

    // Animation system variables - optimized for Wii performance
    static const int MAX_PARTICLES = 32;   // Fixed pool size to avoid malloc
    Particle particlePool[MAX_PARTICLES];  // Pre-allocated particle pool
    int activeParticles;                   // Count of active particles
    CameraShake cameraShake;               // Current camera shake effect
    FadeTransition fadeTransition;         // Current fade transition
    std::vector<UIAnimation> uiAnimations; // Active UI animations

    // Performance optimization variables
    GRRLIB_texImg *prerenderedBackground[2]; // Pre-rendered backgrounds [Normal/Intense]
    bool backgroundsPrerendered;             // Flag to check if backgrounds are ready
    float lastFrameTime;                     // For FPS calculation
    int frameCount;                          // Frame counter for profiling
    float averageFPS;                        // Running average FPS

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

    // Animation system private methods
    void updateParticles(float deltaTime);
    void updateCameraShake(float deltaTime);
    void updateFadeTransition(float deltaTime);
    void updateUIAnimations(float deltaTime);
    void renderParticles();
    void renderFadeTransition();

    // Performance optimization methods
    void prerenderBackgrounds();            // Pre-render background layers to textures
    void updateFPSCounter(float deltaTime); // Calculate and track FPS
    void spawnParticleFromPool(float x, float y, float vx, float vy,
                               float life, float size, u32 color); // Use particle pool
    void clearDeadParticles();                                     // Efficiently remove dead particles
    int findFreeParticleSlot();                                    // Find available slot in pool

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