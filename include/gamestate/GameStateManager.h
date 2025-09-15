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

    // Game data (scores; expand for high scores, etc.)
    int playerScore;
    int cpuScore;
    bool isRunning;

private:
    GameStateType currentState;
    void enterState(GameStateType state);
    void exitState(GameStateType state);
};