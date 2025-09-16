#pragma once

// WiingPong Configuration File - RELEASE VERSION
// This configuration disables debug features for production

// =============================================================================
// DEBUG CONFIGURATION
// =============================================================================

// Disable debug features completely for release
#define WIINGPONG_DEBUG_ENABLED 0

// The following debug settings are ignored when DEBUG_ENABLED = 0
#define WIINGPONG_DEBUG_TOGGLE_METHOD 1
#define WIINGPONG_DEBUG_TOGGLE_DURATION_FRAMES 240
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

#if WIINGPONG_DEBUG_ENABLED && (WIINGPONG_DEBUG_TOGGLE_METHOD < 0 || WIINGPONG_DEBUG_TOGGLE_METHOD > 1)
#error "WIINGPONG_DEBUG_TOGGLE_METHOD must be 0 or 1"
#endif

#if WIINGPONG_DEBUG_ENABLED && WIINGPONG_DEBUG_TOGGLE_DURATION_FRAMES < 60
#error "WIINGPONG_DEBUG_TOGGLE_DURATION_FRAMES must be at least 60 (1 second)"
#endif
