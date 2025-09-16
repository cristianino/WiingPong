// include/WiimoteManager.h
#pragma once

// Conditional includes based on build environment
#ifdef MOCK_WPAD_FUNCTIONS
// For CI/mock builds, define types locally
#include <cstddef>
#ifndef s16
typedef signed short s16;
#endif
#ifndef s32
typedef signed int s32;
#endif
#ifndef u8
typedef unsigned char u8;
#endif
#ifndef u32
typedef unsigned int u32;
#endif
#else
// For real hardware builds, include WPAD headers
#include <wiiuse/wpad.h>
#include <gctypes.h>  // For s16, s32, etc.
#include <cstddef>
#endif

// Sound effects for Wiimote speaker
enum class WiimoteSoundID {
    PlayerPaddleHit = 0,
    PlayerScore = 1,
    PlayerLoss = 2
};

struct WiimoteAudioBuffer {
    signed short* pcmData;  // PCM data as 16-bit signed integers (more compatible than s16*)
    size_t sampleCount;     // Number of samples
    size_t fileSize;        // Original file size in bytes
};

class WiimoteManager {
public:
    WiimoteManager();
    ~WiimoteManager();
    
    void init();
    void update();
    bool isHomePressed();
    
    // Speaker functions
    bool initSpeaker(int channel = 0);
    bool loadWiimoteSound(WiimoteSoundID id, const char* filename);
    void playWiimoteSound(WiimoteSoundID id, int channel = 0);
    void stopSpeaker(int channel = 0);

private:
    WiimoteAudioBuffer audioBuffers[3];  // Storage for Wiimote audio
    bool speakerInitialized;
    
    WiimoteAudioBuffer* getWiimoteAudioBuffer(WiimoteSoundID id);
    void freeWiimoteAudioBuffer(WiimoteSoundID id);
};
