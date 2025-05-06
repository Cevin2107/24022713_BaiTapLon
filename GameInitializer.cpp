#include "Game.h"
#include "Player.h"
#include <iostream>

using namespace std;

// Hàm khởi tạo của lớp Game
Game::Game() : window(nullptr), renderer(nullptr), isRunning(false), player(nullptr), gameOver(false), levelPassed(false), playerVisible(true), isInMenu(true), instructionTexture(nullptr), startButtonTexture(nullptr), quitButtonTexture(nullptr) {}

// Hàm hủy của lớp Game
Game::~Game() {
    delete player; // Giải phóng bộ nhớ của player
    SDL_DestroyTexture(logoTexture); // Giải phóng texture logo
    SDL_DestroyTexture(instructionTexture); // Giải phóng texture instruction
    SDL_DestroyTexture(gateTexture); // Giải phóng cổng
    SDL_DestroyRenderer(renderer); // Hủy renderer
    SDL_DestroyWindow(window); // Hủy window
    SDL_DestroyTexture(startButtonTexture); // Hủy texture Start
    SDL_DestroyTexture(quitButtonTexture);  // Hủy texture Quit
    SDL_DestroyTexture(backgroundTexture); // Hủy Background
    SDL_Quit(); // Kết thúc SDL
}

// Hàm khởi tạo SDL và các thành phần của trò chơi
bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) { // Khởi tạo SDL
        cerr << "SDL không thể khởi tạo! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) { // Khởi tạo SDL_image với PNG
        cerr << "SDL_image không thể khởi tạo! IMG_Error: " << IMG_GetError() << endl;
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

    // Tải background
    SDL_Surface* backgroundSurface = IMG_Load("Background.png");
    if (!backgroundSurface) {
        cerr << "Không thể tải Background.png! IMG_Error: " << IMG_GetError() << endl;
        return false;
    }
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);
    if (!backgroundTexture) {
        cerr << "Không thể tạo texture cho Background! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }
    backgroundRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    // Tải logo
    SDL_Surface* logoSurface = IMG_Load("logo.png"); // chèn logo với tên là logo.png
    if (!logoSurface) {
        cerr << "Không thể tải logo! IMG_Error: " << IMG_GetError() << endl;
        return false;
    }
    logoTexture = SDL_CreateTextureFromSurface(renderer, logoSurface);
    SDL_FreeSurface(logoSurface);
    if (!logoTexture) {
        cerr << "Không thể tạo texture cho logo! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    // Đặt vị trí logo
    int logoWidth, logoHeight;
    SDL_QueryTexture(logoTexture, NULL, NULL, &logoWidth, &logoHeight); // Lấy kích thước logo
    logoRect = {(SCREEN_WIDTH - logoWidth) / 2, 30, logoWidth, logoHeight}; // Ở giữa, và dịch lên trên

    // Tải instruction
    SDL_Surface* instructionSurface = IMG_Load("instruction.png");
    if (!instructionSurface) {
        cerr << "Không thể tải instruction.png! IMG_Error: " << IMG_GetError() << endl;
        return false;
    }
    instructionTexture = SDL_CreateTextureFromSurface(renderer, instructionSurface);
    SDL_FreeSurface(instructionSurface);
    if (!instructionTexture) {
        cerr << "Không thể tạo texture cho instruction! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    // Đặt vị trí instruction
    int instructionWidth, instructionHeight;
    SDL_QueryTexture(instructionTexture, NULL, NULL, &instructionWidth, &instructionHeight);
    instructionRect = {(SCREEN_WIDTH - instructionWidth) / 2, 320, instructionWidth, instructionHeight};

    // Khởi tạo nút Start và Quit
    const int startY = ((SCREEN_HEIGHT - (buttonHeight * 2 + buttonSpacing)) / 2 ) + 50; // Căn giữa
    startButton = {(SCREEN_WIDTH - buttonWidth) / 2, startY, buttonWidth, buttonHeight}; // Nút Start ở giữa
    quitButton = {(SCREEN_WIDTH - buttonWidth) / 2, startY + buttonHeight + buttonSpacing, buttonWidth, buttonHeight}; // Nút Quit ngay dưới

    // Tải Start
    SDL_Surface* startSurface = IMG_Load("Start.png");
    if (!startSurface) {
        cerr << "Không thể tải Start.png! IMG_Error: " << IMG_GetError() << endl;
        return false;
    }
    startButtonTexture = SDL_CreateTextureFromSurface(renderer, startSurface);
    SDL_FreeSurface(startSurface);
    if (!startButtonTexture) {
        cerr << "Không thể tạo texture cho Start.png! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    // Đặt vị trí Start
    startButtonRect = {
        startButton.x,
        startButton.y,
        buttonWidth,
        buttonHeight
    };

    // Tải Quit
    SDL_Surface* quitSurface = IMG_Load("Quit.png");
    if (!quitSurface) {
        cerr << "Không thể tải Quit.png! IMG_Error: " << IMG_GetError() << endl;
        return false;
    }
    quitButtonTexture = SDL_CreateTextureFromSurface(renderer, quitSurface);
    SDL_FreeSurface(quitSurface);
    if (!quitButtonTexture) {
        cerr << "Không thể tạo texture cho Quit.png! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    // Đặt vị trí Quit
    quitButtonRect = {
        quitButton.x,
        quitButton.y,
        buttonWidth,
        buttonHeight
    };

    // Tải texture cho cổng
    SDL_Surface* gateSurface = IMG_Load("gate.png");
    if (!gateSurface) {
        cerr << "Không thể tải gate.png! IMG_Error: " << IMG_GetError() << endl;
        return false;
    }
    gateTexture = SDL_CreateTextureFromSurface(renderer, gateSurface);
    SDL_FreeSurface(gateSurface);
    if (!gateTexture) {
        cerr << "Không thể tạo texture cho cổng! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    setupLevel1(); // Thiết lập Level 1 Easy
    isRunning = true;
    return true;
}
