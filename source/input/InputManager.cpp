// source/input/InputManager.cpp
#include "input/InputManager.h"
#include <ogcsys.h>

InputManager::InputManager() : initialized(false), heldButtons(0), pressedButtons(0) {
    events.clear();
}

InputManager::~InputManager() {
    if (initialized) {
        WPAD_Shutdown();
    }
}

void InputManager::init() {
    if (initialized) return;

    WPAD_Init();
    WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC_IR);
    initialized = true;
}

void InputManager::update() {
    if (!initialized) return;

    events.clear();
    WPAD_ScanPads();

    heldButtons = WPAD_ButtonsHeld(0);
    pressedButtons = WPAD_ButtonsDown(0);

    // Generate events for paddle movement (held for continuous)
    if (heldButtons & WPAD_BUTTON_PLUS) {
        events.push_back({InputEventType::PaddleUp});
    }
    if (heldButtons & WPAD_BUTTON_MINUS) {
        events.push_back({InputEventType::PaddleDown});
    }

    // Home for exit (pressed once)
    if (pressedButtons & WPAD_BUTTON_HOME) {
        events.push_back({InputEventType::Home});
    }
}

std::vector<InputEvent> InputManager::getEvents() const {
    return events;
}