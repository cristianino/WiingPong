#pragma once

#include <gctypes.h>  // For basic types if needed

enum class GameStateType {
    Menu,
    Play,
    Pause,
    GameOver
};

class GameStateManager {
public:
    GameStateManager();
    ~GameStateManager();

    void init();
    void update();  // Handle state-specific logic

    GameStateType getCurrentState() const { return currentState; }
    void setState(GameStateType newState);
    
    // Menu navigation methods
    void menuNavigateUp();
    void menuNavigateDown();
    void menuSelect();
    int getMenuSelectedIndex() const { return menuSelectedIndex; }
    int getMenuItemCount() const { return menuItemCount; }

    // Game data (scores; expand for high scores, etc.)
    GameStateType currentState;
    int playerScore;
    int cpuScore;
    bool isRunning;
    
    // Menu data
    int menuSelectedIndex;
    int menuItemCount;

private:
    void enterState(GameStateType state);
    void exitState(GameStateType state);
};