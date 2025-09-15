#pragma once

// Stub for audio management using Wii ASND or similar
// Play SFX and BGM; expand with libmad for MP3 or BRSTM

enum class SoundID {
    Hit,
    Score,
    MenuClick,
    BackgroundMusic
};

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    void init();
    void playSound(SoundID id);
    void playMusic(SoundID id, bool loop = false);
    void setVolume(float volume);  // 0.0 to 1.0
    void stopAll();

private:
    bool initialized;
    // Stubs for sound buffers; load from assets later
};