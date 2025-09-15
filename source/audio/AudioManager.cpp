// source/audio/AudioManager.cpp
#include "audio/AudioManager.h"
#include <asndlib.h>  // Wii audio library

AudioManager::AudioManager() : initialized(false) {
}

AudioManager::~AudioManager() {
    if (initialized) {
        stopAll();
    }
}

void AudioManager::init() {
    if (initialized) return;
    ASND_Init();  // Initialize Wii audio
    initialized = true;
}

void AudioManager::playSound(SoundID id) {
    // Stub: Play SFX; load from assets later
    // e.g., ASND_PlayVoice(soundBuffer, size, frequency, volume, pan);
    // For now, no-op
}

void AudioManager::playMusic(SoundID id, bool loop) {
    // Stub: Play BGM
    // e.g., ASND_SetBGM for background
    // No-op
}

void AudioManager::setVolume(float volume) {
    // Stub: Set global volume
    // e.g., ASND_SetVolume(volume * 255);
}

void AudioManager::stopAll() {
    if (!initialized) return;
    ASND_StopVoice(0);  // Stop all voices; stub
}