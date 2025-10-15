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
    void loadAudio(AudioManager &audioManager);            // Load audio assets
    void loadWiimoteAudio(WiimoteManager &wiimoteManager); // Load Wiimote audio assets

    // Stubs for getters; return nullptr or default
    GRRLIB_texImg *getTexture(const char *name) const;
    GRRLIB_texImg *getSpritesheet() const;         // Get the spritesheet texture
    SpriteInfo getSprite(SpriteID spriteId) const; // Get sprite coordinates
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
    GRRLIB_texImg *spritesheet;                      // The main spritesheet texture
    std::map<SpriteID, SpriteInfo> sprites;          // Sprite definitions
};