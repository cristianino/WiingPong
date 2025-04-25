#include <gccore.h>
#include "WiimoteManager.h"
#include <cstdio>

int main() {
    VIDEO_Init();
    GXRModeObj *rmode = VIDEO_GetPreferredMode(NULL);
    void *fb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
    console_init(fb, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);
    VIDEO_Configure(rmode);
    VIDEO_SetNextFramebuffer(fb);
    VIDEO_SetBlack(FALSE);
    VIDEO_Flush();
    VIDEO_WaitVSync();

    WiimoteManager wm;
    wm.init();

    std::printf("¡Initial project form c and C++ in Wii!\n");
    std::printf("Press HOME to exit.\n");

    while (1) {
        wm.update();
        if (wm.isHomePressed()) break;
    }

    return 0;
}
