// source/gamestate/GameStateManager.cpp
#include "gamestate/GameStateManager.h"

GameStateManager::GameStateManager() : currentState(GameStateType::Menu), playerScore(0), cpuScore(0), isRunning(true), menuSelectedIndex(0), menuItemCount(2) {
}

GameStateManager::~GameStateManager() {
    // Cleanup if needed
}

void GameStateManager::init() {
    enterState(currentState);
}

void GameStateManager::update() {
    // State-specific updates
    if (currentState == GameStateType::Play) {
        // Game running; scores updated externally
        isRunning = true;
    } else if (currentState == GameStateType::Pause) {
        isRunning = false;
    } else if (currentState == GameStateType::GameOver) {
        // Check win condition, e.g., if (playerScore >= 10 || cpuScore >= 10)
        isRunning = false;
    }
    // Menu state: Not implemented yet
}

void GameStateManager::setState(GameStateType newState) {
    if (newState == currentState) return;

    exitState(currentState);
    currentState = newState;
    enterState(currentState);
}

void GameStateManager::enterState(GameStateType state) {
    switch (state) {
        case GameStateType::Play:
            // Start game, reset scores if new game
            playerScore = 0;
            cpuScore = 0;
            break;
        case GameStateType::Pause:
            // Pause game
            break;
        case GameStateType::GameOver:
            // End game
            break;
        case GameStateType::Menu:
            // Show menu
            break;
    }
}

void GameStateManager::exitState(GameStateType state) {
    // Cleanup for exiting state
    switch (state) {
        case GameStateType::Play:
            // Save progress if needed
            break;
        default:
            break;
    }
}

// Menu navigation methods
void GameStateManager::menuNavigateUp() {
    if (currentState == GameStateType::Menu) {
        menuSelectedIndex = (menuSelectedIndex - 1 + menuItemCount) % menuItemCount;
    }
}

void GameStateManager::menuNavigateDown() {
    if (currentState == GameStateType::Menu) {
        menuSelectedIndex = (menuSelectedIndex + 1) % menuItemCount;
    }
}

void GameStateManager::menuSelect() {
    if (currentState == GameStateType::Menu) {
        switch (menuSelectedIndex) {
            case 0: // Start Game
                setState(GameStateType::Play);
                break;
            case 1: // Exit
                isRunning = false;
                break;
        }
    }
}