#include "enemy.h"
#include <cstdlib>

void EnemyManager::Init() {
    texture = LoadTexture("images/logo.png");
    spawnTimer = 0.0f;
    spawnInterval = 1.5f;
}

void EnemyManager::Update(std::vector<Laser>& enemyLasers) {
    // Update spawning timer
    spawnTimer += GetFrameTime();
    
  if (spawnTimer >= spawnInterval) {
        Enemy newEnemy;
        
        // FORCE FIXED ARCADE SIZE FOR ENEMIES 
        newEnemy.width = 50;
        newEnemy.height = 50;
        
        newEnemy.pos.x = (float)(rand() % (SCREEN_WIDTH - newEnemy.width));
        newEnemy.pos.y = -newEnemy.height;
        newEnemy.alive = true;
        
        enemies.push_back(newEnemy);
        spawnTimer = 0.0f;
    }

    // Move enemies downward and handle shooting
    for (size_t i = 0; i < enemies.size(); i++) {
        if (enemies[i].alive) {
            enemies[i].pos.y += 2.0f; // Downward movement speed

            // Random chance for each enemy to fire a laser downward
            if (rand() % 200 == 0) {
                Laser enemyLaser;
                enemyLaser.pos.x = enemies[i].pos.x + (enemies[i].width / 2.0f) - 2.0f;
                enemyLaser.pos.y = enemies[i].pos.y + enemies[i].height;
                enemyLaser.speed.x = 0.0f;
                enemyLaser.speed.y = 5.0f; // Moves downward
                enemyLaser.active = true;
                
                enemyLasers.push_back(enemyLaser);
            }
        }
    }

    // Clean up dead or off-screen enemies to save memory
    for (auto it = enemies.begin(); it != enemies.end();) {
        if (!it->alive || it->pos.y > SCREEN_HEIGHT) {
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }
}

void EnemyManager::Draw() {
    for (size_t i = 0; i < enemies.size(); i++) {
        if (enemies[i].alive) {
            Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
            Rectangle dest = { enemies[i].pos.x, enemies[i].pos.y, (float)enemies[i].width, (float)enemies[i].height };
            Vector2 origin = { 0.0f, 0.0f };
            
            DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
        }
    }
}

void EnemyManager::Unload() {
    // Unload texture from GPU memory
    UnloadTexture(texture);
}