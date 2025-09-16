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
    
    // Try to detect Wiimote
    printf("Looking for Wiimotes...\n");
    for (int i = 0; i < 10; i++) {
        WPAD_ScanPads();
        if (WPAD_Probe(0, NULL) == WPAD_ERR_NONE) {
            printf("Wiimote found!\n");
            break;
        }
        usleep(100000); // Wait 100ms between tries
    }
    
    initialized = true;
}

void InputManager::update() {
    if (!initialized) return;

    events.clear();
    WPAD_ScanPads();

    // Check if Wiimote is connected
    wiimoteConnected = (WPAD_Probe(0, NULL) == WPAD_ERR_NONE);

    if (wiimoteConnected) {
        heldButtons = WPAD_ButtonsHeld(0);
        pressedButtons = WPAD_ButtonsDown(0);

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
    } else {
        heldButtons = 0;
        pressedButtons = 0;
    }
}

std::vector<InputEvent> InputManager::getEvents() const {
    return events;
}