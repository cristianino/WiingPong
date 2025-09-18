#pragma once

#include <wiiuse/wpad.h>
#include <wiiuse/wiiuse.h>  // For sensor structures
#include <vector>
#include "../config.h"

// Simple event system for input
enum class InputEventType {
    PaddleUp,      // A button held for player movement up
    PaddleDown,    // B button held for player movement down
    Home,          // HOME button pressed to exit/pause
    ToggleDebug,   // A+B held for 4 seconds to toggle debug view
    MenuUp,        // Navigate menu up (UP on d-pad or A button)
    MenuDown,      // Navigate menu down (DOWN on d-pad or B button)
    MenuSelect,    // Select menu item (A button press or 1 button)
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
    
    // Debug functions
    bool isInitialized() const { return initialized; }
    u32 getHeldButtons() const { return heldButtons; }
    u32 getPressedButtons() const { return pressedButtons; }
    bool isWiimoteConnected() const { return wiimoteConnected; }
    float getDebugToggleProgress() const { return wasABPressed ? (float)debugToggleTimer / DEBUG_TOGGLE_DURATION : 0.0f; }
    
    // Sensor data access functions
    const vec3w_t& getAccelerometerData() const { return accelData; }
    const orient_t& getOrientationData() const { return orientData; }
    const gforce_t& getGForceData() const { return gforceData; }
    const ir_t& getIRData() const { return irData; }
    bool hasSensorData() const { return sensorDataValid; }

private:
    bool initialized;
    std::vector<InputEvent> events;
    u32 heldButtons;
    u32 pressedButtons;
    bool wiimoteConnected;
    
    // Debug toggle timing
    u32 debugToggleTimer;
    bool wasABPressed;
    static const u32 DEBUG_TOGGLE_DURATION = WIINGPONG_DEBUG_TOGGLE_DURATION_FRAMES;
    
    // Sensor data
    vec3w_t accelData;
    orient_t orientData;
    gforce_t gforceData;
    ir_t irData;
    bool sensorDataValid;
};