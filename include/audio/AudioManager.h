#pragma once

#include <cstddef>

// Audio management using Wii ASND library
// Play SFX and BGM; supports PCM audio files

enum class SoundID {
    Intro = 0,
    PaddleHit = 1,
    WallHit = 2,
    Score = 3,
    MenuClick = 4
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
    
    // Debug functions
    void testAllSounds();  // Test all loaded sounds
    bool isInitialized() const { return initialized; }

private:
    bool initialized;
    AudioBuffer audioBuffers[5];  // Storage for audio data
    int nextVoice;               // For round-robin voice allocation
    
    AudioBuffer* getAudioBuffer(SoundID id);
    void freeAudioBuffer(SoundID id);
    int getNextVoice();          // Get next available voice for sound effects
    void detectPCMFormat(AudioBuffer* buffer, long fileSize);  // Auto-detect PCM format
};