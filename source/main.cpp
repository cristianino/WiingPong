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
#include "WiimoteManager.h"

int main() {
    fatInitDefault();
    srand((unsigned)time(NULL));

    // Create module instances
    InputManager input;
    PhysicsEngine physics;
    Renderer renderer;
    AudioManager audio;
    WiimoteManager wiimote;
    AssetManager& assets = AssetManager::getInstance();

    // Initialize modules
    printf("Initializing WiingPong...\n");
    
    assets.init();
    input.init();
    physics.init();
    renderer.init();
    audio.init();
    wiimote.init();

    // Connect audio and Wiimote to physics engine for sound effects
    physics.setAudioManager(&audio);
    physics.setWiimoteManager(&wiimote);

    // Load audio assets
    printf("Loading audio...\n");
    assets.loadAudio(audio);
    
    // Load Wiimote audio assets
    printf("Loading Wiimote audio...\n");
    assets.loadWiimoteAudio(wiimote);
    
    // Initialize Wiimote speaker
    printf("Initializing Wiimote speaker...\n");
    wiimote.initSpeaker(0);
    
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
            } else if (event.type == InputEventType::ToggleDebug) {
                // Toggle debug visibility
                renderer.setDebugVisible(!renderer.isDebugVisible());
                printf("Debug view %s\n", renderer.isDebugVisible() ? "enabled" : "disabled");
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
        
        // Render debug info
        renderer.renderDebugInfo(input);
        
        GRRLIB_Render();
    }

    // Cleanup (handled in destructors)
    printf("Shutting down WiingPong...\n");
    return 0;
}