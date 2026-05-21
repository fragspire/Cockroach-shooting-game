#ifndef ENEMY_H
#define ENEMY_H

#include "common.h"
#include <vector>

class Enemy {
public:
    Vector2 pos;
    int width;
    int height;
    bool alive;
};

class EnemyManager {
public:
    std::vector<Enemy> enemies;
    Texture2D texture;
    float spawnTimer;
    float spawnInterval;

    void Init();
    void Update(std::vector<Laser>& enemyLasers);
    void Draw();
    void Unload();
};

#endif