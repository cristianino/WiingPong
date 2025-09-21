// source/assets/AssetManager.cpp
#include "assets/AssetManager.h"
#include "WiimoteManager.h"
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
    // Cleanup loaded textures
    for (auto& pair : textures) {
        if (pair.second) {
            GRRLIB_FreeTexture(pair.second);
        }
    }
    textures.clear();
}

void AssetManager::init() {
    if (initialized) return;
    // Mount SD card if needed
    fatMount("sd", &__io_wiisd, 0, 10, 8);
    initialized = true;
}

void AssetManager::loadAllAssets() {
    // Load textures first
    loadTextures();
}

void AssetManager::loadTextures() {
    if (!initialized) return;
    
    printf("Loading texture assets...\n");
    
    // Load banner image with multiple path attempts
    // Try icon_banner.png first (perfect size for menu), then fallback to other banners
    const char* bannerPaths[] = {
        "./icon_banner_fixed.png",                             // RGBA version first (for development)
        "icon_banner_fixed.png",                               // RGBA version relative path
        "./icon_banner.png",                                    // Current directory (for development)
        "icon_banner.png",                                      // Relative path
        "sd:/icon_banner_fixed.png",                           // RGBA version on SD card root
        "sd:/icon_banner.png",                                  // SD card root
        "sd:/apps/WiingPong/icon_banner_fixed.png",            // RGBA version on SD card apps folder
        "sd:/apps/WiingPong/icon_banner.png",                  // SD card apps folder
        "apps/WiingPong/icon_banner_fixed.png",                // RGBA version in local apps folder
        "apps/WiingPong/icon_banner.png",                      // Local apps folder
        "./apps/WiingPong/icon_banner_fixed.png",              // RGBA version in local apps folder with ./
        "./apps/WiingPong/icon_banner.png",                    // Local apps folder with ./
        "sd:/apps/WiingPong/data/img/wiipong-banner.png",      // Fallback: large banner on SD
        "sd:/data/img/wiipong-banner.png",                     // Fallback: large banner on SD
        "data/img/wiipong-banner.png",                         // Fallback: large banner local
        "apps/WiingPong/data/img/wiipong-banner.png"           // Fallback: large banner apps folder
    };
    
    GRRLIB_texImg* bannerTexture = nullptr;
    for (int i = 0; i < 16 && !bannerTexture; i++) {
        printf("Trying to load banner from: %s\n", bannerPaths[i]);
        bannerTexture = GRRLIB_LoadTextureFromFile(bannerPaths[i]);
        if (bannerTexture) {
            printf("Successfully loaded banner from %s (size: %dx%d)\n", bannerPaths[i], bannerTexture->w, bannerTexture->h);
            textures["banner"] = bannerTexture;
        } else {
            printf("Failed to load from: %s\n", bannerPaths[i]);
        }
    }
    if (!bannerTexture) {
        printf("Failed to load banner from any path\n");
    }
}

void AssetManager::loadAudio(AudioManager& audioManager) {
    if (!initialized) return;
    
    printf("Loading audio assets...\n");
    
    // Load intro.pcm from data/sounds/ (try multiple possible paths)
    const char* introPaths[] = {
        "sd:/apps/WiingPong/data/sounds/intro.pcm",
        "sd:/data/sounds/intro.pcm", 
        "data/sounds/intro.pcm",
        "apps/WiingPong/data/sounds/intro.pcm"
    };
    
    bool introLoaded = false;
    for (int i = 0; i < 4 && !introLoaded; i++) {
        printf("Trying to load intro from: %s\n", introPaths[i]);
        if (audioManager.loadPCM(SoundID::Intro, introPaths[i])) {
            printf("Successfully loaded intro.pcm from %s\n", introPaths[i]);
            introLoaded = true;
        }
    }
    if (!introLoaded) {
        printf("Failed to load intro.pcm from any path\n");
    }
    
    // Load sound effects with multiple path attempts
    const char* paddlePaths[] = {
        "sd:/apps/WiingPong/data/sounds/paddle_hit.pcm",
        "sd:/data/sounds/paddle_hit.pcm",
        "data/sounds/paddle_hit.pcm",
        "apps/WiingPong/data/sounds/paddle_hit.pcm"
    };
    
    bool paddleLoaded = false;
    for (int i = 0; i < 4 && !paddleLoaded; i++) {
        printf("Trying to load paddle_hit from: %s\n", paddlePaths[i]);
        if (audioManager.loadPCM(SoundID::PaddleHit, paddlePaths[i])) {
            printf("Successfully loaded paddle_hit.pcm from %s\n", paddlePaths[i]);
            paddleLoaded = true;
        }
    }
    if (!paddleLoaded) {
        printf("Failed to load paddle_hit.pcm from any path\n");
    }
    
    const char* wallPaths[] = {
        "sd:/apps/WiingPong/data/sounds/wall_hit.pcm",
        "sd:/data/sounds/wall_hit.pcm",
        "data/sounds/wall_hit.pcm",
        "apps/WiingPong/data/sounds/wall_hit.pcm"
    };
    
    bool wallLoaded = false;
    for (int i = 0; i < 4 && !wallLoaded; i++) {
        printf("Trying to load wall_hit from: %s\n", wallPaths[i]);
        if (audioManager.loadPCM(SoundID::WallHit, wallPaths[i])) {
            printf("Successfully loaded wall_hit.pcm from %s\n", wallPaths[i]);
            wallLoaded = true;
        }
    }
    if (!wallLoaded) {
        printf("Failed to load wall_hit.pcm from any path\n");
    }
    
    const char* scorePaths[] = {
        "sd:/apps/WiingPong/data/sounds/score.pcm",
        "sd:/data/sounds/score.pcm",
        "data/sounds/score.pcm",
        "apps/WiingPong/data/sounds/score.pcm"
    };
    
    bool scoreLoaded = false;
    for (int i = 0; i < 4 && !scoreLoaded; i++) {
        printf("Trying to load score from: %s\n", scorePaths[i]);
        if (audioManager.loadPCM(SoundID::Score, scorePaths[i])) {
            printf("Successfully loaded score.pcm from %s\n", scorePaths[i]);
            scoreLoaded = true;
        }
    }
    if (!scoreLoaded) {
        printf("Failed to load score.pcm from any path\n");
    }
}

void AssetManager::loadWiimoteAudio(WiimoteManager& wiimoteManager) {
    if (!initialized) return;
    
    printf("Loading Wiimote audio assets...\n");
    
    // Load Wiimote sound effects
    if (wiimoteManager.loadWiimoteSound(WiimoteSoundID::PlayerPaddleHit, "data/sounds/wiimote/player_paddle_hit.pcm")) {
        printf("Successfully loaded player_paddle_hit.pcm for Wiimote\n");
    } else {
        printf("Failed to load player_paddle_hit.pcm for Wiimote\n");
    }
    
    if (wiimoteManager.loadWiimoteSound(WiimoteSoundID::PlayerScore, "data/sounds/wiimote/player_score.pcm")) {
        printf("Successfully loaded player_score.pcm for Wiimote\n");
    } else {
        printf("Failed to load player_score.pcm for Wiimote\n");
    }
    
    if (wiimoteManager.loadWiimoteSound(WiimoteSoundID::PlayerLoss, "data/sounds/wiimote/player_loss.pcm")) {
        printf("Successfully loaded player_loss.pcm for Wiimote\n");
    } else {
        printf("Failed to load player_loss.pcm for Wiimote\n");
    }
}

GRRLIB_texImg* AssetManager::getTexture(const char* name) const {
    auto it = textures.find(name);
    if (it != textures.end()) {
        return it->second;
    }
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