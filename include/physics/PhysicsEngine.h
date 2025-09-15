#pragma once

#include <gctypes.h>
#include "Components.h"
#include <vector>

// Lite ECS Entity ID (fixed for simplicity: 0=player paddle, 1=CPU paddle, 2=ball)
typedef unsigned int EntityID;
const EntityID PLAYER_PADDLE = 0;
const EntityID CPU_PADDLE = 1;
const EntityID BALL = 2;
const unsigned int MAX_ENTITIES = 3;

// PhysicsEngine manages updates for entities with components
class PhysicsEngine {
public:
    PhysicsEngine();
    ~PhysicsEngine();

    void init();
    void update();  // Updates positions, velocities, collisions, AI

    // Accessors for components (public for simplicity; later encapsulate)
    Position positions[MAX_ENTITIES];
    Velocity velocities[MAX_ENTITIES];
    Size sizes[MAX_ENTITIES];
    Collider colliders[MAX_ENTITIES];
    AIControlled aiControlled[MAX_ENTITIES];

    // Scoring (temporary; move to GameState later)
    int playerScore;
    int cpuScore;

private:
    void updateAI();
    void updateBall();
    void checkCollisions();
    void checkScoring();
    bool checkAABBCollision(EntityID a, EntityID b);
};