// include/WiimoteManager.h
#pragma once
#include <wiiuse/wpad.h>

class WiimoteManager {
public:
    void init();
    void update();
    bool isHomePressed();
};
