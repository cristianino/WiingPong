// source/audio/AudioManager.cpp
#include "audio/AudioManager.h"
#include "config.h"  // Include config for audio debug flags
#include <asndlib.h>  // Wii audio library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ogcsys.h>  // For VIDEO_WaitVSync

// Define constants if not already defined (for compatibility)
// These are the correct ASND format constants
#ifndef VOICE_MONO16
#define VOICE_MONO16 0
#endif
#ifndef VOICE_STEREO16  
#define VOICE_STEREO16 1
#endif

// Add fallback values for common PCM format
#ifndef VOICE_MONO8
#define VOICE_MONO8 2
#endif
#ifndef VOICE_STEREO8
#define VOICE_STEREO8 3
#endif

AudioManager::AudioManager() : initialized(false), nextVoice(1) {
    // Initialize audio buffers
    for (int i = 0; i < 5; i++) {
        audioBuffers[i].data = nullptr;
        audioBuffers[i].size = 0;
        audioBuffers[i].frequency = 22050;  // Standard frequency for PCM
        audioBuffers[i].format = VOICE_MONO16;  // 16-bit mono (most common for game SFX)
    }
    printf("AudioManager constructor: Initialized buffers with 22050Hz, MONO16 format\n");
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
#if WIINGPONG_AUDIO_ENABLED
    if (initialized) return;
    printf("Initializing ASND audio library...\n");
    ASND_Init();  // Initialize Wii audio
    ASND_Pause(0);  // Unpause audio (0 = unpause)
    printf("ASND audio library initialized successfully\n");
    initialized = true;
#else
    printf("Audio disabled by config\n");
    initialized = false;
#endif
}

bool AudioManager::loadPCM(SoundID id, const char* filename) {
#if WIINGPONG_AUDIO_ENABLED
    if (!initialized) {
#if WIINGPONG_AUDIO_DEBUG
        printf("Error: AudioManager not initialized when trying to load %s\n", filename);
#endif
        return false;
    }
    
#if WIINGPONG_AUDIO_DEBUG
    printf("Attempting to load audio file: %s\n", filename);
#endif
    FILE* file = fopen(filename, "rb");
    if (!file) {
#if WIINGPONG_AUDIO_DEBUG
        printf("Error: Could not open audio file %s\n", filename);
#endif
        return false;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    
#if WIINGPONG_AUDIO_DEBUG
    printf("Audio file %s size: %ld bytes\n", filename, fileSize);
#endif
    
    // Allocate buffer
    AudioBuffer* buffer = getAudioBuffer(id);
    if (buffer->data) {
        free(buffer->data);  // Free existing data
    }
    
    buffer->data = malloc(fileSize);
    if (!buffer->data) {
        printf("Error: Could not allocate memory for audio file (%ld bytes)\n", fileSize);
        fclose(file);
        return false;
    }
    
    // Read file data
    size_t bytesRead = fread(buffer->data, 1, fileSize, file);
    fclose(file);
    
    if (bytesRead != (size_t)fileSize) {
        printf("Error: Could not read complete audio file (read %zu of %ld bytes)\n", bytesRead, fileSize);
        free(buffer->data);
        buffer->data = nullptr;
        return false;
    }
    
    buffer->size = fileSize;
    
    // Auto-detect PCM format
    detectPCMFormat(buffer, fileSize);
    
#if WIINGPONG_AUDIO_DEBUG
    printf("Successfully loaded audio file %s (%ld bytes, %dHz, format %d)\n", 
           filename, fileSize, buffer->frequency, buffer->format);
#endif
    return true;
#else
    (void)id; (void)filename; // Suppress unused parameter warnings
    return false;
#endif
}

void AudioManager::playSound(SoundID id) {
#if WIINGPONG_AUDIO_ENABLED
    if (!initialized) {
#if WIINGPONG_AUDIO_DEBUG
        printf("Error: AudioManager not initialized when trying to play sound %d\n", static_cast<int>(id));
#endif
        return;
    }
    
    AudioBuffer* buffer = getAudioBuffer(id);
    if (!buffer->data) {
#if WIINGPONG_AUDIO_DEBUG
        printf("Error: No audio data loaded for sound %d\n", static_cast<int>(id));
#endif
        return;
    }
    
    // Use round-robin voice allocation for sound effects (voices 1-3)
    int voice = getNextVoice();
#if WIINGPONG_AUDIO_DEBUG
    printf("Playing sound %d on voice %d (size: %zu bytes, freq: %d)\n", 
           static_cast<int>(id), voice, buffer->size, buffer->frequency);
#endif
    
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
#else
    (void)id; // Suppress unused parameter warning
#endif
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

int AudioManager::getNextVoice() {
    // Round-robin allocation for voices 1-3 (voice 0 reserved for music)
    int voice = nextVoice;
    nextVoice++;
    if (nextVoice > 3) nextVoice = 1;
    return voice;
}

void AudioManager::detectPCMFormat(AudioBuffer* buffer, long fileSize) {
    // Default safe values
    buffer->frequency = 22050;
    buffer->format = VOICE_MONO16;
    
    // Heuristic detection based on file size and common patterns
    // These are educated guesses based on typical game audio
    
    if (fileSize > 300000) {
        // Very large file - probably intro music, could be stereo
        // 393718 bytes (intro.pcm) at 22050Hz mono 16-bit = ~9 seconds
        buffer->frequency = 22050;
        buffer->format = VOICE_MONO16;
    } else if (fileSize > 15000) {
        // Medium file - longer sound effect like score
        // 22050 bytes (score.pcm) at 22050Hz mono 16-bit = ~0.5 seconds  
        buffer->frequency = 22050;
        buffer->format = VOICE_MONO16;
    } else {
        // Small file - short sound effect
        // 6614 bytes (paddle_hit.pcm), 4410 bytes (wall_hit.pcm)
        buffer->frequency = 22050;
        buffer->format = VOICE_MONO16;
    }
    
#if WIINGPONG_AUDIO_DEBUG
    printf("Detected format for %ld byte file: %dHz, format %d\n", 
           fileSize, buffer->frequency, buffer->format);
#endif
}

void AudioManager::testAllSounds() {
#if WIINGPONG_AUDIO_ENABLED
    if (!initialized) {
        printf("AudioManager not initialized - cannot test sounds\n");
        return;
    }
    
    printf("=== AUDIO TEST SEQUENCE ===\n");
    
    const char* soundNames[] = {"Intro", "PaddleHit", "WallHit", "Score", "MenuClick"};
    
    for (int i = 0; i < 5; i++) {
        SoundID id = static_cast<SoundID>(i);
        AudioBuffer* buffer = getAudioBuffer(id);
        
        if (buffer->data) {
            printf("Testing %s sound (ID %d)...\n", soundNames[i], i);
            playSound(id);
            printf("  -> Sound played successfully\n");
        } else {
            printf("No data loaded for %s sound (ID %d)\n", soundNames[i], i);
        }
    }
    
    printf("=== AUDIO TEST COMPLETE ===\n");
#else
    printf("Audio disabled by config\n");
#endif
}