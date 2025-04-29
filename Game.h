#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
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
    SDL_Texture* gateTexture; // Texture cho cổng
    bool gameOver;
    bool levelPassed;
    SDL_Texture* logoTexture; // Texture cho logo
    SDL_Rect logoRect;        // Vị trí và kích thước logo
    SDL_Texture* instructionTexture; // Texture cho instruction
    SDL_Rect instructionRect; // Vị trí và kích thước instruction
    bool playerVisible;
    bool isInMenu; // Thêm trạng thái menu
    SDL_Rect startButton; // Nút Start
    SDL_Rect quitButton; // Nút Quit
    SDL_Texture* startButtonTexture; // Texture cho Start.png
    SDL_Texture* quitButtonTexture;  // Texture cho Quit.png
    SDL_Rect startButtonRect;        // Vùng hiển thị Start.png
    SDL_Rect quitButtonRect;         // Vùng hiển thị Quit.png

    // Size màn hình
    const int SCREEN_WIDTH = 1089; // Chiều rộng màn hình
    const int SCREEN_HEIGHT = 612; // Chiều cao màn hình
    const int ACTIVE_ZONE_WIDTH = 856; // Chiều rộng vùng hoạt động
    const int ACTIVE_ZONE_HEIGHT = 121; // Chiều cao vùng hoạt động
    const int FLOOR_THICKNESS = 5; // Độ dày của sàn
    const int HOLE_WIDTH = 50; // Chiều rộng của lỗ

    // Size nút
    const int buttonWidth = 350; // Chiều rộng nút
    const int buttonHeight = 70; // Chiều cao nút
    const int buttonSpacing = 30; // Khoảng cách giữa hai nút
};

#endif
