#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"
#include <vector>

class Player {
public:
    Vector2 pos;
    Vector2 speed;
    int width;
    int height;
    Texture2D texture;

    void Init();
    void Update(std::vector<Laser>& playerLasers);
    void Draw();
    void Unload();
};

#endif