// source/input/InputManager.cpp
#include "input/InputManager.h"
#include "config.h"
#include <ogcsys.h>
#include <stdio.h>
#include <unistd.h>  // For usleep

InputManager::InputManager() : initialized(false), heldButtons(0), pressedButtons(0), wiimoteConnected(false), 
                               debugToggleTimer(0), wasABPressed(false), sensorDataValid(false) {
    events.clear();
    
    // Initialize sensor data with default values
    accelData.x = accelData.y = accelData.z = 0;
    orientData.roll = orientData.pitch = orientData.yaw = 0.0f;
    orientData.a_roll = orientData.a_pitch = 0.0f;
    gforceData.x = gforceData.y = gforceData.z = 0.0f;
    irData.num_dots = 0;
    irData.raw_valid = 0;
    irData.ax = irData.ay = irData.distance = 0.0f;
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
    
    // Collect sensor data if available
    sensorDataValid = false;
    WPADData* data = WPAD_Data(0);
    if (data) {
        // Copy sensor data from WPAD
        accelData = data->accel;
        orientData = data->orient;
        gforceData = data->gforce;
        irData = data->ir;
        sensorDataValid = true;
    } else {
        // Initialize with default values if no data available
        accelData.x = accelData.y = accelData.z = 0;
        orientData.roll = orientData.pitch = orientData.yaw = 0.0f;
        orientData.a_roll = orientData.a_pitch = 0.0f;
        gforceData.x = gforceData.y = gforceData.z = 0.0f;
        irData.num_dots = 0;
        irData.raw_valid = 0;
        irData.ax = irData.ay = irData.distance = 0.0f;
    }
    
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
    
    #elif WIINGPONG_DEBUG_TOGGLE_METHOD == 2
    // PLUS+MINUS hold method for debug toggle
    bool currentPMPressed = (heldButtons & WPAD_BUTTON_PLUS) && (heldButtons & WPAD_BUTTON_MINUS);
    
    if (currentPMPressed) {
        if (!wasABPressed) {
            // Just started pressing PLUS+MINUS
            debugToggleTimer = 0;
            wasABPressed = true;
        } else {
            // Continue holding PLUS+MINUS
            debugToggleTimer++;
            if (debugToggleTimer >= DEBUG_TOGGLE_DURATION) {
                // Held for configured duration, trigger debug toggle
                events.push_back({InputEventType::ToggleDebug});
                debugToggleTimer = 0; // Reset to prevent multiple triggers
            }
        }
    } else {
        // Not holding PLUS+MINUS anymore
        if (wasABPressed) {
            debugToggleTimer = 0;
        }
        wasABPressed = false;
    }
    
    // Normal paddle movement (always enabled with this method)
    if (heldButtons & WPAD_BUTTON_A) {
        events.push_back({InputEventType::PaddleUp});
        printf("[INPUT] A pressed - PaddleUp event generated\n");
    }
    if (heldButtons & WPAD_BUTTON_B) {
        events.push_back({InputEventType::PaddleDown});
        printf("[INPUT] B pressed - PaddleDown event generated\n");
    }
    
    #elif WIINGPONG_DEBUG_TOGGLE_METHOD == 0
    // Single button press method for debug toggle
    if (pressedButtons & WIINGPONG_DEBUG_TOGGLE_BUTTON) {
        events.push_back({InputEventType::ToggleDebug});
    }
    
    // Normal paddle movement
    if (heldButtons & WPAD_BUTTON_A) {
        events.push_back({InputEventType::PaddleUp});
        printf("[INPUT] A pressed - PaddleUp event generated (method 0)\n");
    }
    if (heldButtons & WPAD_BUTTON_B) {
        events.push_back({InputEventType::PaddleDown});
        printf("[INPUT] B pressed - PaddleDown event generated (method 0)\n");
    }
    #endif
    
#else
    // Debug disabled - normal paddle movement only
    if (heldButtons & WPAD_BUTTON_A) {
        events.push_back({InputEventType::PaddleUp});
        printf("[INPUT] A pressed - PaddleUp event generated (debug disabled)\n");
    }
    if (heldButtons & WPAD_BUTTON_B) {
        events.push_back({InputEventType::PaddleDown});
        printf("[INPUT] B pressed - PaddleDown event generated (debug disabled)\n");
    }
#endif

    // Menu navigation events (work in any state)
    if (pressedButtons & WPAD_BUTTON_UP) {
        events.push_back({InputEventType::MenuUp});
    }
    if (pressedButtons & WPAD_BUTTON_DOWN) {
        events.push_back({InputEventType::MenuDown});
    }
    // Multiple buttons can select menu items for better usability
    if (pressedButtons & WPAD_BUTTON_1 || pressedButtons & WPAD_BUTTON_A) {
        events.push_back({InputEventType::MenuSelect});
    }

    // Home for exit (pressed once)
    if (pressedButtons & WPAD_BUTTON_HOME) {
        events.push_back({InputEventType::Home});
    }
}

std::vector<InputEvent> InputManager::getEvents() const {
    return events;
}