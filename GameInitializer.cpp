#include "Game.h"
#include "Player.h"
#include <iostream>

using namespace std;

static SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* file, SDL_Rect& rect, int x, int y, bool centerX = true) {
    SDL_Surface* surface = IMG_Load(file);
    if (!surface) {
        cerr << "Không thể tải " << file << "! IMG_Error: " << IMG_GetError() << endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        cerr << "Không thể tạo texture cho " << file << "! SDL_Error: " << SDL_GetError() << endl;
        return nullptr;
    }

    int width, height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    rect = {centerX ? (x - width / 2) : x, y, width, height};
    return texture;
}

// Hàm khởi tạo của lớp Game
Game::Game()
    : window(nullptr), renderer(nullptr), isRunning(false), player(nullptr),
      gameOver(false), levelPassed(false), playerVisible(true), isInMenu(true),
      logoTexture(nullptr), instructionTexture(nullptr),
      startButtonTexture(nullptr), quitButtonTexture(nullptr),
      gateTexture(nullptr), backgroundTexture(nullptr) {}

// Hàm hủy của lớp Game
Game::~Game() {
    delete player;
    SDL_DestroyTexture(logoTexture);
    SDL_DestroyTexture(instructionTexture);
    SDL_DestroyTexture(gateTexture);
    SDL_DestroyTexture(startButtonTexture);
    SDL_DestroyTexture(quitButtonTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Hàm khởi tạo SDL và các thành phần của trò chơi
bool Game::init() {
    // Khởi tạo SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL không thể khởi tạo! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    // Khởi tạo SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        cerr << "SDL_image không thể khởi tạo! IMG_Error: " << IMG_GetError() << endl;
        return false;
    }

    // Tạo cửa sổ
    window = SDL_CreateWindow("Fall Gravity", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        cerr << "Không thể tạo cửa sổ! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    // Tạo renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << "Không thể tạo renderer! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    // Tải background
    backgroundTexture = loadTexture(renderer, "Background.png", backgroundRect, 0, 0, false);
    if (!backgroundTexture) return false;
    backgroundRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    // Tải logo
    logoTexture = loadTexture(renderer, "logo.png", logoRect, SCREEN_WIDTH / 2, 30);
    if (!logoTexture) return false;

    // Tải instruction
    instructionTexture = loadTexture(renderer, "instruction.png", instructionRect, SCREEN_WIDTH / 2, 320);
    if (!instructionTexture) return false;

    // Tải nút Start
    const int startY = (SCREEN_HEIGHT - (buttonHeight * 2 + buttonSpacing)) / 2 + 50;
    startButtonTexture = loadTexture(renderer, "Start.png", startButtonRect, SCREEN_WIDTH / 2, startY);
    if (!startButtonTexture) return false;
    startButton = {startButtonRect.x, startButtonRect.y, buttonWidth, buttonHeight}; // Vùng click

    // Tải nút Quit
    quitButtonTexture = loadTexture(renderer, "Quit.png", quitButtonRect, SCREEN_WIDTH / 2, startY + buttonHeight + buttonSpacing);
    if (!quitButtonTexture) return false;
    quitButton = {quitButtonRect.x, quitButtonRect.y, buttonWidth, buttonHeight}; // Vùng click

    // Tải texture cổng
    gateTexture = loadTexture(renderer, "gate.png", gate, 0, 0, false); // gateRect sẽ được set trong setupLevel
    if (!gateTexture) return false;

    // Thiết lập Level 1
    setupLevel1();
    currentLevel = 1;
    isRunning = true;
    return true;
}
