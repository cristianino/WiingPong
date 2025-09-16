#include "wpad_wrapper.h"
#include <stdio.h>

// Mock implementations for WPAD functions when not available
#ifdef WPAD_MOCK_MODE

extern "C" {
    int WPAD_ControlSpeaker(int chan, int enable) {
        printf("Mock WPAD_ControlSpeaker called (chan=%d, enable=%d)\n", chan, enable);
        return 0; // Success
    }

    int WPAD_IsSpeakerEnabled(int chan) {
        printf("Mock WPAD_IsSpeakerEnabled called (chan=%d)\n", chan);
        return 1; // Speaker is enabled
    }

    int WPAD_SendStreamData(int chan, void* buf, unsigned int len) {
        printf("Mock WPAD_SendStreamData called (chan=%d, len=%u)\n", chan, len);
        return 0; // Success
    }

    void WPAD_EncodeData(WPADEncStatus* info, unsigned int flag, const signed short* pcmSamples, int numSamples, unsigned char* encData) {
        printf("Mock WPAD_EncodeData called (numSamples=%d)\n", numSamples);
        // Mock encoding - just fill with zeros
        if (encData) {
            for (int i = 0; i < 40; i++) {
                encData[i] = 0;
            }
        }
    }
}

#endif // WPAD_MOCK_MODE
