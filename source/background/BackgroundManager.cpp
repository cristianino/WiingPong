#include "background/BackgroundManager.h"
#include "physics/PhysicsEngine.h"
#include <math.h>
#include <stdio.h>

BackgroundManager::BackgroundManager()
    : initialized(false), frameCounter(0.0f), animationSpeedMultiplier(1.0f),
      ballIsMoving(false), recentScoreEvent(false), scoreEventTimer(0.0f)
{
}

BackgroundManager::~BackgroundManager()
{
}

void BackgroundManager::init()
{
    if (initialized)
        return;

    printf("Initializing BackgroundManager...\n");

    // Setup layer configurations for both atlas types
    setupNormalAtlasLayers();
    setupIntenseAtlasLayers();

    initialized = true;
    printf("BackgroundManager initialized\n");
}

void BackgroundManager::setupNormalAtlasLayers()
{
    std::vector<ParallaxLayer> normalLayers;

    // Far background layer - very slow movement, subtle
    ParallaxLayer farLayer;
    farLayer.spriteId = SpriteID::Background;
    farLayer.scrollSpeedX = 0.1f;
    farLayer.scrollSpeedY = 0.05f;
    farLayer.opacity = 0.3f;
    farLayer.wrapHorizontal = true;
    farLayer.pulseSpeed = 0.02f;
    farLayer.vibrateIntensity = 0.0f;
    normalLayers.push_back(farLayer);

    // Mid background layer - medium movement
    ParallaxLayer midLayer;
    midLayer.spriteId = SpriteID::Background;
    midLayer.scrollSpeedX = 0.3f;
    midLayer.scrollSpeedY = 0.1f;
    midLayer.opacity = 0.5f;
    midLayer.wrapHorizontal = true;
    midLayer.pulseSpeed = 0.05f;
    midLayer.vibrateIntensity = 0.2f;
    normalLayers.push_back(midLayer);

    // Near background layer - faster movement, more visible
    ParallaxLayer nearLayer;
    nearLayer.spriteId = SpriteID::Background;
    nearLayer.scrollSpeedX = 0.8f;
    nearLayer.scrollSpeedY = 0.2f;
    nearLayer.opacity = 0.7f;
    nearLayer.wrapHorizontal = true;
    nearLayer.pulseSpeed = 0.1f;
    nearLayer.vibrateIntensity = 0.5f;
    normalLayers.push_back(nearLayer);

    // Playing field layer - minimal movement, main court
    ParallaxLayer fieldLayer;
    fieldLayer.spriteId = SpriteID::Background;
    fieldLayer.scrollSpeedX = 0.0f;
    fieldLayer.scrollSpeedY = 0.0f;
    fieldLayer.opacity = 1.0f;
    fieldLayer.wrapHorizontal = false;
    fieldLayer.pulseSpeed = 0.01f;
    fieldLayer.vibrateIntensity = 0.1f;
    normalLayers.push_back(fieldLayer);

    layerConfigs[AtlasType::Normal] = normalLayers;
}

