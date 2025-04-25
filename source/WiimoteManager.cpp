// source/WiimoteManager.cpp
#include "WiimoteManager.h"

void WiimoteManager::init() {
    WPAD_Init();
}

void WiimoteManager::update() {
    WPAD_ScanPads();
}

bool WiimoteManager::isHomePressed() {
    return WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME;
}
