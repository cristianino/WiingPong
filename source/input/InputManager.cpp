// source/input/InputManager.cpp
#include "input/InputManager.h"
#include <ogcsys.h>
#include <stdio.h>
#include <unistd.h>  // For usleep

InputManager::InputManager() : initialized(false), heldButtons(0), pressedButtons(0), wiimoteConnected(false) {
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

    // Generate events for paddle movement (held for continuous)
    if (heldButtons & WPAD_BUTTON_A) {
        events.push_back({InputEventType::PaddleUp});
    }
    if (heldButtons & WPAD_BUTTON_B) {
        events.push_back({InputEventType::PaddleDown});
    }

    // Home for exit (pressed once)
    if (pressedButtons & WPAD_BUTTON_HOME) {
        events.push_back({InputEventType::Home});
    }
    
    // Toggle debug view with button 1 (pressed once)
    if (pressedButtons & WPAD_BUTTON_1) {
        events.push_back({InputEventType::ToggleDebug});
    }
}

std::vector<InputEvent> InputManager::getEvents() const {
    return events;
}