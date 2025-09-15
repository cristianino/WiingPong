// source/assets/AssetManager.cpp
#include "assets/AssetManager.h"
#include <grrlib.h>
#include <fat.h>
#include <sdcard/wiisd_io.h>
#include <stdio.h>

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
    // Stub: Load textures, fonts from /assets/ on SD
    // Audio is loaded separately via loadAudio()
}

void AssetManager::loadAudio(AudioManager& audioManager) {
    if (!initialized) return;
    
    printf("Loading audio assets...\n");
    
    // Load intro.pcm from data/sounds/
    if (audioManager.loadPCM(SoundID::Intro, "data/sounds/intro.pcm")) {
        printf("Successfully loaded intro.pcm\n");
    } else {
        printf("Failed to load intro.pcm\n");
    }
    
    // Add more audio files here as needed
    // audioManager.loadPCM(SoundID::Hit, "data/sounds/hit.pcm");
    // audioManager.loadPCM(SoundID::Score, "data/sounds/score.pcm");
}

GRRLIB_texImg* AssetManager::getTexture(const char* name) const {
    // Stub: Return nullptr; rendering uses primitives
    (void)name; // Suppress unused parameter warning
    return nullptr;
}

void* AssetManager::getSound(const char* name) const {
    // Stub for audio buffer
    (void)name; // Suppress unused parameter warning
    return nullptr;
}

GRRLIB_texImg* AssetManager::getFont(const char* name) const {
    // Stub: Return nullptr; use primitive text later
    (void)name; // Suppress unused parameter warning
    return nullptr;
}