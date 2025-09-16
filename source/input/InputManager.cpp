// source/input/InputManager.cpp
#include "input/InputManager.h"
#include "config.h"
#include <ogcsys.h>
#include <stdio.h>
#include <unistd.h>  // For usleep

InputManager::InputManager() : initialized(false), heldButtons(0), pressedButtons(0), wiimoteConnected(false), 
                               debugToggleTimer(0), wasABPressed(false) {
    events.clear();
}

InputManager::~InputManager() {
    if (initialized) {
        WPAD_Shutdown();
    }
}

void InputManager::init() {
    if (initialized) return;

    printf("Initializing WPAD...\n");
    WPAD_Init();
    
    // Set data format for buttons, accelerometer, and IR
    WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC_IR);
    
    // Wait a bit for Wiimote to initialize
    usleep(100000); // 100ms
    
    printf("WPAD initialized. Looking for Wiimotes during gameplay...\n");
    
    initialized = true;
}

void InputManager::update() {
    if (!initialized) return;

    events.clear();
    WPAD_ScanPads();

    // Try to get button data - if we get valid data, Wiimote is connected
    heldButtons = WPAD_ButtonsHeld(0);
    pressedButtons = WPAD_ButtonsDown(0);
    
    // Simple connection detection: if we can read button data or if any button is pressed
    // This is a basic check - a more sophisticated approach would check for specific conditions
    wiimoteConnected = (heldButtons != 0 || pressedButtons != 0 || initialized);
    
    // For now, assume connected if initialized (will show in debug)
    wiimoteConnected = initialized;

#if WIINGPONG_DEBUG_ENABLED
    
    #if WIINGPONG_DEBUG_TOGGLE_METHOD == 1
    // A+B hold method for debug toggle
    bool currentABPressed = (heldButtons & WPAD_BUTTON_A) && (heldButtons & WPAD_BUTTON_B);
    
    if (currentABPressed) {
        if (!wasABPressed) {
            // Just started pressing A+B
            debugToggleTimer = 0;
            wasABPressed = true;
        } else {
            // Continue holding A+B
            debugToggleTimer++;
            if (debugToggleTimer >= DEBUG_TOGGLE_DURATION) {
                // Held for configured duration, trigger debug toggle
                events.push_back({InputEventType::ToggleDebug});
                debugToggleTimer = 0; // Reset to prevent multiple triggers
            }
        }
    } else {
        // Not holding A+B anymore
        if (wasABPressed) {
            debugToggleTimer = 0;
        }
        wasABPressed = false;
        
        // Generate events for paddle movement (only if not holding both A+B)
        if (heldButtons & WPAD_BUTTON_A) {
            events.push_back({InputEventType::PaddleUp});
        }
        if (heldButtons & WPAD_BUTTON_B) {
            events.push_back({InputEventType::PaddleDown});
        }
    }
    
    #elif WIINGPONG_DEBUG_TOGGLE_METHOD == 0
    // Single button press method for debug toggle
    if (pressedButtons & WIINGPONG_DEBUG_TOGGLE_BUTTON) {
        events.push_back({InputEventType::ToggleDebug});
    }
    
    // Normal paddle movement
    if (heldButtons & WPAD_BUTTON_A) {
        events.push_back({InputEventType::PaddleUp});
    }
    if (heldButtons & WPAD_BUTTON_B) {
        events.push_back({InputEventType::PaddleDown});
    }
    #endif
    
#else
    // Debug disabled - normal paddle movement only
    if (heldButtons & WPAD_BUTTON_A) {
        events.push_back({InputEventType::PaddleUp});
    }
    if (heldButtons & WPAD_BUTTON_B) {
        events.push_back({InputEventType::PaddleDown});
    }
#endif

    // Home for exit (pressed once)
    if (pressedButtons & WPAD_BUTTON_HOME) {
        events.push_back({InputEventType::Home});
    }
}

std::vector<InputEvent> InputManager::getEvents() const {
    return events;
}