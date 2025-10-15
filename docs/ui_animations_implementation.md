# UI Transitions and Animations System

## Overview

The WiingPong UI transitions and animations system provides smooth visual effects and feedback to enhance the gaming experience. This system includes screen transitions, camera shake effects, particle systems, and UI animations that respond to gameplay events.

## System Architecture

### Core Components

#### 1. Particle System
```cpp
struct Particle {
    float x, y;           // Current position
    float vx, vy;         // Velocity
    float life;           // Remaining life (0.0 to 1.0)
    float maxLife;        // Maximum life duration
    float size;           // Current size
    float rotation;       // Current rotation
    u32 color;           // Current color
};
```

**Features:**
- Automatic lifecycle management
- Physics-based movement
- Color fading over time
- Collision and scoring effects

#### 2. Camera Shake
```cpp
struct CameraShake {
    float intensity;      // Shake intensity
    float duration;       // Total duration
    float timeLeft;       // Time remaining
    float frequency;      // Shake frequency (default: 15.0f Hz)
    float offsetX, offsetY; // Current offset
};
```

**Features:**
- Configurable intensity and duration
- Sinusoidal movement pattern
- Automatic decay and cleanup
- Impact feedback for ball collisions

#### 3. Fade Transitions
```cpp
struct FadeTransition {
    enum FadeType { None, FadeIn, FadeOut, FadeInOut };
    
    FadeType type;
    float duration;       // Total duration
    float timeLeft;       // Time remaining
    float alpha;          // Current alpha value
    u32 color;           // Fade color
};
```

**Features:**
- Multiple transition types
- Configurable color and duration
- Smooth alpha interpolation
- Round start/end transitions

#### 4. UI Animations
```cpp
struct UIAnimation {
    enum AnimType { Slide, Scale, Rotate, Pulse };
    
    AnimType type;
    float duration;
    float timeLeft;
    float startValue;
    float endValue;
    float currentValue;
    bool looping;
};
```

**Features:**
- Multiple animation types
- Value interpolation
- Optional looping
- Menu and UI enhancements

## Public API

### Screen Transitions
```cpp
// Fade from black to transparent (game start)
void startScreenFadeIn(float duration = 1.0f, u32 color = 0x000000FF);

// Fade from transparent to black (game end)
void startScreenFadeOut(float duration = 1.0f, u32 color = 0x000000FF);

// Fade out then in (round transition)
void startScreenFadeInOut(float duration = 2.0f, u32 color = 0x000000FF);
```

### Camera Effects
```cpp
// Trigger camera shake on impacts
void triggerCameraShake(float intensity = 5.0f, float duration = 0.5f);
```

### Particle Effects
```cpp
// Spawn particles at collision points
void spawnImpactParticles(float x, float y, u32 color = 0xFFFFFFFF, int count = 5);
```

### Animation Control
```cpp
// Check if any animations are active
bool isTransitionActive() const;

// Reset all animations and effects
void resetAllAnimations();
```

## Integration Points

### Update Loop
The animation system is automatically updated in `Renderer::update()`:
```cpp
void Renderer::update(float deltaTime, const PhysicsEngine &physics) {
    // ... existing code ...
    
    // Update animation systems
    updateParticles(deltaTime);
    updateCameraShake(deltaTime);
    updateFadeTransition(deltaTime);
    updateUIAnimations(deltaTime);
}
```

### Rendering Pipeline
Animations are rendered in the correct order in `Renderer::render()`:
```cpp
void Renderer::render(const PhysicsEngine &physics) {
    // ... game objects rendering ...
    
    // Apply camera shake offset if active
    // (affects all subsequent rendering)
    
    // Render particles (overlay on top of everything)
    renderParticles();
    
    // Render fade transition (final overlay)
    renderFadeTransition();
}
```

## Usage Examples

### Game State Transitions
```cpp
// Round start
renderer.startScreenFadeIn(1.0f);

// Round end
renderer.startScreenFadeOut(1.0f);

// Between rounds
renderer.startScreenFadeInOut(2.0f);
```

### Gameplay Feedback
```cpp
// Ball hits paddle
renderer.triggerCameraShake(5.0f, 0.3f);
renderer.spawnImpactParticles(ballX, ballY, 0xFFFFFFFF, 5);

// Player scores
renderer.triggerCameraShake(10.0f, 0.5f);
renderer.spawnImpactParticles(goalX, goalY, 0xFFD700FF, 10);

// Intense mode activation (3-3+ score)
renderer.triggerCameraShake(15.0f, 1.0f);
renderer.startScreenFadeInOut(1.5f, 0xFF0000FF); // Red flash
```

### Menu Interactions
```cpp
// Menu item selection
if (menuItemSelected) {
    renderer.spawnImpactParticles(menuX, menuY, 0x00FF00FF, 3);
}

// Menu transition
renderer.startScreenFadeOut(0.5f);
// ... change game state ...
renderer.startScreenFadeIn(0.5f);
```

## Atlas Integration

The animation system works seamlessly with the existing multi-atlas system:

- **Normal Mode (atlas.png)**: Subtle animations with lower intensity
- **Intense Mode (atlas_2.png)**: Enhanced effects with higher intensity and different colors

```cpp
// Effects automatically adapt to current atlas
AtlasType currentAtlas = assets.getCurrentAtlas();
if (currentAtlas == AtlasType::Intense) {
    // Stronger effects for intense mode
    renderer.triggerCameraShake(15.0f, 0.8f);
} else {
    // Regular effects for normal mode
    renderer.triggerCameraShake(8.0f, 0.4f);
}
```

## Performance Considerations

### Efficient Particle Management
- Particles automatically removed when life reaches 0
- Vector cleanup prevents memory accumulation
- Configurable particle counts for performance tuning

### Camera Shake Optimization
- Single shake instance (no accumulation)
- Sinusoidal calculations only when active
- Automatic cleanup when duration expires

### Fade Transition Efficiency
- Full-screen overlay only when transition is active
- Alpha calculations optimized for smooth interpolation
- Minimal overdraw with proper ordering

## Future Enhancements

The system is designed for extensibility:

1. **Advanced Particle Types**: Different shapes, physics behaviors
2. **Complex UI Animations**: Multi-stage animations, easing functions
3. **Audio-Visual Sync**: Animations triggered by audio events
4. **Performance Scaling**: Adaptive quality based on system performance

## Testing

Use the provided test script to verify system integrity:
```bash
./test_animation_system.sh
```

This script checks:
- Compilation success
- Structure definitions
- API availability
- Integration completeness
- Initialization correctness

## Conclusion

The UI transitions and animations system adds professional polish to WiingPong, providing:
- **Enhanced Visual Feedback**: Players get immediate response to their actions
- **Smooth Transitions**: Seamless flow between game states
- **Gameplay Integration**: Effects that enhance rather than distract
- **Scalable Architecture**: Easy to extend and customize

The system complements the existing visual effects (parallax backgrounds, shadows/trails) to create a cohesive and engaging visual experience.