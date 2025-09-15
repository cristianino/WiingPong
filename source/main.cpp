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

int main() {
    fatInitDefault();
    srand((unsigned)time(NULL));

    // Create module instances
    InputManager input;
    PhysicsEngine physics;
    Renderer renderer;

    // Initialize modules
    input.init();
    physics.init();
    renderer.init();

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
    return 0;
}