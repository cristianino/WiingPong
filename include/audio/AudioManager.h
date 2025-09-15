#pragma once

#include <cstddef>

// Audio management using Wii ASND library
// Play SFX and BGM; supports PCM audio files

enum class SoundID {
    Hit,
    Score,
    MenuClick,
    BackgroundMusic,
    Intro
};

struct AudioBuffer {
    void* data;
    size_t size;
    int frequency;
    int format;
};

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    void init();
    bool loadPCM(SoundID id, const char* filename);
    void playSound(SoundID id);
    void playMusic(SoundID id, bool loop = false);
    void setVolume(float volume);  // 0.0 to 1.0
    void stopAll();

private:
    bool initialized;
    AudioBuffer audioBuffers[5];  // Storage for audio data
    
    AudioBuffer* getAudioBuffer(SoundID id);
    void freeAudioBuffer(SoundID id);
};