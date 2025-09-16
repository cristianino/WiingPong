// source/WiimoteManager.cpp
#include "WiimoteManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>  // for usleep

// Compatibility definitions for CI builds
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

// Mock definitions for CI builds if WPAD functions are not available
#ifdef CI_BUILD
// Mock structure for CI
typedef struct {
    u8 data[32];
} WPADEncStatus;

// Mock functions for CI
static s32 WPAD_ControlSpeaker(s32 chan, s32 enable) { (void)chan; (void)enable; return 0; }
static s32 WPAD_IsSpeakerEnabled(s32 chan) { (void)chan; return 1; }
static s32 WPAD_SendStreamData(s32 chan, void* buf, u32 len) { (void)chan; (void)buf; (void)len; return 0; }
static void WPAD_EncodeData(WPADEncStatus* info, u32 flag, const s16* pcmSamples, s32 numSamples, u8* encData) {
    (void)info; (void)flag; (void)pcmSamples; (void)numSamples; (void)encData;
}
#endif

WiimoteManager::WiimoteManager() : speakerInitialized(false) {
    // Initialize audio buffers
    for (int i = 0; i < 3; i++) {
        audioBuffers[i].pcmData = nullptr;
        audioBuffers[i].sampleCount = 0;
        audioBuffers[i].fileSize = 0;
    }
}

WiimoteManager::~WiimoteManager() {
    // Free all audio buffers
    for (int i = 0; i < 3; i++) {
        freeWiimoteAudioBuffer(static_cast<WiimoteSoundID>(i));
    }
    if (speakerInitialized) {
        stopSpeaker(0);
    }
}

void WiimoteManager::init() {
    WPAD_Init();
}

void WiimoteManager::update() {
    WPAD_ScanPads();
}

bool WiimoteManager::isHomePressed() {
    return WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME;
}

bool WiimoteManager::initSpeaker(int channel) {
    if (speakerInitialized) return true;
    
    // Enable speaker on the Wiimote
    s32 result = WPAD_ControlSpeaker(channel, 1);
    if (result >= 0) {
        speakerInitialized = true;
        printf("Wiimote speaker initialized on channel %d\n", channel);
        return true;
    } else {
        printf("Failed to initialize Wiimote speaker: %d\n", result);
        return false;
    }
}

bool WiimoteManager::loadWiimoteSound(WiimoteSoundID id, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Could not open Wiimote audio file %s\n", filename);
        return false;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Calculate number of samples (16-bit = 2 bytes per sample)
    size_t sampleCount = fileSize / 2;
    
    WiimoteAudioBuffer* buffer = getWiimoteAudioBuffer(id);
    if (buffer->pcmData) {
        free(buffer->pcmData);  // Free existing data
    }
    
    buffer->pcmData = (signed short*)malloc(fileSize);
    if (!buffer->pcmData) {
        printf("Error: Could not allocate memory for Wiimote audio file\n");
        fclose(file);
        return false;
    }
    
    // Read file data as 16-bit samples
    size_t bytesRead = fread(buffer->pcmData, 1, fileSize, file);
    fclose(file);
    
    if (bytesRead != (size_t)fileSize) {
        printf("Error: Could not read complete Wiimote audio file\n");
        free(buffer->pcmData);
        buffer->pcmData = nullptr;
        return false;
    }
    
    buffer->sampleCount = sampleCount;
    buffer->fileSize = fileSize;
    printf("Loaded Wiimote audio file %s (%ld bytes, %zu samples)\n", filename, fileSize, sampleCount);
    return true;
}

void WiimoteManager::playWiimoteSound(WiimoteSoundID id, int channel) {
    if (!speakerInitialized) {
        if (!initSpeaker(channel)) return;
    }
    
    WiimoteAudioBuffer* buffer = getWiimoteAudioBuffer(id);
    if (!buffer->pcmData || buffer->sampleCount == 0) return;
    
    // Check if speaker is still enabled
    if (!WPAD_IsSpeakerEnabled(channel)) {
        printf("Wiimote speaker not enabled on channel %d\n", channel);
        return;
    }
    
    // Encode and send audio data
    // Note: For simplicity, we'll send the raw data in chunks
    // The Wiimote speaker has limited buffer, so we need to be careful
    const size_t CHUNK_SIZE = 20;  // Small chunks for Wiimote speaker
    size_t samplesRemaining = buffer->sampleCount;
    signed short* currentData = buffer->pcmData;
    
    while (samplesRemaining > 0) {
        size_t chunkSamples = (samplesRemaining > CHUNK_SIZE) ? CHUNK_SIZE : samplesRemaining;
        
        // Encode data for Wiimote
        WPADEncStatus encStatus;
        u8 encodedData[40];  // Buffer for encoded data
        
        WPAD_EncodeData(&encStatus, 0, (const s16*)currentData, chunkSamples, encodedData);
        
        // Send encoded data to Wiimote speaker
        WPAD_SendStreamData(channel, encodedData, sizeof(encodedData));
        
        currentData += chunkSamples;
        samplesRemaining -= chunkSamples;
        
        // Small delay to prevent overwhelming the Wiimote
        // Note: In a real game loop, this would be handled differently
        usleep(1000);  // 1ms delay
    }
}

void WiimoteManager::stopSpeaker(int channel) {
    WPAD_ControlSpeaker(channel, 0);
    printf("Wiimote speaker stopped on channel %d\n", channel);
}

WiimoteAudioBuffer* WiimoteManager::getWiimoteAudioBuffer(WiimoteSoundID id) {
    int index = static_cast<int>(id);
    if (index >= 0 && index < 3) {
        return &audioBuffers[index];
    }
    return nullptr;
}

void WiimoteManager::freeWiimoteAudioBuffer(WiimoteSoundID id) {
    WiimoteAudioBuffer* buffer = getWiimoteAudioBuffer(id);
    if (buffer && buffer->pcmData) {
        free(buffer->pcmData);
        buffer->pcmData = nullptr;
        buffer->sampleCount = 0;
        buffer->fileSize = 0;
    }
}
