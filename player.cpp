#include "player.h"

void Player::Init() {
    Image tempImg = LoadImage("images/insect.png");
    
    if (tempImg.data != nullptr) {
        ImageFormat(&tempImg, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
        texture = LoadTextureFromImage(tempImg);
        UnloadImage(tempImg);
    } else {
        texture = LoadTexture("images/insect.png");
    }
    
    // FORCE FIXED ARCADE SIZE FOR PLAYER (Instead of using raw image size)
    width = 70;  
    height = 70;

    pos.x = (SCREEN_WIDTH / 2.0f) - (width / 2.0f);
    pos.y = SCREEN_HEIGHT - height - 20.0f;
    speed.x = 5.0f; 
    speed.y = 0.0f;
}

void Player::Update(std::vector<Laser>& playerLasers) {
    // Movement logic
    if (IsKeyDown(KEY_LEFT) && pos.x > 0) {
        pos.x -= speed.x;
    }
    if (IsKeyDown(KEY_RIGHT) && pos.x < (SCREEN_WIDTH - width)) {
        pos.x += speed.x;
    }

    // Shooting logic
    if (IsKeyPressed(KEY_SPACE)) {
        Laser newLaser;
        newLaser.pos.x = pos.x + (width / 2.0f) - 2.0f; // Center laser horizontally
        newLaser.pos.y = pos.y;
        newLaser.speed.x = 0.0f;
        newLaser.speed.y = -7.0f; // Moves upward
        newLaser.active = true;
        
        playerLasers.push_back(newLaser);
    }
}

void Player::Draw() {
    // Defines what part of the texture to read (the whole thing)
    Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
    
    // Defines where to draw it on screen and forces the target scale size
    Rectangle dest = { pos.x, pos.y, (float)width, (float)height };
    
    // Centers the rotation origin perfectly
    Vector2 origin = { 0.0f, 0.0f };
    
    DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
}

void Player::Unload() {
    // Unload texture from GPU memory
    UnloadTexture(texture);
}