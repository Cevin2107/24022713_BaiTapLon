#include "Player.h"

using namespace std;

Player::Player(float x, float y, int width, int height)
    : x(x), y(y), width(width), height(height), xVel(0), yVel(0), isOnGround(false), canJump(true) {}

void Player::handleInput(SDL_Event& e, const Uint8* keyState) {
    // Di chuyển ngang
    if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT]) {
        xVel = -SPEED;
    } else if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]) {
        xVel = SPEED;
    } else {
        xVel = 0;
    }

    // Nhảy: Chỉ nhảy khi phím W hoặc ↑ được nhấn, nhân vật trên sàn, và canJump là true
    if ((keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP]) && isOnGround && canJump) {
        yVel = -JUMP_SPEED;
        canJump = false; // Ngăn nhảy lại cho đến khi nhả phím
    }
    // Khi nhả phím W hoặc ↑, cho phép nhảy lần tiếp theo
    else if (!keyState[SDL_SCANCODE_W] && !keyState[SDL_SCANCODE_UP]) {
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
            if (yVel > 0) { // Chỉ điều chỉnh khi rơi xuống
                y = segment.y - height;
                yVel = 0; // Dừng rơi khi chạm sàn
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
        if (yVel < 0) yVel = 0; // Dừng khi chạm biên trên
    }
}

void Player::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
    SDL_Rect playerRect = {(int)x, (int)y, width, height};
    SDL_RenderFillRect(renderer, &playerRect);
}

SDL_Rect Player::getRect() const {
    return {(int)x, (int)y, width, height};
}
