#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <vector>

using namespace std;

class Player;

class Game {
public:
    Game();
    ~Game();
    bool init(); // Khởi tạo trò chơi
    void run(); // Vòng lặp chính của trò chơi

private:
    void setupLevel1();
    void update();
    void render();

    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning;
    SDL_Event event;
    Player* player;
    vector<SDL_Rect> floorSegments; // Danh sách các đoạn sàn
    SDL_Rect gate; // Cổng
    bool gameOver;
    bool levelPassed;

    // Các size px
    const int SCREEN_WIDTH = 1089; // Chiều rộng màn hình
    const int SCREEN_HEIGHT = 612; // Chiều cao màn hình
    const int ACTIVE_ZONE_WIDTH = 856; // Chiều rộng vùng hoạt động
    const int ACTIVE_ZONE_HEIGHT = 121; // Chiều cao vùng hoạt động
    const int FLOOR_THICKNESS = 5; // Độ dày của sàn
    const int HOLE_WIDTH = 40; // Chiều rộng của lỗ
};

#endif
