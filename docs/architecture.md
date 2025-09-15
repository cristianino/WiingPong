# WiingPong Modular Architecture

## Overview

WiingPong is a table tennis game for Wii homebrew, refactored into modular components for collaborative development. The architecture separates input, physics, rendering, audio, UI, game state, assets, and core engine. It uses lightweight ECS for entities and Observer for events.

## Core Modules

- **Input**: Wiimote handling via [`InputManager.h`](include/input/InputManager.h).
- **Physics**: Collision and movement via [`PhysicsEngine.h`](include/physics/PhysicsEngine.h) with components [`Components.h`](include/physics/Components.h).
- **Rendering**: GRRLIB drawing via [`Renderer.h`](include/rendering/Renderer.h).
- **Audio**: Stub for SFX/BGM via [`AudioManager.h`](include/audio/AudioManager.h).
- **UI**: Placeholder for menus.
- **Game State**: State transitions via [`GameStateManager.h`](include/gamestate/GameStateManager.h).
- **Assets**: Singleton loader via [`AssetManager.h`](include/assets/AssetManager.h).

## Directory Structure

- `include/`: Public headers.
- `source/`: Implementations per module.
- `tests/`: Unit tests (e.g., [`test_physics.cpp`](tests/unit/test_physics.cpp)).
- `docs/`: This document.
- `assets/`: Resources (empty for now).

## Design Patterns

- ECS-lite: Components for entities.
- State Pattern: GameStateManager.
- Singleton: AssetManager.
- Observer: Future event bus.

## Collaboration

- Tests: GTest for host builds.
- CI: GitHub Actions for compilation.
- Docs: Doxygen-ready comments.

## Next Steps

- Integrate game state into main.cpp.
- Add UI module.
- Implement event bus.
- Run tests and build with CMake.