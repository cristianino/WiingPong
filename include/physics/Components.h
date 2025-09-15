#pragma once

// Lite ECS Components for WiingPong
// Components are plain structs; entities reference them via ID

struct Position {
    int x, y;
};

struct Velocity {
    int dx, dy;
};

struct Size {
    int width, height;  // For paddles; size for ball
};

struct Collider {
    bool isSolid;  // True for paddles/walls
    int bounceFactor;  // For angle variation
};

struct AIControlled {
    bool isAI;  // For CPU paddle
};