void BackgroundManager::setupIntenseAtlasLayers()
{
    std::vector<ParallaxLayer> intenseLayers;

    // Far background layer - more movement for intense feel
    ParallaxLayer farLayer;
    farLayer.spriteId = SpriteID::Background;
    farLayer.scrollSpeedX = 0.3f;
    farLayer.scrollSpeedY = 0.15f;
    farLayer.opacity = 0.4f;
    farLayer.wrapHorizontal = true;
    farLayer.pulseSpeed = 0.08f;
    farLayer.vibrateIntensity = 0.3f;
    intenseLayers.push_back(farLayer);

    // Mid background layer - much more active
    ParallaxLayer midLayer;
    midLayer.spriteId = SpriteID::Background;
    midLayer.scrollSpeedX = 0.6f;
    midLayer.scrollSpeedY = 0.3f;
    midLayer.opacity = 0.6f;
    midLayer.wrapHorizontal = true;
    midLayer.pulseSpeed = 0.15f;
    midLayer.vibrateIntensity = 0.8f;
    intenseLayers.push_back(midLayer);

    // Near background layer - very active, cyberpunk feel
    ParallaxLayer nearLayer;
    nearLayer.spriteId = SpriteID::Background;
    nearLayer.scrollSpeedX = 1.2f;
    nearLayer.scrollSpeedY = 0.4f;
    nearLayer.opacity = 0.8f;
    nearLayer.wrapHorizontal = true;
    nearLayer.pulseSpeed = 0.25f;
    nearLayer.vibrateIntensity = 1.2f;
    intenseLayers.push_back(nearLayer);

    // Playing field layer - subtle but noticeable movement for intensity
    ParallaxLayer fieldLayer;
    fieldLayer.spriteId = SpriteID::Background;
    fieldLayer.scrollSpeedX = 0.05f;
    fieldLayer.scrollSpeedY = 0.02f;
    fieldLayer.opacity = 1.0f;
    fieldLayer.wrapHorizontal = false;
    fieldLayer.pulseSpeed = 0.05f;
    fieldLayer.vibrateIntensity = 0.3f;
    intenseLayers.push_back(fieldLayer);

    layerConfigs[AtlasType::Intense] = intenseLayers;
}

void BackgroundManager::update(float deltaTime, const PhysicsEngine &physics)
{
    if (!initialized)
        return;

    frameCounter += deltaTime * animationSpeedMultiplier;

    // Update gameplay-reactive effects
    updateGameplayEffects(physics);

    // Update score event timer
    if (recentScoreEvent)
    {
        scoreEventTimer -= deltaTime;
        if (scoreEventTimer <= 0.0f)
        {
            recentScoreEvent = false;
        }
    }
}

void BackgroundManager::updateGameplayEffects(const PhysicsEngine &physics)
{
    // Check if ball is moving (simple velocity check)
    float ballVelX = physics.velocities[BALL].dx;
    float ballVelY = physics.velocities[BALL].dy;
    float ballSpeed = sqrt(ballVelX * ballVelX + ballVelY * ballVelY);

    ballIsMoving = (ballSpeed > 10.0f); // Threshold for "moving"

    // Detect score events (this is a simple approximation)
    // In a real implementation, you'd want a proper event system
    static int lastPlayerScore = 0;
    static int lastCpuScore = 0;

    if (physics.playerScore != lastPlayerScore || physics.cpuScore != lastCpuScore)
    {
        recentScoreEvent = true;
        scoreEventTimer = 2.0f; // Effect lasts 2 seconds
        lastPlayerScore = physics.playerScore;
        lastCpuScore = physics.cpuScore;
    }
}

void BackgroundManager::render(AtlasType currentAtlas) const
{
    if (!initialized)
        return;

    auto layerIt = layerConfigs.find(currentAtlas);
    if (layerIt == layerConfigs.end())
        return;

    const std::vector<ParallaxLayer> &layers = layerIt->second;

    // Render layers from back to front
    BackgroundLayer layerTypes[] = {
        BackgroundLayer::FarBackground,
        BackgroundLayer::MidBackground,
        BackgroundLayer::NearBackground,
        BackgroundLayer::PlayingField};

    for (int i = 0; i < 4 && i < (int)layers.size(); i++)
    {
        renderLayer(layers[i], layerTypes[i], currentAtlas);
    }
}

