#pragma once

#include <grrlib.h>
#include <fat.h>
#include "audio/AudioManager.h"

// Forward declaration
class WiimoteManager;

// Simple singleton for asset management
// Load textures, sounds, fonts from SD card via fat.h

class AssetManager {
public:
    static AssetManager& getInstance();

    void init();
    void loadAllAssets();  // Preload game assets
    void loadAudio(AudioManager& audioManager);  // Load audio assets
    void loadWiimoteAudio(WiimoteManager& wiimoteManager);  // Load Wiimote audio assets

    // Stubs for getters; return nullptr or default
    GRRLIB_texImg* getTexture(const char* name) const;
    void* getSound(const char* name) const;  // For future audio
    GRRLIB_texImg* getFont(const char* name) const;

    ~AssetManager();

private:
    AssetManager();  // Private for singleton
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    bool initialized;
};