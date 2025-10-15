// source/assets/AssetManager.cpp
#include "assets/AssetManager.h"
#include "WiimoteManager.h"
#include <grrlib.h>
#include <fat.h>
#include <sdcard/wiisd_io.h>
#include <stdio.h>

AssetManager &AssetManager::getInstance()
{
    static AssetManager instance;
    return instance;
}

AssetManager::AssetManager() : initialized(false), spritesheet(nullptr), currentAtlasType(AtlasType::Normal)
{
}

AssetManager::~AssetManager()
{
    // Cleanup loaded textures
    for (auto &pair : textures)
    {
        if (pair.second)
        {
            GRRLIB_FreeTexture(pair.second);
        }
    }
    textures.clear();

    // Cleanup spritesheet
    if (spritesheet)
    {
        GRRLIB_FreeTexture(spritesheet);
        spritesheet = nullptr;
    }

    // Cleanup multiple atlases
    for (auto &pair : atlases)
    {
        if (pair.second)
        {
            GRRLIB_FreeTexture(pair.second);
        }
    }
    atlases.clear();
}

void AssetManager::init()
{
    if (initialized)
        return;
    // Mount SD card if needed
    fatMount("sd", &__io_wiisd, 0, 10, 8);
    initialized = true;
}

void AssetManager::loadAllAssets()
{
    // Load textures first
    loadTextures();
    // Load spritesheet (legacy)
    loadSpritesheet();
    // Load multiple atlases
    loadMultipleAtlases();
}

void AssetManager::loadTextures()
{
    if (!initialized)
        return;

    printf("Loading texture assets...\n");

    // Load banner image with multiple path attempts
    // Try icon_banner.png first (perfect size for menu), then fallback to other banners
    const char *bannerPaths[] = {
        "./icon_banner_fixed.png",                        // RGBA version first (for development)
        "icon_banner_fixed.png",                          // RGBA version relative path
        "./icon_banner.png",                              // Current directory (for development)
        "icon_banner.png",                                // Relative path
        "sd:/icon_banner_fixed.png",                      // RGBA version on SD card root
        "sd:/icon_banner.png",                            // SD card root
        "sd:/apps/WiingPong/icon_banner_fixed.png",       // RGBA version on SD card apps folder
        "sd:/apps/WiingPong/icon_banner.png",             // SD card apps folder
        "apps/WiingPong/icon_banner_fixed.png",           // RGBA version in local apps folder
        "apps/WiingPong/icon_banner.png",                 // Local apps folder
        "./apps/WiingPong/icon_banner_fixed.png",         // RGBA version in local apps folder with ./
        "./apps/WiingPong/icon_banner.png",               // Local apps folder with ./
        "sd:/apps/WiingPong/data/img/wiipong-banner.png", // Fallback: large banner on SD
        "sd:/data/img/wiipong-banner.png",                // Fallback: large banner on SD
        "data/img/wiipong-banner.png",                    // Fallback: large banner local
        "apps/WiingPong/data/img/wiipong-banner.png"      // Fallback: large banner apps folder
    };

    GRRLIB_texImg *bannerTexture = nullptr;
    for (int i = 0; i < 16 && !bannerTexture; i++)
    {
        printf("Trying to load banner from: %s\n", bannerPaths[i]);
        bannerTexture = GRRLIB_LoadTextureFromFile(bannerPaths[i]);
        if (bannerTexture)
        {
            printf("Successfully loaded banner from %s (size: %dx%d)\n", bannerPaths[i], bannerTexture->w, bannerTexture->h);
            textures["banner"] = bannerTexture;
        }
        else
        {
            printf("Failed to load from: %s\n", bannerPaths[i]);
        }
    }
    if (!bannerTexture)
    {
        printf("Failed to load banner from any path\n");
    }
}

