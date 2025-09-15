#include <fat.h>
#include <ogcsys.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern DISC_INTERFACE __io_wiisd;
extern DISC_INTERFACE __io_usbstorage;
extern DISC_INTERFACE __io_gcsda;
extern DISC_INTERFACE __io_gcsdb;

#include "input/InputManager.h"
#include "physics/PhysicsEngine.h"
#include "rendering/Renderer.h"
#include "audio/AudioManager.h"
#include "assets/AssetManager.h"

int main() {
    fatInitDefault();
    srand((unsigned)time(NULL));

    // Create module instances
    InputManager input;
    PhysicsEngine physics;
    Renderer renderer;
    AudioManager audio;
    AssetManager& assets = AssetManager::getInstance();

    // Initialize modules
    printf("Initializing WiingPong...\n");
    
    assets.init();
    input.init();
    physics.init();
    renderer.init();
    audio.init();

    // Load audio assets
    printf("Loading audio...\n");
    assets.loadAudio(audio);
    
    // Play intro sound
    printf("Playing intro sound...\n");
    audio.playSound(SoundID::Intro);

    bool running = true;
    while (running) {
        // Update input
        input.update();
        auto events = input.getEvents();

        // Process input events for player movement
        bool paddleUp = false;
        bool paddleDown = false;
        for (const auto& event : events) {
            if (event.type == InputEventType::PaddleUp) {
                paddleUp = true;
            } else if (event.type == InputEventType::PaddleDown) {
                paddleDown = true;
            } else if (event.type == InputEventType::Home) {
                running = false;
                break;
            }
        }

        // Apply player movement to physics
        if (paddleUp) {
            physics.velocities[PLAYER_PADDLE].dy = -5;  // paddleSpeed
        } else if (paddleDown) {
            physics.velocities[PLAYER_PADDLE].dy = 5;
        } else {
            physics.velocities[PLAYER_PADDLE].dy = 0;
        }

        // Update physics (includes AI, ball, collisions)
        physics.update();

        // Render
        renderer.render(physics);
        GRRLIB_Render();
    }

    // Cleanup (handled in destructors)
    printf("Shutting down WiingPong...\n");
    return 0;
}