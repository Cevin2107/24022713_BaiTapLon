#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <vector>

using namespace std;
// Khởi tạo nhân vật
class Player {
public:
    Player(float x, float y, int width, int height, SDL_Renderer* renderer); // Chỗ này thêm renderer để tải texture hamiboom
    ~Player(); // Destructor để giải phóng texture
    void handleInput(SDL_Event& e, const Uint8* keyState);
    void update(const vector<SDL_Rect>& floorSegments);
    void render(SDL_Renderer* renderer);
    SDL_Rect getRect() const;

private:
    float x, y;
    int width, height;
    float xVel, yVel;
    bool isOnGround;
    bool canJump; // Nhảy 1 lần khi nhấn hay giữ W

    // Texture nhân vật
    SDL_Texture* playerTexture; // Texture cho nhân vật
    SDL_Rect currentFrame; // Frame
    int frameCount; // Đếm frame
    int currentFrameIndex; // Chỉ số frame hiện tại
    float frameTime; // Thời gian mỗi frame (setup 5FPS tương ứng 0.2 1 frame)
    float frameTimer; // Bộ đếm thời gian để chuyển frame
    void updateAnimation();

    const float SPEED = 0.075f; // Tốc độ chạy
    const float JUMP_SPEED = 0.4f; // Độ nhảy
    const float GRAVITY = 0.001f; // Trọng lực
};

#endif
