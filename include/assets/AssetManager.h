#pragma once

#include <grrlib.h>
#include <fat.h>

// Simple singleton for asset management
// Stubs for textures, sounds, fonts; load from SD card via fat.h

class AssetManager {
public:
    static AssetManager& getInstance();

    void init();
    void loadAllAssets();  // Preload game assets

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
    // Maps or arrays for assets; stubs as nullptr
};