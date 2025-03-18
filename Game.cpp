#include "Game.h"
#include "Player.h"
#include <iostream>

using namespace std;

// Hàm khởi tạo của lớp Game
Game::Game() : window(nullptr), renderer(nullptr), isRunning(false), player(nullptr), gameOver(false), levelPassed(false) {}

// Hàm hủy của lớp Game
Game::~Game() {
    delete player; // Giải phóng bộ nhớ của player
    SDL_DestroyRenderer(renderer); // Hủy renderer
    SDL_DestroyWindow(window); // Hủy window
    SDL_Quit(); // Kết thúc SDL
}

// Hàm khởi tạo SDL và các thành phần của trò chơi
bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) { // Khởi tạo SDL
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    window = SDL_CreateWindow("Fall Gravity", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN); // Tạo cửa sổ
    if (!window) {
        cerr << "Không thể tạo cửa sổ Win! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // Tạo renderer
    if (!renderer) {
        cerr << "Không thể tạo Renderer! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    setupLevel1(); // Thiết lập Level 1 Easy
    isRunning = true;
    return true;
}

// Thiết lập Level 1 của trò chơi
void Game::setupLevel1() {
    const float activeZoneX = (SCREEN_WIDTH - ACTIVE_ZONE_WIDTH) / 2.0f;
    const float activeZoneY = (SCREEN_HEIGHT - ACTIVE_ZONE_HEIGHT) / 2.0f;
    const float floorY = activeZoneY + ACTIVE_ZONE_HEIGHT;

    player = new Player(activeZoneX + 30.0f, floorY - 32.0f, 12, 32); // Tạo nhân vật mới

    const int numHoles = 3;
    const float floorWidth = (ACTIVE_ZONE_WIDTH - numHoles * HOLE_WIDTH) / (numHoles + 1);
    floorSegments.clear();
    for (int i = 0; i <= numHoles; i++) {
        float x = activeZoneX + i * (floorWidth + HOLE_WIDTH);
        floorSegments.push_back({(int)x, (int)(floorY - FLOOR_THICKNESS), (int)floorWidth, FLOOR_THICKNESS}); // Vẽ sàn (Flat) vào code
    }

    gate = {(int)(activeZoneX + ACTIVE_ZONE_WIDTH - 50), (int)(floorY - 35), 30, 35}; // Cổng
}

// Cập nhật trạng thái của trò chơi
void Game::update() {
    const Uint8* keyState = SDL_GetKeyboardState(NULL);
    player->handleInput(event, keyState); // Xử lý đầu vào của người chơi
    player->update(floorSegments); // Cập nhật vị trí của nhân vật

    if (player->getRect().y > SCREEN_HEIGHT) { // Kiểm tra nếu nhân vật rơi xuống dưới màn hình
        gameOver = true;
        cout << "Press any key!" << endl; // Thua thì bắt đầu lại bằng cách nhấn nút bất kỳ
    }

    SDL_Rect playerRect = player->getRect();
    if (SDL_HasIntersection(&playerRect, &gate)) { // Kiểm tra nếu nhân vật chạm vào cổng
        levelPassed = true;
        cout << "Easy Pass!" << endl; // Thắng thì qua màn Level 1
    }
}

// Vẽ các thành phần của trò chơi lên màn hình
void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0xD2, 0xB4, 0x8C, 0xFF); // Nền nâu nhạt
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x99, 0xFF); // Vùng active zone màu vàng nhạt
    SDL_Rect activeZone = {(SCREEN_WIDTH - ACTIVE_ZONE_WIDTH) / 2, (SCREEN_HEIGHT - ACTIVE_ZONE_HEIGHT) / 2, ACTIVE_ZONE_WIDTH, ACTIVE_ZONE_HEIGHT};
    SDL_RenderFillRect(renderer, &activeZone); // Vẽ vùng hoạt động

    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF); // Đặt màu cho sàn
    for (const auto& segment : floorSegments) {
        SDL_RenderFillRect(renderer, &segment); // Vẽ các đoạn sàn
    }

    player->render(renderer); // Vẽ nhân vật

    SDL_SetRenderDrawColor(renderer, 0x80, 0x80, 0x80, 0xFF); // Đặt màu cho cổng
    SDL_RenderFillRect(renderer, &gate); // Vẽ cổng
    SDL_RenderPresent(renderer); // Cập nhật màn hình
}

// Vòng lặp chính của trò chơi
void Game::run() {
    while (isRunning) { // Khi trò chơi đang chạy
        while (SDL_PollEvent(&event) != 0) { // Xử lý các sự kiện
            if (event.type == SDL_QUIT) {
                isRunning = false;
            } else if (gameOver && event.type == SDL_KEYDOWN) {
                setupLevel1(); // Thiết lập lại cấp độ 1
                gameOver = false; // Đặt lại gameOver
            }
        }

        if (!gameOver && !levelPassed) {
            update(); // Cập nhật trạng thái trò chơi nếu chưa kết thúc và chưa qua màn
        }
        render(); // Vẽ các thành phần của trò chơi
    }
}
