// source/assets/AssetManager.cpp
#include "assets/AssetManager.h"
#include <grrlib.h>
#include <fat.h>
#include <sdcard/wiisd_io.h>

AssetManager& AssetManager::getInstance() {
    static AssetManager instance;
    return instance;
}

AssetManager::AssetManager() : initialized(false) {
}

AssetManager::~AssetManager() {
    // Cleanup assets if loaded
}

void AssetManager::init() {
    if (initialized) return;
    // Mount SD card if needed
    fatMount("sd", &__io_wiisd, 0, 10, 8);
    initialized = true;
}

void AssetManager::loadAllAssets() {
    // Stub: Load textures, sounds, fonts from /assets/ on SD
    // For now, do nothing; return defaults in getters
}

GRRLIB_texImg* AssetManager::getTexture(const char* name) const {
    // Stub: Return nullptr; rendering uses primitives
    return nullptr;
}

void* AssetManager::getSound(const char* name) const {
    // Stub for audio buffer
    return nullptr;
}

GRRLIB_texImg* AssetManager::getFont(const char* name) const {
    // Stub: Return nullptr; use primitive text later
    return nullptr;
}