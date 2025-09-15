// source/audio/AudioManager.cpp
#include "audio/AudioManager.h"
#include <asndlib.h>  // Wii audio library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define constants if not already defined (for compatibility)
#ifndef VOICE_MONO16
#define VOICE_MONO16 0
#endif
#ifndef VOICE_STEREO16  
#define VOICE_STEREO16 1
#endif

AudioManager::AudioManager() : initialized(false) {
    // Initialize audio buffers
    for (int i = 0; i < 5; i++) {
        audioBuffers[i].data = nullptr;
        audioBuffers[i].size = 0;
        audioBuffers[i].frequency = 22050;  // Default frequency
        audioBuffers[i].format = VOICE_MONO16;  // Default format
    }
}

AudioManager::~AudioManager() {
    if (initialized) {
        stopAll();
        // Free all audio buffers
        for (int i = 0; i < 5; i++) {
            freeAudioBuffer(static_cast<SoundID>(i));
        }
    }
}

void AudioManager::init() {
    if (initialized) return;
    ASND_Init();  // Initialize Wii audio
    initialized = true;
}

bool AudioManager::loadPCM(SoundID id, const char* filename) {
    if (!initialized) return false;
    
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Could not open audio file %s\n", filename);
        return false;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Allocate buffer
    AudioBuffer* buffer = getAudioBuffer(id);
    if (buffer->data) {
        free(buffer->data);  // Free existing data
    }
    
    buffer->data = malloc(fileSize);
    if (!buffer->data) {
        printf("Error: Could not allocate memory for audio file\n");
        fclose(file);
        return false;
    }
    
    // Read file data
    size_t bytesRead = fread(buffer->data, 1, fileSize, file);
    fclose(file);
    
    if (bytesRead != (size_t)fileSize) {
        printf("Error: Could not read complete audio file\n");
        free(buffer->data);
        buffer->data = nullptr;
        return false;
    }
    
    buffer->size = fileSize;
    printf("Loaded audio file %s (%ld bytes)\n", filename, fileSize);
    return true;
}

void AudioManager::playSound(SoundID id) {
    if (!initialized) return;
    
    AudioBuffer* buffer = getAudioBuffer(id);
    if (!buffer->data) return;
    
    // Play sound using ASND
    // Use voice 1-3 for sound effects (voice 0 reserved for music)
    int voice = 1;  // Simple voice allocation, could be improved
    ASND_SetVoice(voice, 
                  buffer->format, 
                  buffer->frequency, 
                  0,  // delay
                  buffer->data, 
                  buffer->size, 
                  255,  // volume (max)
                  255,  // volume (max)
                  NULL  // callback
    );
}

void AudioManager::playMusic(SoundID id, bool loop) {
    if (!initialized) return;
    
    AudioBuffer* buffer = getAudioBuffer(id);
    if (!buffer->data) return;
    
    // For background music, use voice 0
    ASND_SetVoice(0, 
                  buffer->format, 
                  buffer->frequency, 
                  0,  // delay
                  buffer->data, 
                  buffer->size, 
                  255,  // volume (max)
                  255,  // volume (max)
                  NULL  // callback - looping handled separately
    );
    
    // Note: ASND doesn't have built-in looping, would need custom callback
    // For now, just play once
    (void)loop; // Suppress unused parameter warning
}

void AudioManager::setVolume(float volume) {
    if (!initialized) return;
    // ASND doesn't have a global volume, this is a stub
    // Individual voices can have their volume set when played
    (void)volume; // Suppress unused parameter warning
}

void AudioManager::stopAll() {
    if (!initialized) return;
    for (int i = 0; i < 4; i++) {  // Stop all 4 voices
        ASND_StopVoice(i);
    }
}

AudioBuffer* AudioManager::getAudioBuffer(SoundID id) {
    int index = static_cast<int>(id);
    if (index >= 0 && index < 5) {
        return &audioBuffers[index];
    }
    return nullptr;
}

void AudioManager::freeAudioBuffer(SoundID id) {
    AudioBuffer* buffer = getAudioBuffer(id);
    if (buffer && buffer->data) {
        free(buffer->data);
        buffer->data = nullptr;
        buffer->size = 0;
    }
}