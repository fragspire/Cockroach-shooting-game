#include "raylib.h"
#include "common.h"
#include "player.h"
#include "enemy.h"
#include <vector>
#include <cstdlib>
#include <ctime>

// Define our two distinct game screens
enum GameState {
    STATE_GAMEPLAY,
    STATE_GAMEOVER
};

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Insect vs Logos - Arcade Mode");
    SetTargetFPS(60);
    srand((unsigned int)time(0));

    // Create game objects
    Player player;
    EnemyManager enemyManager;

    player.Init();
    enemyManager.Init();

    // Load static background texture
    Image bgImg = LoadImage("images/background.png");
    Texture2D background;
    if (bgImg.data != nullptr) {
        ImageFormat(&bgImg, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
        background = LoadTextureFromImage(bgImg);
        UnloadImage(bgImg);
    } else {
        background = LoadTexture("images/background.png");
    }

    // --- SYSTEM STATE SETUP ---
    GameState currentState = STATE_GAMEPLAY;
    int lifelines = 5;

    std::vector<Laser> playerLasers;
    std::vector<Laser> enemyLasers;

    // Defines the layout size and position of our Retry button
    Rectangle retryButton = { (float)SCREEN_WIDTH / 2 - 80, (float)SCREEN_HEIGHT / 2 + 20, 160, 45 };

    while (!WindowShouldClose()) {
        
        // ==========================================
        // 1. UPDATE LOGIC STEP
        // ==========================================
        if (currentState == STATE_GAMEPLAY) {
            
            player.Update(playerLasers);
            enemyManager.Update(enemyLasers);

            // CONDITION 1: Check if enemy crosses below the player screen bounds
            for (auto& enemy : enemyManager.enemies) {
                if (enemy.alive && enemy.pos.y > player.pos.y) {
                    enemy.alive = false; // Disable enemy
                    lifelines--;         // Lose a life
                }
            }

            // Update player lasers & check if they hit enemies
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

            // CONDITION 2: Update enemy lasers & check if an enemy bullet hits the insect player
            for (auto& laser : enemyLasers) {
                if (laser.active) {
                    laser.pos.y += laser.speed.y;

                    if (CheckCollisionRecs({player.pos.x, player.pos.y, (float)player.width, (float)player.height}, {laser.pos.x, laser.pos.y, 4.0f, 12.0f})) {
                        laser.active = false;
                        lifelines--; // Lose a life
                    }
                }
            }

            // Route execution to Game Over screen if health depletes
            if (lifelines <= 0) {
                lifelines = 0;
                currentState = STATE_GAMEOVER;
            }

            // Clean up off-screen lasers
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

        } else if (currentState == STATE_GAMEOVER) {
            // Check if user is clicking or hovering over the retry button bounds
            Vector2 mousePos = GetMousePosition();
            if (CheckCollisionPointRec(mousePos, retryButton)) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    // RESET LOOP FOR A NEW RUN
                    lifelines = 5;
                    playerLasers.clear();
                    enemyLasers.clear();
                    enemyManager.enemies.clear(); // Clear out spawned enemy wave completely
                    player.Init();                // Respawn player at base center
                    enemyManager.Init();          // Reset enemy timers
                    currentState = STATE_GAMEPLAY;
                }
            }
        }

        // ==========================================
        // 2. RENDERING DRAW STEP
        // ==========================================
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw static backdrop image
        Rectangle bgSource = { 0.0f, 0.0f, (float)background.width, (float)background.height };
        Rectangle bgDest = { 0.0f, 0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT };
        Vector2 bgOrigin = { 0.0f, 0.0f };
        DrawTexturePro(background, bgSource, bgDest, bgOrigin, 0.0f, WHITE);

        if (currentState == STATE_GAMEPLAY) {
            // Draw regular game entities
            player.Draw();
            enemyManager.Draw();

            for (const auto& laser : playerLasers) {
                if (laser.active) {
                    DrawRectangle((int)laser.pos.x, (int)laser.pos.y, 4, 15, CLITERAL(Color){ 0, 255, 255, 255 });
                }
            }

            for (const auto& laser : enemyLasers) {
                if (laser.active) {
                    DrawRectangle((int)laser.pos.x, (int)laser.pos.y, 4, 12, CLITERAL(Color){ 255, 0, 0, 255 });
                }
            }

            // HUD Overlays
            DrawText(TextFormat("LIVES: %d", lifelines), 15, 15, 25, RED);

        } else if (currentState == STATE_GAMEOVER) {
            // Tint and darken the active background for menu overlay readability
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, CLITERAL(Color){ 0, 0, 0, 180 });

            // Display "GAME OVER"
            DrawText("GAME OVER", SCREEN_WIDTH / 2 - MeasureText("GAME OVER", 40) / 2, SCREEN_HEIGHT / 2 - 50, 40, RED);

            // Change button color to darker Maroon on mouse hover
            Color btnColor = CheckCollisionPointRec(GetMousePosition(), retryButton) ? MAROON : RED;
            
            // Draw Interactive Retry Box
            DrawRectangleRec(retryButton, btnColor);
            DrawRectangleLinesEx(retryButton, 2, WHITE);
            DrawText("RETRY", (int)retryButton.x + ((int)retryButton.width / 2) - (MeasureText("RETRY", 20) / 2), (int)retryButton.y + 12, 20, WHITE);
        }
        
        EndDrawing();
    }

    // --- 3. CLEANUP STEP ---
    player.Unload();
    enemyManager.Unload();
    UnloadTexture(background);
    CloseWindow();

    return 0;
}