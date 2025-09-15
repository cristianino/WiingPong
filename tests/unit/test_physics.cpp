#include <gtest/gtest.h>
#include "physics/PhysicsEngine.h"
#include "physics/Components.h"

// Test ball collision with paddle
TEST(PhysicsTest, BallPaddleCollision) {
    PhysicsEngine engine;
    engine.init();

    // Setup for hit
    engine.positions[BALL].x = 25;  // Touch player paddle at x=20
    engine.positions[BALL].y = 210;
    engine.velocities[BALL].dx = 3;  // Moving right

    engine.update();

    // Expect dx reversed
    EXPECT_LT(engine.velocities[BALL].dx, 0);
}

// Test wall bounce
TEST(PhysicsTest, BallWallBounce) {
    PhysicsEngine engine;
    engine.init();

    engine.positions[BALL].y = 0;
    engine.velocities[BALL].dy = 2;

    engine.update();

    EXPECT_EQ(engine.velocities[BALL].dy, -2);
}

// Test scoring reset
TEST(PhysicsTest, Scoring) {
    PhysicsEngine engine;
    engine.init();

    engine.positions[BALL].x = -1;  // Left out
    engine.update();

    EXPECT_EQ(engine.cpuScore, 1);
    EXPECT_EQ(engine.positions[BALL].x, 320);
}