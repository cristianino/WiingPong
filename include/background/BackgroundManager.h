#pragma once

#include <grrlib.h>
#include <map>
#include <vector>
#include "assets/AssetManager.h"

// Forward declaration
class PhysicsEngine;

class BackgroundManager
{
public:
    BackgroundManager();
    ~BackgroundManager();

    void init();
    void update(float deltaTime, const PhysicsEngine &physics); // Update animations and effects
    void render(AtlasType currentAtlas) const;                  // Render all background layers

    // Configuration methods
    void setupNormalAtlasLayers();                              // Setup layers for normal atlas
    void setupIntenseAtlasLayers();                             // Setup layers for intense atlas
    void setGameplayEffects(bool ballMoving, bool recentScore); // Add gameplay-reactive effects

    // Animation control
    void reset();                             // Reset all animations
    void setAnimationSpeed(float multiplier); // Speed up/slow down animations

private:
    bool initialized;
    float frameCounter;             // Global frame counter for animations
    float animationSpeedMultiplier; // Global animation speed control

    // Gameplay reactive variables
    bool ballIsMoving;
    bool recentScoreEvent;
    float scoreEventTimer;

    // Layer configurations for different atlas types
    std::map<AtlasType, std::vector<ParallaxLayer>> layerConfigs;

    // Internal rendering methods
    void renderLayer(const ParallaxLayer &layer, BackgroundLayer layerType, AtlasType currentAtlas) const;
    float calculateLayerOffset(const ParallaxLayer &layer, BackgroundLayer layerType) const;
    u32 calculateLayerColor(const ParallaxLayer &layer, AtlasType currentAtlas) const;

    // Effect calculation methods
    float calculateVibration(float intensity, float time) const;
    float calculatePulse(float speed, float time) const;
    void updateGameplayEffects(const PhysicsEngine &physics);
};