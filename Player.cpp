#include "Player.h"
#include <SDL_image.h>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

Player::Player(float x, float y, int width, int height, SDL_Renderer* renderer)
    : x(x), y(y), width(width), height(height), xVel(0), yVel(0), isOnGround(false), canJump(true),
      playerTexture(nullptr), frameCount(4), currentFrameIndex(0), frameTime(3.0f), frameTimer(0.0f) { // Khởi tạo các biến animation
    SDL_Surface* playerSurface = IMG_Load("player_sprites.png"); // Load ảnh nhân vật
    if (!playerSurface) {
        cerr << "Không thể tải player_sprites.png! IMG_Error: " << IMG_GetError() << endl;
    } else {
        playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
        SDL_FreeSurface(playerSurface);
        if (!playerTexture) {
            cerr << "Không thể tạo texture cho player! SDL_Error: " << SDL_GetError() << endl;
        }
    }
    currentFrame = {0, 0, width, height}; // Khởi tạo frame đầu tiên (Frame 1: đứng yên)
}

Player::~Player() {
    SDL_DestroyTexture(playerTexture); // Giải phóng texture
}

void Player::handleInput(SDL_Event& e, const Uint8* keyState) {
    // Di chuyển ngang trái phải
    if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT]) {
        xVel = -SPEED;
    } else if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]) {
        xVel = SPEED;
    } else {
        xVel = 0;
    }

    // Chỉ nhảy khi phím W hoặc ↑ được nhấn, nhân vật trên sàn, và canJump là true
    if ((keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_SPACE]) && isOnGround && canJump) {
        yVel = -JUMP_SPEED;
        canJump = false; // Ngăn nhảy lại cho đến khi nhả phím
    }
    // Khi nhả phím W hoặc ↑, cho phép nhảy lần tiếp theo
    else if (!keyState[SDL_SCANCODE_W] && !keyState[SDL_SCANCODE_UP] && !keyState[SDL_SCANCODE_SPACE]) {
        canJump = true; // Reset canJump khi phím được nhả
    }
}

void Player::update(const vector<SDL_Rect>& floorSegments) {
    // Cập nhật vị trí
    x += xVel;
    y += yVel;

    // Áp dụng trọng lực
    yVel += GRAVITY;

    // Kiểm tra va chạm với sàn
    isOnGround = false;
    SDL_Rect playerRect = {(int)x, (int)y, width, height};
    for (const auto& segment : floorSegments) {
        if (SDL_HasIntersection(&playerRect, &segment)) {
            isOnGround = true;
            if (yVel > 0) {
                y = segment.y - height;
                yVel = 0;
            }
            break;
        }
    }

    // Giới hạn trong active zone
    const float ACTIVE_ZONE_X = (1089 - 856) / 2.0f;
    const float ACTIVE_ZONE_TOP = (612 - 121) / 2.0f;
    if (x < ACTIVE_ZONE_X) x = ACTIVE_ZONE_X;
    if (x + width > ACTIVE_ZONE_X + 856) x = ACTIVE_ZONE_X + 856 - width;
    if (y < ACTIVE_ZONE_TOP) {
        y = ACTIVE_ZONE_TOP;
        if (yVel < 0) yVel = 0;
    }

    // Logic animation
    if (xVel != 0) { // Chỉ kiểm tra di chuyển ngang
        frameTimer += 0.016f; // Giả sử 60 FPS
        if (frameTimer >= frameTime) { // 0.2s mỗi frame (5 FPS)
            frameTimer = 0.0f;
            currentFrameIndex = (currentFrameIndex % 3) + 1; // Frame 2 -> 3 -> 4 -> 2 -> 3 -> ... loop
            currentFrame.x = currentFrameIndex * width;
        }
    } else { // Đứng yên
        currentFrameIndex = 0;
        currentFrame.x = 0;
        frameTimer = 0.0f;
    }
}

void Player::render(SDL_Renderer* renderer) {
    if (playerTexture) {
        SDL_Rect playerRect = {(int)x, (int)y, width, height};
        SDL_RenderCopy(renderer, playerTexture, &currentFrame, &playerRect); // Vẽ frame hiện tại
    } else {
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
        SDL_Rect playerRect = {(int)x, (int)y, width, height};
        SDL_RenderFillRect(renderer, &playerRect);
    }
}

SDL_Rect Player::getRect() const {
    return {(int)x, (int)y, width, height};
}
