#include "Game.h"
#include "Player.h"
#include <iostream>

using namespace std;

// Hàm khởi tạo của lớp Game
Game::Game() : window(nullptr), renderer(nullptr), isRunning(false), player(nullptr), gameOver(false), levelPassed(false), playerVisible(true), isInMenu(true) {
    // Khởi tạo nút Start và Quit
    const int startY = (SCREEN_HEIGHT - (buttonHeight * 2 + buttonSpacing)) / 2; // Căn giữa
    startButton = {(SCREEN_WIDTH - buttonWidth) / 2, startY, buttonWidth, buttonHeight}; // Nút Start ở giữa
    quitButton = {(SCREEN_WIDTH - buttonWidth) / 2, startY + buttonHeight + buttonSpacing, buttonWidth, buttonHeight}; // Nút Quit ngay dưới
    }

// Hàm hủy của lớp Game
Game::~Game() {
    delete player; // Giải phóng bộ nhớ của player
    SDL_DestroyTexture(logoTexture); // Giải phóng texture logo
    SDL_DestroyTexture(instructionTexture); // Giải phóng texture instruction
    SDL_DestroyTexture(gateTexture); // Giải phóng cổng
    SDL_DestroyRenderer(renderer); // Hủy renderer
    SDL_DestroyWindow(window); // Hủy window
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
    logoRect = {(SCREEN_WIDTH - logoWidth) / 2, -135, logoWidth, logoHeight}; // Ở giữa, và dịch lên trên

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
    // Đặt vị trí instruction
    int instructionWidth, instructionHeight;
    SDL_QueryTexture(instructionTexture, NULL, NULL, &instructionWidth, &instructionHeight);
    const int activeZoneY = (SCREEN_HEIGHT - ACTIVE_ZONE_HEIGHT) / 2;
    // const int instructionY = activeZoneY + ACTIVE_ZONE_HEIGHT - 50;
    instructionRect = {(SCREEN_WIDTH - instructionWidth) / 2, 320, instructionWidth, instructionHeight};

    setupLevel1(); // Thiết lập Level 1 Easy
    isRunning = true;
    return true;
}

// Thiết lập Level 1 của trò chơi
void Game::setupLevel1() {
    const float activeZoneX = (SCREEN_WIDTH - ACTIVE_ZONE_WIDTH) / 2.0f; // Tọa độ mép trái vùng hoạt động
    const float activeZoneY = (SCREEN_HEIGHT - ACTIVE_ZONE_HEIGHT) / 2.0f;
    const float floorY = activeZoneY + ACTIVE_ZONE_HEIGHT;

    player = new Player(activeZoneX + 30.0f, floorY - 32.0f, 32, 32, renderer);

    const int numHoles = 3;
    const float floorWidth = (ACTIVE_ZONE_WIDTH - numHoles * HOLE_WIDTH) / (numHoles + 1);
    floorSegments.clear(); // Xóa các đoạn sàn từ game cũ
    for (int i = 0; i <= numHoles; i++) {
        float x = activeZoneX + i * (floorWidth + HOLE_WIDTH);
        floorSegments.push_back({(int)x, (int)(floorY - FLOOR_THICKNESS), (int)floorWidth, FLOOR_THICKNESS});
    }

    gate = {(int)(activeZoneX + ACTIVE_ZONE_WIDTH - 50), (int)(floorY - 45), 45, 45}; // Đặt vị trí và kích cỡ cổng
    playerVisible = true; // Nhân vật hiển thị khi chơi
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
        playerVisible = false;
        cout << "Easy Pass!" << endl; // Thắng thì qua màn Level 1
    }
}
// Vẽ các thành phần của trò chơi lên màn hình
void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0xD2, 0xB4, 0x8C, 0xFF); // Nền nâu nhạt
    SDL_RenderClear(renderer);

    if (isInMenu) {
        // Vẽ nút Start (màu xanh lá)
        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF); // Màu xanh lá
        SDL_RenderFillRect(renderer, &startButton);

        // Vẽ nút Quit (màu đỏ)
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF); // Màu đỏ
        SDL_RenderFillRect(renderer, &quitButton);
    } else {
    // Vẽ logo
    SDL_RenderCopy(renderer, logoTexture, NULL, &logoRect);
    // Vẽ instruction
    SDL_RenderCopy(renderer, instructionTexture, NULL, &instructionRect);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x99, 0xFF); // Vùng active zone màu vàng nhạt
    SDL_Rect activeZone = {(SCREEN_WIDTH - ACTIVE_ZONE_WIDTH) / 2, (SCREEN_HEIGHT - ACTIVE_ZONE_HEIGHT) / 2, ACTIVE_ZONE_WIDTH, ACTIVE_ZONE_HEIGHT};
    SDL_RenderFillRect(renderer, &activeZone); // Vẽ vùng hoạt động

    SDL_SetRenderDrawColor(renderer, 0x8A, 0x67, 0x8F, 0xFF); // Đặt màu cho sàn
    for (const auto& segment : floorSegments) {
        SDL_RenderFillRect(renderer, &segment); // Vẽ các đoạn sàn
    }
    if (playerVisible) {
    player->render(renderer); // Vẽ nhân vật khi playerVisible được đặt là true
    }

    // SDL_SetRenderDrawColor(renderer, 0x80, 0x80, 0x80, 0xFF); // Đặt màu cho cổng
    // SDL_RenderFillRect(renderer, &gate); // Vẽ cổng
    SDL_RenderCopy(renderer, gateTexture, NULL, &gate); // Vẽ cổng bằng texture
    }
    SDL_RenderPresent(renderer); // Cập nhật màn hình
}

// Vòng lặp chính của trò chơi
void Game::run() {
    while (isRunning) { // Khi trò chơi đang chạy
        while (SDL_PollEvent(&event) != 0) { // Xử lý các sự kiện
            if (event.type == SDL_QUIT) {
                isRunning = false;
                // Logic khi nhất nút Start Quit
                } else if (isInMenu && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                // Lấy tọa độ chuột
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                SDL_Point mousePoint = {mouseX, mouseY};
                // Kiểm tra nhấn nút Start
                if (SDL_PointInRect(&mousePoint, &startButton)) {
                    isInMenu = false; // Chuyển sang màn hình trò chơi
                }
                // Kiểm tra nhấn nút Quit
                else if (SDL_PointInRect(&mousePoint, &quitButton)) {
                    isRunning = false; // Thoát trò chơi
                }
            } else if ((gameOver || levelPassed) && event.type == SDL_KEYDOWN) { // Sửa lại đoạn này xíu
                setupLevel1(); // Thiết lập lại cấp độ 1
                gameOver = false; // Đặt lại gameOver
                // levelPassed = false;
                playerVisible = true;
            }
        }

        if (!gameOver && !levelPassed) {
            update(); // Cập nhật trạng thái trò chơi nếu chưa kết thúc và chưa qua màn
        }
        render(); // Vẽ các thành phần của trò chơi
    }
}
