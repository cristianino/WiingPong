#pragma once

// Wrapper header to handle WPAD function availability
// This ensures compatibility between real hardware and CI builds

#include <wiiuse/wpad.h>

// Check if we're in a CI environment or if WPAD functions are not available
// We'll detect this by trying to use a simple compilation test

// First, try to include the real WPAD functions
#ifdef __GNUC__
// GCC-specific: We can use __has_include to check for header availability
#if __has_include(<wiiuse/wpad.h>)
    // Real WPAD functions should be available from the included header
    // But let's add a compile-time check
    
    // If this is a syntax-only check or CI build, provide mocks
    #ifdef __SYNTAX_CHECK_ONLY__
        #define WPAD_MOCK_MODE 1
    #endif
#endif
#endif

// If we're in mock mode or functions aren't available, provide mock implementations
#ifdef WPAD_MOCK_MODE

// Mock types
typedef struct {
    unsigned char data[32];
} WPADEncStatus;

// Mock function declarations
extern "C" {
    int WPAD_ControlSpeaker(int chan, int enable);
    int WPAD_IsSpeakerEnabled(int chan);
    int WPAD_SendStreamData(int chan, void* buf, unsigned int len);
    void WPAD_EncodeData(WPADEncStatus* info, unsigned int flag, const signed short* pcmSamples, int numSamples, unsigned char* encData);
}

#endif // WPAD_MOCK_MODE
