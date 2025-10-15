#pragma once

#include <grrlib.h>
#include <fat.h>
#include "audio/AudioManager.h"
#include <map>
#include <string>

// Forward declaration
class WiimoteManager;

// Structure to define a sprite within a spritesheet
struct SpriteInfo
{
    int x, y;          // Position in spritesheet
    int width, height; // Dimensions of the sprite

    SpriteInfo() : x(0), y(0), width(0), height(0) {}
    SpriteInfo(int x, int y, int w, int h) : x(x), y(y), width(w), height(h) {}
};

// Enum for different sprites in our spritesheet
enum class SpriteID
{
    LeftPaddle,
    RightPaddle,
    Ball,
    Digit0,
    Digit1,
    Digit2,
    Digit3,
    Digit4,
    Digit5,
    Digit6,
    Digit7,
    Digit8,
    Digit9,
    CenterLine,
    Background
};

// Enum for different atlas types
enum class AtlasType
{
    Normal, // Regular atlas (atlas.png or spritesheet.png)
    Intense // Intense/tied game atlas (atlas_2.png)
};

// Enum for background layer types
enum class BackgroundLayer
{
    FarBackground,  // Slowest moving layer (distant background)
    MidBackground,  // Medium speed layer (middle ground)
    NearBackground, // Fastest moving layer (close to playing field)
    PlayingField    // The main court/field (static or minimal movement)
};

// Structure for parallax layer configuration
struct ParallaxLayer
{
    SpriteID spriteId;      // Which sprite to use for this layer
    float scrollSpeedX;     // Horizontal scroll speed multiplier
    float scrollSpeedY;     // Vertical scroll speed multiplier
    float opacity;          // Layer opacity (0.0 to 1.0)
    bool wrapHorizontal;    // Whether to wrap horizontally
    bool wrapVertical;      // Whether to wrap vertically
    float pulseSpeed;       // Speed of pulsing effect (0 = no pulse)
    float vibrateIntensity; // Intensity of vibration effect (0 = no vibration)

    ParallaxLayer() : spriteId(SpriteID::Background), scrollSpeedX(0.0f), scrollSpeedY(0.0f),
                      opacity(1.0f), wrapHorizontal(false), wrapVertical(false),
                      pulseSpeed(0.0f), vibrateIntensity(0.0f) {}
};

// Simple singleton for asset management
// Load textures, sounds, fonts from SD card via fat.h

class AssetManager
{
public:
    static AssetManager &getInstance();

    void init();
    void loadAllAssets();                                  // Preload game assets
    void loadTextures();                                   // Load texture assets
    void loadSpritesheet();                                // Load the spritesheet and define sprites
    void loadMultipleAtlases();                            // Load multiple atlas textures
    void loadAudio(AudioManager &audioManager);            // Load audio assets
    void loadWiimoteAudio(WiimoteManager &wiimoteManager); // Load Wiimote audio assets

    // Atlas management methods
    void setCurrentAtlas(AtlasType atlasType);                              // Switch between atlas types
    AtlasType getCurrentAtlas() const;                                      // Get current atlas type
    AtlasType determineAtlasForScores(int playerScore, int cpuScore) const; // Determine atlas based on game state

    // Stubs for getters; return nullptr or default
    GRRLIB_texImg *getTexture(const char *name) const;
    GRRLIB_texImg *getSpritesheet() const;              // Get the current active spritesheet texture
    GRRLIB_texImg *getAtlas(AtlasType atlasType) const; // Get specific atlas texture
    SpriteInfo getSprite(SpriteID spriteId) const;      // Get sprite coordinates
    void drawSprite(SpriteID spriteId, int x, int y, float scaleX = 1.0f, float scaleY = 1.0f, u32 color = 0xFFFFFFFF) const;
    void *getSound(const char *name) const; // For future audio
    GRRLIB_texImg *getFont(const char *name) const;

    ~AssetManager();

private:
    AssetManager(); // Private for singleton
    AssetManager(const AssetManager &) = delete;
    AssetManager &operator=(const AssetManager &) = delete;

    void initializeSpriteDefinitions(); // Initialize sprite coordinates

    bool initialized;
    std::map<std::string, GRRLIB_texImg *> textures; // Store loaded textures
    GRRLIB_texImg *spritesheet;                      // The main spritesheet texture (legacy support)
    std::map<SpriteID, SpriteInfo> sprites;          // Sprite definitions

    // Multiple atlas support
    std::map<AtlasType, GRRLIB_texImg *> atlases; // Store multiple atlas textures
    AtlasType currentAtlasType;                   // Currently active atlas
};