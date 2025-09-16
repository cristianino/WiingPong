#pragma once

// WiingPong Configuration File
// This file contains compile-time constants for the game

// =============================================================================
// DEBUG CONFIGURATION
// =============================================================================

// Enable/disable debug features at compile time
// Set to 1 to enable debug view, 0 to disable completely
#define WIINGPONG_DEBUG_ENABLED 1

// Debug toggle method (when WIINGPONG_DEBUG_ENABLED = 1)
// 0 = Single button press (Button 1)
// 1 = A+B hold for specified duration
// 2 = PLUS+MINUS hold for specified duration (recommended for gameplay)
#define WIINGPONG_DEBUG_TOGGLE_METHOD 2

// Duration for A+B hold method (in frames at 60fps)
// 240 frames = 4 seconds
// 180 frames = 3 seconds  
// 300 frames = 5 seconds
#define WIINGPONG_DEBUG_TOGGLE_DURATION_FRAMES 240

// Debug toggle button for single press method (when toggle method = 0)
// WPAD_BUTTON_1, WPAD_BUTTON_2, WPAD_BUTTON_PLUS, WPAD_BUTTON_MINUS
#define WIINGPONG_DEBUG_TOGGLE_BUTTON WPAD_BUTTON_1

// =============================================================================
// GAME CONFIGURATION
// =============================================================================

// Paddle movement speed
#define WIINGPONG_PADDLE_SPEED 5

// Ball initial speed
#define WIINGPONG_BALL_SPEED_X 3
#define WIINGPONG_BALL_SPEED_Y 2

// =============================================================================
// AUDIO CONFIGURATION
// =============================================================================

// Enable/disable audio features
#define WIINGPONG_AUDIO_ENABLED 1
#define WIINGPONG_WIIMOTE_AUDIO_ENABLED 1

// =============================================================================
// BUILD CONFIGURATION
// =============================================================================

// Game version information
#define WIINGPONG_VERSION_MAJOR 1
#define WIINGPONG_VERSION_MINOR 2
#define WIINGPONG_VERSION_PATCH 0

// Build date (automatically set by build system)
#define WIINGPONG_BUILD_DATE __DATE__

// =============================================================================
// CONFIGURATION VALIDATION
// =============================================================================

#if WIINGPONG_DEBUG_ENABLED && (WIINGPONG_DEBUG_TOGGLE_METHOD < 0 || WIINGPONG_DEBUG_TOGGLE_METHOD > 2)
#error "WIINGPONG_DEBUG_TOGGLE_METHOD must be 0, 1, or 2"
#endif

#if WIINGPONG_DEBUG_ENABLED && WIINGPONG_DEBUG_TOGGLE_DURATION_FRAMES < 60
#error "WIINGPONG_DEBUG_TOGGLE_DURATION_FRAMES must be at least 60 (1 second)"
#endif