void AssetManager::loadAudio(AudioManager &audioManager)
{
    if (!initialized)
        return;

    printf("Loading audio assets...\n");

    // Load intro.pcm from data/sounds/ (try multiple possible paths)
    const char *introPaths[] = {
        "sd:/apps/WiingPong/data/sounds/intro.pcm",
        "sd:/data/sounds/intro.pcm",
        "data/sounds/intro.pcm",
        "apps/WiingPong/data/sounds/intro.pcm"};

    bool introLoaded = false;
    for (int i = 0; i < 4 && !introLoaded; i++)
    {
        printf("Trying to load intro from: %s\n", introPaths[i]);
        if (audioManager.loadPCM(SoundID::Intro, introPaths[i]))
        {
            printf("Successfully loaded intro.pcm from %s\n", introPaths[i]);
            introLoaded = true;
        }
    }
    if (!introLoaded)
    {
        printf("Failed to load intro.pcm from any path\n");
    }

    // Load sound effects with multiple path attempts
    const char *paddlePaths[] = {
        "sd:/apps/WiingPong/data/sounds/paddle_hit.pcm",
        "sd:/data/sounds/paddle_hit.pcm",
        "data/sounds/paddle_hit.pcm",
        "apps/WiingPong/data/sounds/paddle_hit.pcm"};

    bool paddleLoaded = false;
    for (int i = 0; i < 4 && !paddleLoaded; i++)
    {
        printf("Trying to load paddle_hit from: %s\n", paddlePaths[i]);
        if (audioManager.loadPCM(SoundID::PaddleHit, paddlePaths[i]))
        {
            printf("Successfully loaded paddle_hit.pcm from %s\n", paddlePaths[i]);
            paddleLoaded = true;
        }
    }
    if (!paddleLoaded)
    {
        printf("Failed to load paddle_hit.pcm from any path\n");
    }

    const char *wallPaths[] = {
        "sd:/apps/WiingPong/data/sounds/wall_hit.pcm",
        "sd:/data/sounds/wall_hit.pcm",
        "data/sounds/wall_hit.pcm",
        "apps/WiingPong/data/sounds/wall_hit.pcm"};

    bool wallLoaded = false;
    for (int i = 0; i < 4 && !wallLoaded; i++)
    {
        printf("Trying to load wall_hit from: %s\n", wallPaths[i]);
        if (audioManager.loadPCM(SoundID::WallHit, wallPaths[i]))
        {
            printf("Successfully loaded wall_hit.pcm from %s\n", wallPaths[i]);
            wallLoaded = true;
        }
    }
    if (!wallLoaded)
    {
        printf("Failed to load wall_hit.pcm from any path\n");
    }

    const char *scorePaths[] = {
        "sd:/apps/WiingPong/data/sounds/score.pcm",
        "sd:/data/sounds/score.pcm",
        "data/sounds/score.pcm",
        "apps/WiingPong/data/sounds/score.pcm"};

    bool scoreLoaded = false;
    for (int i = 0; i < 4 && !scoreLoaded; i++)
    {
        printf("Trying to load score from: %s\n", scorePaths[i]);
        if (audioManager.loadPCM(SoundID::Score, scorePaths[i]))
        {
            printf("Successfully loaded score.pcm from %s\n", scorePaths[i]);
            scoreLoaded = true;
        }
    }
    if (!scoreLoaded)
    {
        printf("Failed to load score.pcm from any path\n");
    }
}

void AssetManager::loadWiimoteAudio(WiimoteManager &wiimoteManager)
{
    if (!initialized)
        return;

    printf("Loading Wiimote audio assets...\n");

    // Load Wiimote sound effects
    if (wiimoteManager.loadWiimoteSound(WiimoteSoundID::PlayerPaddleHit, "data/sounds/wiimote/player_paddle_hit.pcm"))
    {
        printf("Successfully loaded player_paddle_hit.pcm for Wiimote\n");
    }
    else
    {
        printf("Failed to load player_paddle_hit.pcm for Wiimote\n");
    }

    if (wiimoteManager.loadWiimoteSound(WiimoteSoundID::PlayerScore, "data/sounds/wiimote/player_score.pcm"))
    {
        printf("Successfully loaded player_score.pcm for Wiimote\n");
    }
    else
    {
        printf("Failed to load player_score.pcm for Wiimote\n");
    }

    if (wiimoteManager.loadWiimoteSound(WiimoteSoundID::PlayerLoss, "data/sounds/wiimote/player_loss.pcm"))
    {
        printf("Successfully loaded player_loss.pcm for Wiimote\n");
    }
    else
    {
        printf("Failed to load player_loss.pcm for Wiimote\n");
    }
}

GRRLIB_texImg *AssetManager::getTexture(const char *name) const
{
    auto it = textures.find(name);
    if (it != textures.end())
    {
        return it->second;
    }
    return nullptr;
}

void *AssetManager::getSound(const char *name) const
{
    // Stub for audio buffer
    (void)name; // Suppress unused parameter warning
    return nullptr;
}

