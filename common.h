#ifndef COMMON_H
#define COMMON_H

#include "raylib.h"

// Screen configuration
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 700;

// Shared data structures
struct Laser {
    Vector2 pos;
    Vector2 speed;
    bool active;
};

#endif