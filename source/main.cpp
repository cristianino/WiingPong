#include <fat.h>
#include <ogcsys.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern DISC_INTERFACE __io_wiisd;
extern DISC_INTERFACE __io_usbstorage;
extern DISC_INTERFACE __io_gcsda;
extern DISC_INTERFACE __io_gcsdb;

#include "config.h"
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
                printf("[MAIN] PaddleUp event processed - paddleUp = true\n");
            } else if (event.type == InputEventType::PaddleDown) {
                paddleDown = true;
                printf("[MAIN] PaddleDown event processed - paddleDown = true\n");
            } else if (event.type == InputEventType::Home) {
                running = false;
                break;
#if WIINGPONG_DEBUG_ENABLED
            } else if (event.type == InputEventType::ToggleDebug) {
                // Toggle debug visibility
                renderer.setDebugVisible(!renderer.isDebugVisible());
                printf("Debug view %s\n", renderer.isDebugVisible() ? "enabled" : "disabled");
#endif
            }
        }

        // Apply player movement to physics
        if (paddleUp) {
            physics.velocities[PLAYER_PADDLE].dy = -WIINGPONG_PADDLE_SPEED;  // Use config
            printf("[MAIN] Applied paddleUp - velocity.dy = %d\n", -WIINGPONG_PADDLE_SPEED);
        } else if (paddleDown) {
            physics.velocities[PLAYER_PADDLE].dy = WIINGPONG_PADDLE_SPEED;
            printf("[MAIN] Applied paddleDown - velocity.dy = %d\n", WIINGPONG_PADDLE_SPEED);
        } else {
            physics.velocities[PLAYER_PADDLE].dy = 0;
        }

        // Update physics (includes AI, ball, collisions)
        physics.update();

        // Render
        renderer.render(physics);
        
        // Render debug info if enabled
        renderer.renderDebugInfo(input);
        
        // Always render debug toggle progress (A+B indicator)
        renderer.renderDebugToggleProgress(input);
        
        GRRLIB_Render();
    }

    // Cleanup (handled in destructors)
    printf("Shutting down WiingPong...\n");
    return 0;
}