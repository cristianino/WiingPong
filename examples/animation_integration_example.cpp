// Example integration for UI transitions and animations
// This shows how to integrate the animation system with game events

#include "rendering/Renderer.h"
#include "physics/PhysicsEngine.h"
#include "gamestate/GameStateManager.h"

// Example: Game event handlers using animation system

class GameEventHandler
{
private:
    Renderer *renderer;

public:
    GameEventHandler(Renderer *r) : renderer(r) {}

    // Round start sequence
    void onRoundStart()
    {
        // Fade in from black
        renderer->startScreenFadeIn(1.0f);
    }

    // Round end sequence
    void onRoundEnd()
    {
        // Fade out to black
        renderer->startScreenFadeOut(1.0f);
    }

    // Player scores
    void onPlayerScore(float ballX, float ballY)
    {
        // Camera shake for impact
        renderer->triggerCameraShake(8.0f, 0.5f);

        // Gold particles at score location
        renderer->spawnImpactParticles(ballX, ballY, 0xFFD700FF, 12);
    }

    // CPU scores
    void onCPUScore(float ballX, float ballY)
    {
        // Weaker shake for CPU score
        renderer->triggerCameraShake(5.0f, 0.3f);

        // Red particles
        renderer->spawnImpactParticles(ballX, ballY, 0xFF4444FF, 8);
    }

    // Ball hits paddle
    void onPaddleHit(float ballX, float ballY, bool isPlayerPaddle)
    {
        // Quick shake
        renderer->triggerCameraShake(3.0f, 0.2f);

        // Color based on paddle
        u32 color = isPlayerPaddle ? 0x4488FFFF : 0xFF4488FF;
        renderer->spawnImpactParticles(ballX, ballY, color, 5);
    }

    // Ball hits wall
    void onWallHit(float ballX, float ballY)
    {
        // Minimal shake
        renderer->triggerCameraShake(2.0f, 0.15f);

        // White particles
        renderer->spawnImpactParticles(ballX, ballY, 0xFFFFFFFF, 3);
    }

    // Intense mode activation (3-3+ score tie)
    void onIntenseModeActivation()
    {
        // Strong camera shake
        renderer->triggerCameraShake(15.0f, 1.0f);

        // Red screen flash
        renderer->startScreenFadeInOut(1.5f, 0xFF0000FF);

        // Particles burst from center
        float centerX = 320.0f;
        float centerY = 240.0f;
        renderer->spawnImpactParticles(centerX, centerY, 0xFF8800FF, 20);
    }

    // Game state transition
    void onStateTransition(GameState fromState, GameState toState)
    {
        switch (toState)
        {
        case GameState::Playing:
            if (fromState == GameState::Menu)
            {
                renderer->startScreenFadeIn(0.8f);
            }
            break;

        case GameState::Menu:
            if (fromState == GameState::Playing)
            {
                renderer->startScreenFadeOut(0.8f);
            }
            break;

        case GameState::Paused:
            // Quick fade for pause
            renderer->startScreenFadeInOut(0.4f, 0x000000AA);
            break;
        }
    }

    // Menu selection
    void onMenuSelection(float menuX, float menuY)
    {
        // Green particles for selection
        renderer->spawnImpactParticles(menuX, menuY, 0x00FF00FF, 4);
    }
};

// Example: Integration in main game loop
void gameLoop()
{
    static GameEventHandler eventHandler(&renderer);
    static int lastPlayerScore = 0;
    static int lastCPUScore = 0;
    static bool wasInIntenseMode = false;

    // Update physics and detect events
    physics.update(deltaTime);

    // Check for scoring events
    if (physics.playerScore > lastPlayerScore)
    {
        eventHandler.onPlayerScore(physics.positions[BALL].x, physics.positions[BALL].y);
        lastPlayerScore = physics.playerScore;
    }

    if (physics.cpuScore > lastCPUScore)
    {
        eventHandler.onCPUScore(physics.positions[BALL].x, physics.positions[BALL].y);
        lastCPUScore = physics.cpuScore;
    }

    // Check for intense mode transition
    bool isIntenseMode = (physics.playerScore >= 3 && physics.cpuScore >= 3);
    if (isIntenseMode && !wasInIntenseMode)
    {
        eventHandler.onIntenseModeActivation();
        wasInIntenseMode = true;
    }

    // Update renderer (includes animation updates)
    renderer.update(deltaTime, physics);

    // Render frame
    renderer.render(physics);
}

// Example: Collision detection with animation feedback
void handleCollisions()
{
    CollisionResult result = physics.checkCollisions();

    switch (result.type)
    {
    case CollisionType::BallPaddle:
        eventHandler.onPaddleHit(
            result.position.x,
            result.position.y,
            result.isPlayerPaddle);
        break;

    case CollisionType::BallWall:
        eventHandler.onWallHit(result.position.x, result.position.y);
        break;

    case CollisionType::BallGoal:
        // Score event will be handled in main loop
        break;
    }
}

// Example: Menu integration
void handleMenuInput()
{
    if (input.isButtonPressed(WPAD_BUTTON_A))
    {
        // Get current menu item position
        float menuX = getMenuItemX();
        float menuY = getMenuItemY();

        // Visual feedback for selection
        eventHandler.onMenuSelection(menuX, menuY);

        // Process menu action after a brief delay for animation
        // (or use isTransitionActive() to wait for completion)
    }
}

// Example: Advanced timing control
void waitForTransitionComplete()
{
    while (renderer.isTransitionActive())
    {
        // Update only the renderer during transition
        renderer.update(deltaTime, physics);
        renderer.render(physics);

        // Process input for early exit if needed
        if (input.isButtonPressed(WPAD_BUTTON_HOME))
        {
            renderer.resetAllAnimations();
            break;
        }
    }
}

/*
Integration Notes:

1. Animation Triggers:
   - Call animation methods in response to game events
   - Use consistent timing for similar events
   - Scale intensity based on game state (normal vs intense mode)

2. Performance:
   - Animation updates are automatic in renderer.update()
   - Particles self-cleanup when life expires
   - Use isTransitionActive() for state management

3. Visual Consistency:
   - Match particle colors to game elements
   - Scale shake intensity with impact magnitude
   - Use fade transitions for clean state changes

4. User Experience:
   - Provide immediate feedback for player actions
   - Don't overwhelm with too many simultaneous effects
   - Allow players to skip long transitions if needed

5. Atlas Integration:
   - Effects automatically adapt to current atlas (normal/intense)
   - Consider stronger effects for intense mode
   - Maintain visual coherence with existing effects (parallax, shadows)
*/