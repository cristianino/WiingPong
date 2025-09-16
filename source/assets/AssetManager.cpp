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
    
    // Load sound effects
    if (audioManager.loadPCM(SoundID::PaddleHit, "data/sounds/paddle_hit.pcm")) {
        printf("Successfully loaded paddle_hit.pcm\n");
    } else {
        printf("Failed to load paddle_hit.pcm\n");
    }
    
    if (audioManager.loadPCM(SoundID::WallHit, "data/sounds/wall_hit.pcm")) {
        printf("Successfully loaded wall_hit.pcm\n");
    } else {
        printf("Failed to load wall_hit.pcm\n");
    }
    
    if (audioManager.loadPCM(SoundID::Score, "data/sounds/score.pcm")) {
        printf("Successfully loaded score.pcm\n");
    } else {
        printf("Failed to load score.pcm\n");
    }
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