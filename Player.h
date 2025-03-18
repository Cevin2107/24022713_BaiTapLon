#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <vector>

using namespace std;
// Khởi tạo nhân vật
class Player {
public:
    Player(float x, float y, int width, int height);
    void handleInput(SDL_Event& e, const Uint8* keyState);
    void update(const vector<SDL_Rect>& floorSegments);
    void render(SDL_Renderer* renderer);
    SDL_Rect getRect() const;

private:
    float x, y;
    int width, height;
    float xVel, yVel;
    bool isOnGround;
    bool canJump;
    const float SPEED = 0.12f; // Tốc độ chạy
    const float JUMP_SPEED = 0.7f; // Độ nhảy
    const float GRAVITY = 0.003f; // Trọng lực
};

#endif