GRRLIB_texImg *AssetManager::getFont(const char *name) const
{
    // Stub: Return nullptr; use primitive text later
    (void)name; // Suppress unused parameter warning
    return nullptr;
}

void AssetManager::loadSpritesheet()
{
    if (!initialized)
        return;

    printf("Loading spritesheet...\n");

    // Try to load spritesheet from multiple possible paths
    const char *spritesheetPaths[] = {
        "sd:/apps/WiingPong/data/img/spritesheet.png",
        "sd:/data/img/spritesheet.png",
        "data/img/spritesheet.png",
        "apps/WiingPong/data/img/spritesheet.png",
        "./data/img/spritesheet.png",
        "./apps/WiingPong/data/img/spritesheet.png"};

    for (int i = 0; i < 6 && !spritesheet; i++)
    {
        printf("Trying to load spritesheet from: %s\n", spritesheetPaths[i]);
        spritesheet = GRRLIB_LoadTextureFromFile(spritesheetPaths[i]);
        if (spritesheet)
        {
            printf("Successfully loaded spritesheet from %s (size: %dx%d)\n",
                   spritesheetPaths[i], spritesheet->w, spritesheet->h);
            break;
        }
        else
        {
            printf("Failed to load from: %s\n", spritesheetPaths[i]);
        }
    }

    if (!spritesheet)
    {
        printf("Failed to load spritesheet from any path\n");
        return;
    }

    // Initialize sprite definitions based on the image structure
    initializeSpriteDefinitions();
}

void AssetManager::initializeSpriteDefinitions()
{
    // Based on the spritesheet image, define coordinates for each sprite
    // Analyzing the image: it appears to be approximately 640x480 pixels
    // Left paddle (brown/dark area on left)
    sprites[SpriteID::LeftPaddle] = SpriteInfo(0, 0, 150, 480);

    // Right paddle (similar position on right side, but we'll use the left one and flip it)
    sprites[SpriteID::RightPaddle] = SpriteInfo(0, 0, 150, 480);

    // Ball (circular white object in center-upper area)
    sprites[SpriteID::Ball] = SpriteInfo(300, 50, 60, 60);

    // Score digits (from the score display area)
    // Digit 0 (from the score "0")
    sprites[SpriteID::Digit0] = SpriteInfo(220, 180, 60, 80);
    // Digit 1 (from the score "1")
    sprites[SpriteID::Digit1] = SpriteInfo(360, 180, 60, 80);

    // For other digits, we'll use the available ones and create variations
    sprites[SpriteID::Digit2] = SpriteInfo(360, 180, 60, 80); // Use 1 as template
    sprites[SpriteID::Digit3] = SpriteInfo(360, 180, 60, 80);
    sprites[SpriteID::Digit4] = SpriteInfo(360, 180, 60, 80);
    sprites[SpriteID::Digit5] = SpriteInfo(360, 180, 60, 80);
    sprites[SpriteID::Digit6] = SpriteInfo(360, 180, 60, 80);
    sprites[SpriteID::Digit7] = SpriteInfo(360, 180, 60, 80);
    sprites[SpriteID::Digit8] = SpriteInfo(360, 180, 60, 80);
    sprites[SpriteID::Digit9] = SpriteInfo(360, 180, 60, 80);

    // Center line (vertical dashed line in the middle)
    sprites[SpriteID::CenterLine] = SpriteInfo(320, 300, 20, 180);

    // Background (whole playing field area)
    sprites[SpriteID::Background] = SpriteInfo(150, 0, 340, 480);

    printf("Initialized %zu sprite definitions\n", sprites.size());
}

GRRLIB_texImg *AssetManager::getSpritesheet() const
{
    // Return current active atlas, fallback to legacy spritesheet
    GRRLIB_texImg *currentTexture = getAtlas(currentAtlasType);
    return currentTexture ? currentTexture : spritesheet;
}

SpriteInfo AssetManager::getSprite(SpriteID spriteId) const
{
    auto it = sprites.find(spriteId);
    if (it != sprites.end())
    {
        return it->second;
    }
    return SpriteInfo(); // Return empty sprite info if not found
}

