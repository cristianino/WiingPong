#pragma once

#include <wiiuse/wpad.h>
#include <vector>

// Simple event system for input
enum class InputEventType {
    PaddleUp,      // A button held for player movement up
    PaddleDown,    // B button held for player movement down
    Home,          // HOME button pressed to exit/pause
    None
};

struct InputEvent {
    InputEventType type;
    // Future: Add data like intensity for accelerometer
};

class InputManager {
public:
    InputManager();
    ~InputManager();

    void init();
    void update();
    std::vector<InputEvent> getEvents() const;

private:
    bool initialized;
    std::vector<InputEvent> events;
    u32 heldButtons;
    u32 pressedButtons;
};