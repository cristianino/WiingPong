// include/WiimoteManager.h
#pragma once
#include <wiiuse/wpad.h>
#include <cstddef>

// Sound effects for Wiimote speaker
enum class WiimoteSoundID {
    PlayerPaddleHit = 0,
    PlayerScore = 1,
    PlayerLoss = 2
};

struct WiimoteAudioBuffer {
    s16* pcmData;       // PCM data as 16-bit signed integers
    size_t sampleCount; // Number of samples
    size_t fileSize;    // Original file size in bytes
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
