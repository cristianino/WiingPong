// source/physics/PhysicsEngine.cpp
#include "physics/PhysicsEngine.h"
#include "audio/AudioManager.h"
#include <cstdlib>  // for rand
#include <time.h>

PhysicsEngine::PhysicsEngine() : playerScore(0), cpuScore(0), audioManager(nullptr) {
    // Initialize components
    // Player paddle
    positions[PLAYER_PADDLE] = {20, 200};
    velocities[PLAYER_PADDLE] = {0, 0};
    sizes[PLAYER_PADDLE] = {10, 60};
    colliders[PLAYER_PADDLE] = {true, 1};
    aiControlled[PLAYER_PADDLE] = {false};

    // CPU paddle
    positions[CPU_PADDLE] = {600, 200};
    velocities[CPU_PADDLE] = {0, 0};
    sizes[CPU_PADDLE] = {10, 60};
    colliders[CPU_PADDLE] = {true, 1};
    aiControlled[CPU_PADDLE] = {true};

    // Ball
    positions[BALL] = {320, 240};
    velocities[BALL] = {3, 2};
    sizes[BALL] = {10, 10};
    colliders[BALL] = {false, 1};  // Ball isn't solid, but collides
    aiControlled[BALL] = {false};
}

PhysicsEngine::~PhysicsEngine() {
    // Cleanup if needed
}

void PhysicsEngine::init() {
    srand((unsigned)time(NULL));
}

void PhysicsEngine::setAudioManager(AudioManager* audioManager) {
    this->audioManager = audioManager;
}

void PhysicsEngine::update() {
    // Note: For now, no input events passed; player movement will be integrated later
    // Temporary: No player movement; set dy to 0
    velocities[PLAYER_PADDLE].dy = 0;

    // Update positions for all entities
    for (unsigned int i = 0; i < MAX_ENTITIES; ++i) {
        if (i == BALL) continue;  // Ball updated separately
        positions[i].y += velocities[i].dy;
    }

    updateAI();
    updateBall();
    checkCollisions();
    checkScoring();

    // Clamp paddles
    if (positions[PLAYER_PADDLE].y < 0) positions[PLAYER_PADDLE].y = 0;
    if (positions[PLAYER_PADDLE].y > 420) positions[PLAYER_PADDLE].y = 420;
    if (positions[CPU_PADDLE].y < 0) positions[CPU_PADDLE].y = 0;
    if (positions[CPU_PADDLE].y > 420) positions[CPU_PADDLE].y = 420;
}

void PhysicsEngine::updateAI() {
    if (!aiControlled[CPU_PADDLE].isAI) return;

    const int cpuSpeed = 3;
    if (positions[CPU_PADDLE].y + sizes[CPU_PADDLE].height / 2 < positions[BALL].y) {
        velocities[CPU_PADDLE].dy = cpuSpeed;
    } else if (positions[CPU_PADDLE].y + sizes[CPU_PADDLE].height / 2 > positions[BALL].y) {
        velocities[CPU_PADDLE].dy = -cpuSpeed;
    } else {
        velocities[CPU_PADDLE].dy = 0;
    }
}

void PhysicsEngine::updateBall() {
    positions[BALL].x += velocities[BALL].dx;
    positions[BALL].y += velocities[BALL].dy;
}

void PhysicsEngine::checkCollisions() {
    // Wall collisions (top/bottom)
    if (positions[BALL].y <= 0 || positions[BALL].y >= 470) {
        velocities[BALL].dy = -velocities[BALL].dy;
        // Play wall hit sound effect
        if (audioManager) {
            audioManager->playSound(SoundID::WallHit);
        }
    }

    // Paddle collisions
    bool hitPlayer = checkAABBCollision(BALL, PLAYER_PADDLE);
    bool hitCPU = checkAABBCollision(BALL, CPU_PADDLE);

    if (hitPlayer || hitCPU) {
        velocities[BALL].dx = -velocities[BALL].dx;
        int hitY = (hitPlayer ? positions[PLAYER_PADDLE].y : positions[CPU_PADDLE].y);
        velocities[BALL].dy = (positions[BALL].y + sizes[BALL].width / 2 - hitY - sizes[PLAYER_PADDLE].height / 2) / 10;
        if (velocities[BALL].dy > 5) velocities[BALL].dy = 5;
        if (velocities[BALL].dy < -5) velocities[BALL].dy = -5;
        
        // Play paddle hit sound effect
        if (audioManager) {
            audioManager->playSound(SoundID::PaddleHit);
        }
    }
}

bool PhysicsEngine::checkAABBCollision(EntityID a, EntityID b) {
    int leftA = positions[a].x;
    int rightA = positions[a].x + sizes[a].width;
    int topA = positions[a].y;
    int bottomA = positions[a].y + sizes[a].height;

    int leftB = positions[b].x;
    int rightB = positions[b].x + sizes[b].width;
    int topB = positions[b].y;
    int bottomB = positions[b].y + sizes[b].height;

    return (leftA < rightB && rightA > leftB && topA < bottomB && bottomA > topB);
}

void PhysicsEngine::checkScoring() {
    if (positions[BALL].x < 0) {  // CPU scores
        ++cpuScore;
        positions[BALL].x = 320;
        positions[BALL].y = 240;
        velocities[BALL].dx = 3;
        velocities[BALL].dy = (rand() % 5) - 2;
        
        // Play score sound effect
        if (audioManager) {
            audioManager->playSound(SoundID::Score);
        }
    } else if (positions[BALL].x > 640) {  // Player scores
        ++playerScore;
        positions[BALL].x = 320;
        positions[BALL].y = 240;
        velocities[BALL].dx = -3;
        velocities[BALL].dy = (rand() % 5) - 2;
        
        // Play score sound effect
        if (audioManager) {
            audioManager->playSound(SoundID::Score);
        }
    }
}