void AssetManager::drawSprite(SpriteID spriteId, int x, int y, float scaleX, float scaleY, u32 color) const
{
    // Use current active atlas, fallback to legacy spritesheet
    GRRLIB_texImg *currentTexture = getAtlas(currentAtlasType);
    if (!currentTexture)
    {
        currentTexture = spritesheet;
    }

    if (!currentTexture)
        return;

    SpriteInfo sprite = getSprite(spriteId);
    if (sprite.width == 0 || sprite.height == 0)
        return;

    // Use GRRLIB_DrawPart to draw a portion of the current atlas
    GRRLIB_DrawPart(x, y, sprite.x, sprite.y, sprite.width, sprite.height,
                    currentTexture, 0, scaleX, scaleY, color);
}

void AssetManager::loadMultipleAtlases()
{
    if (!initialized)
        return;

    printf("Loading multiple atlases...\n");

    // Load normal atlas (atlas.png)
    const char *normalAtlasPaths[] = {
        "sd:/apps/WiingPong/data/img/atlas.png",
        "sd:/data/img/atlas.png",
        "data/img/atlas.png",
        "apps/WiingPong/data/img/atlas.png",
        "./data/img/atlas.png",
        "./apps/WiingPong/data/img/atlas.png",
        // Fallback to spritesheet.png
        "sd:/apps/WiingPong/data/img/spritesheet.png",
        "sd:/data/img/spritesheet.png",
        "data/img/spritesheet.png",
        "apps/WiingPong/data/img/spritesheet.png"};

    GRRLIB_texImg *normalAtlas = nullptr;
    for (int i = 0; i < 10 && !normalAtlas; i++)
    {
        printf("Trying to load normal atlas from: %s\n", normalAtlasPaths[i]);
        normalAtlas = GRRLIB_LoadTextureFromFile(normalAtlasPaths[i]);
        if (normalAtlas)
        {
            printf("Successfully loaded normal atlas from %s (size: %dx%d)\n",
                   normalAtlasPaths[i], normalAtlas->w, normalAtlas->h);
            atlases[AtlasType::Normal] = normalAtlas;
            break;
        }
    }

    // Load intense atlas (atlas_2.png)
    const char *intenseAtlasPaths[] = {
        "sd:/apps/WiingPong/data/img/atlas_2.png",
        "sd:/data/img/atlas_2.png",
        "data/img/atlas_2.png",
        "apps/WiingPong/data/img/atlas_2.png",
        "./data/img/atlas_2.png",
        "./apps/WiingPong/data/img/atlas_2.png"};

    GRRLIB_texImg *intenseAtlas = nullptr;
    for (int i = 0; i < 6 && !intenseAtlas; i++)
    {
        printf("Trying to load intense atlas from: %s\n", intenseAtlasPaths[i]);
        intenseAtlas = GRRLIB_LoadTextureFromFile(intenseAtlasPaths[i]);
        if (intenseAtlas)
        {
            printf("Successfully loaded intense atlas from %s (size: %dx%d)\n",
                   intenseAtlasPaths[i], intenseAtlas->w, intenseAtlas->h);
            atlases[AtlasType::Intense] = intenseAtlas;
            break;
        }
    }

    if (!normalAtlas)
    {
        printf("WARNING: Failed to load normal atlas from any path\n");
    }
    if (!intenseAtlas)
    {
        printf("WARNING: Failed to load intense atlas from any path\n");
    }

    printf("Atlas loading complete. Normal: %s, Intense: %s\n",
           normalAtlas ? "✓" : "✗",
           intenseAtlas ? "✓" : "✗");
}

void AssetManager::setCurrentAtlas(AtlasType atlasType)
{
    // Only switch if the requested atlas exists
    if (atlases.find(atlasType) != atlases.end() && atlases[atlasType])
    {
        currentAtlasType = atlasType;
        printf("Switched to %s atlas\n",
               atlasType == AtlasType::Normal ? "Normal" : "Intense");
    }
    else
    {
        printf("WARNING: Requested atlas type not available, staying with current\n");
    }
}

AtlasType AssetManager::getCurrentAtlas() const
{
    return currentAtlasType;
}

AtlasType AssetManager::determineAtlasForScores(int playerScore, int cpuScore) const
{
    // Use intense atlas when both players have 3+ points and are tied
    if (playerScore >= 3 && cpuScore >= 3 && playerScore == cpuScore)
    {
        return AtlasType::Intense;
    }

    // Use normal atlas for all other cases
    return AtlasType::Normal;
}

GRRLIB_texImg *AssetManager::getAtlas(AtlasType atlasType) const
{
    auto it = atlases.find(atlasType);
    if (it != atlases.end())
    {
        return it->second;
    }
    return nullptr;
}