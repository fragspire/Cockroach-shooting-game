#include "raylib.h"
#include "common.h"
#include "player.h"
#include "enemy.h"
#include <vector>
#include <cstdlib>
#include <ctime>

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Insect vs Logos - Arcade Mode");
    SetTargetFPS(60);
    srand((unsigned int)time(0));

    // Create game objects
    Player player;
    EnemyManager enemyManager;

    player.Init();
    enemyManager.Init();

    // Load and uncompress the static background texture
    Image bgImg = LoadImage("images/background.png");
    Texture2D background;
    if (bgImg.data != nullptr) {
        ImageFormat(&bgImg, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
        background = LoadTextureFromImage(bgImg);
        UnloadImage(bgImg);
    } else {
        background = LoadTexture("images/background.png");
    }

    std::vector<Laser> playerLasers;
    std::vector<Laser> enemyLasers;

    while (!WindowShouldClose()) {
        
        // --- 1. UPDATE STATE ---
        player.Update(playerLasers);
        enemyManager.Update(enemyLasers);

        // Update & animate player lasers
        for (auto& laser : playerLasers) {
            if (laser.active) {
                laser.pos.y += laser.speed.y;

                for (auto& enemy : enemyManager.enemies) {
                    if (enemy.alive && CheckCollisionRecs({enemy.pos.x, enemy.pos.y, (float)enemy.width, (float)enemy.height}, {laser.pos.x, laser.pos.y, 4.0f, 15.0f})) {
                        enemy.alive = false;
                        laser.active = false;
                        break;
                    }
                }
            }
        }

        // Update & animate enemy lasers
        for (auto& laser : enemyLasers) {
            if (laser.active) {
                laser.pos.y += laser.speed.y;

                if (CheckCollisionRecs({player.pos.x, player.pos.y, (float)player.width, (float)player.height}, {laser.pos.x, laser.pos.y, 4.0f, 12.0f})) {
                    laser.active = false;
                }
            }
        }

        // Clean up inactive lasers
        for (auto it = playerLasers.begin(); it != playerLasers.end();) {
            if (!it->active || it->pos.y < -20) {
                it = playerLasers.erase(it);
            } else {
                ++it;
            }
        }
        for (auto it = enemyLasers.begin(); it != enemyLasers.end();) {
            if (!it->active || it->pos.y > SCREEN_HEIGHT + 20) {
                it = enemyLasers.erase(it);
            } else {
                ++it;
            }
        }

        // --- 2. RENDER STEP ---
        BeginDrawing();
        ClearBackground(BLACK); // Fallback behind image

        // Draw background texture statically to fill the entire window frame smoothly
        Rectangle bgSource = { 0.0f, 0.0f, (float)background.width, (float)background.height };
        Rectangle bgDest = { 0.0f, 0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT };
        Vector2 bgOrigin = { 0.0f, 0.0f };
        
        DrawTexturePro(background, bgSource, bgDest, bgOrigin, 0.0f, WHITE);

        // Draw game entities over the background image
        player.Draw();
        enemyManager.Draw();

        // Draw Player Lasers
        for (const auto& laser : playerLasers) {
            if (laser.active) {
                DrawRectangle((int)laser.pos.x, (int)laser.pos.y, 4, 15, CLITERAL(Color){ 0, 255, 255, 255 });
            }
        }

        // Draw Enemy Lasers
        for (const auto& laser : enemyLasers) {
            if (laser.active) {
                DrawRectangle((int)laser.pos.x, (int)laser.pos.y, 4, 12, CLITERAL(Color){ 255, 0, 0, 255 });
            }
        }

        // UI text
        DrawText("Use LEFT/RIGHT arrows. SPACE to Shoot.", 10, 10, 20, LIGHTGRAY);
        
        EndDrawing();
    }

    // --- 3. CLEANUP STEP ---
    player.Unload();
    enemyManager.Unload();
    UnloadTexture(background); // Free memory completely on close
    CloseWindow();

    return 0;
}