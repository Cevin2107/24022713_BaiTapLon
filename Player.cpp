#include "Player.h"
#include <SDL_image.h>
#include <iostream>

using namespace std;

// Khởi tạo nhân vật
Player::Player(float x, float y, int width, int height, SDL_Renderer* renderer)
    : x(x), y(y), width(width), height(height),
      xVel(0.0f), yVel(0.0f),
      isOnGround(false), canJump(true),
      playerTexture(nullptr), frameCount(4), currentFrameIndex(0),
      frameTime(3.0f), frameTimer(0.0f) { // 0.2s mỗi frame cho 5 FPS
    // Khởi tạo texture nhân vật
    SDL_Surface* playerSurface = IMG_Load("player_sprites.png");
    if (!playerSurface) {
        cerr << "Không thể tải player_sprites.png! IMG_Error: " << IMG_GetError() << endl;
        return;
    }

    playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
    SDL_FreeSurface(playerSurface);
    if (!playerTexture) {
        cerr << "Không thể tạo texture cho nhân vật! SDL_Error: " << SDL_GetError() << endl;
        return;
    }

    // Khởi tạo frame đầu tiên (đứng yên)
    currentFrame = {0, 0, width, height};
}

// Hủy nhân vật, giải phóng tài nguyên
Player::~Player() {
    SDL_DestroyTexture(playerTexture);
}

// Xử lý đầu vào từ người chơi
void Player::handleInput(SDL_Event& event, const Uint8* keyState) {
    // Di chuyển ngang
    if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT]) {
        xVel = -SPEED;
    } else if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]) {
        xVel = SPEED;
    } else {
        xVel = 0.0f;
    }

    // Nhảy khi nhấn W, ↑ hoặc Space, đang trên sàn và được phép nhảy
    if ((keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_SPACE]) && isOnGround && canJump) {
        yVel = -JUMP_SPEED;
        canJump = false; // Ngăn nhảy lại cho đến khi nhả phím
    }
    // Cho phép nhảy lại khi nhả phím
    if (!(keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_SPACE])) {
        canJump = true;
    }
}

// Cập nhật trạng thái nhân vật
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
            if (yVel > 0) { // Nhân vật đang rơi
                y = segment.y - height; // Đặt nhân vật trên sàn
                yVel = 0.0f;
            }
            break;
        }
    }

    // Giới hạn nhân vật trong vùng hoạt động
    const float ACTIVE_ZONE_X = (1089.0f - 856.0f) / 2.0f;
    const float ACTIVE_ZONE_TOP = (612.0f - 121.0f) / 2.0f;
    if (x < ACTIVE_ZONE_X) {
        x = ACTIVE_ZONE_X;
    }
    if (x + width > ACTIVE_ZONE_X + 856.0f) {
        x = ACTIVE_ZONE_X + 856.0f - width;
    }
    if (y < ACTIVE_ZONE_TOP) {
        y = ACTIVE_ZONE_TOP;
        if (yVel < 0) { // Nhân vật đang nhảy lên
            yVel = 0.0f;
        }
    }

    // Cập nhật animation
    updateAnimation();
}

// Cập nhật animation của nhân vật
void Player::updateAnimation() {
    if (xVel != 0.0f) { // Đang di chuyển ngang
        frameTimer += 0.016f; // Giả sử 60 FPS (1/60s mỗi frame)
        if (frameTimer >= frameTime) {
            frameTimer = 0.0f;
            currentFrameIndex = (currentFrameIndex % 3) + 1; // Chuyển giữa frame 2, 3, 4
            currentFrame.x = currentFrameIndex * width;
        }
    } else { // Đứng yên
        currentFrameIndex = 0;
        currentFrame.x = 0;
        frameTimer = 0.0f;
    }
}

// Vẽ nhân vật lên màn hình
void Player::render(SDL_Renderer* renderer) {
    SDL_Rect playerRect = {(int)x, (int)y, width, height};
    if (playerTexture) {
        // Vẽ frame hiện tại của nhân vật
        SDL_RenderCopy(renderer, playerTexture, &currentFrame, &playerRect);
    } else {
        // Vẽ hình chữ nhật xanh nếu không có texture
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
        SDL_RenderFillRect(renderer, &playerRect);
    }
}

SDL_Rect Player::getRect() const {
    return {(int)x, (int)y, width, height};
}