void BackgroundManager::renderLayer(const ParallaxLayer &layer, BackgroundLayer layerType, AtlasType currentAtlas) const
{
    AssetManager &assets = AssetManager::getInstance();

    // Calculate layer offset with parallax effect
    float offsetX = calculateLayerOffset(layer, layerType);
    float offsetY = calculateLayerOffset(layer, layerType) * 0.3f; // Less vertical movement

    // Add vibration effect
    if (layer.vibrateIntensity > 0.0f)
    {
        offsetX += calculateVibration(layer.vibrateIntensity, frameCounter * 2.0f);
        offsetY += calculateVibration(layer.vibrateIntensity * 0.5f, frameCounter * 3.0f);
    }

    // Calculate layer color with pulsing effect
    u32 layerColor = calculateLayerColor(layer, currentAtlas);

    // Get sprite info
    SpriteInfo sprite = assets.getSprite(layer.spriteId);
    if (sprite.width == 0 || sprite.height == 0)
        return;

    // Render the layer (potentially multiple times for wrapping)
    int renderWidth = 640;
    int renderHeight = 480;

    if (layer.wrapHorizontal || layer.wrapVertical)
    {
        // Calculate how many times we need to tile
        int tilesX = layer.wrapHorizontal ? ((renderWidth / sprite.width) + 2) : 1;
        int tilesY = layer.wrapVertical ? ((renderHeight / sprite.height) + 2) : 1;

        for (int x = 0; x < tilesX; x++)
        {
            for (int y = 0; y < tilesY; y++)
            {
                float drawX = (x * sprite.width) + offsetX;
                float drawY = (y * sprite.height) + offsetY;

                // Wrap positions
                if (layer.wrapHorizontal)
                {
                    drawX = fmod(drawX + renderWidth, renderWidth) - sprite.width;
                }
                if (layer.wrapVertical)
                {
                    drawY = fmod(drawY + renderHeight, renderHeight) - sprite.height;
                }

                assets.drawSprite(layer.spriteId, (int)drawX, (int)drawY, 1.0f, 1.0f, layerColor);
            }
        }
    }
    else
    {
        // Single render
        assets.drawSprite(layer.spriteId, (int)offsetX, (int)offsetY, 1.0f, 1.0f, layerColor);
    }
}

float BackgroundManager::calculateLayerOffset(const ParallaxLayer &layer, BackgroundLayer layerType) const
{
    float baseOffset = frameCounter;

    // Apply layer-specific speed
    float offset = baseOffset * layer.scrollSpeedX;

    // Add gameplay-reactive effects
    if (ballIsMoving)
    {
        offset += sin(frameCounter * 0.5f) * 0.5f; // Slight wave when ball is moving
    }

    if (recentScoreEvent)
    {
        // Dramatic effect after scoring
        float eventIntensity = scoreEventTimer / 2.0f; // Fade out over 2 seconds
        offset += sin(frameCounter * 2.0f) * 3.0f * eventIntensity;
    }

    return offset;
}

u32 BackgroundManager::calculateLayerColor(const ParallaxLayer &layer, AtlasType currentAtlas) const
{
    // Base opacity
    u8 alpha = (u8)(layer.opacity * 255.0f);

    // Apply pulsing effect
    if (layer.pulseSpeed > 0.0f)
    {
        float pulse = calculatePulse(layer.pulseSpeed, frameCounter);
        alpha = (u8)(alpha * (0.7f + 0.3f * pulse)); // Pulse between 70% and 100%
    }

    // Different color tints for different atlas types
    if (currentAtlas == AtlasType::Intense)
    {
        // Cyan tint for intense mode
        return (alpha << 24) | (0xAA << 16) | (0xFF << 8) | 0xFF;
    }
    else
    {
        // White/normal for regular mode
        return (alpha << 24) | (0xFF << 16) | (0xFF << 8) | 0xFF;
    }
}

float BackgroundManager::calculateVibration(float intensity, float time) const
{
    return sin(time * 15.0f) * intensity * 0.5f; // High frequency, small amplitude
}

float BackgroundManager::calculatePulse(float speed, float time) const
{
    return (sin(time * speed) + 1.0f) * 0.5f; // Returns 0.0 to 1.0
}

void BackgroundManager::setGameplayEffects(bool ballMoving, bool recentScore)
{
    ballIsMoving = ballMoving;
    if (recentScore)
    {
        recentScoreEvent = true;
        scoreEventTimer = 2.0f;
    }
}

void BackgroundManager::reset()
{
    frameCounter = 0.0f;
    ballIsMoving = false;
    recentScoreEvent = false;
    scoreEventTimer = 0.0f;
}

void BackgroundManager::setAnimationSpeed(float multiplier)
{
    animationSpeedMultiplier = multiplier;
}