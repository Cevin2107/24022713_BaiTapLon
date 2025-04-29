#include "Game.h"
#include "Player.h"
#include <iostream>

using namespace std;
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
        // Vẽ Start.png
        if (startButtonTexture) {
            SDL_RenderCopy(renderer, startButtonTexture, NULL, &startButtonRect);
        } else {
            cerr << "Lỗi: startButtonTexture là nullptr!" << endl;
            SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
            SDL_RenderFillRect(renderer, &startButton);
        }

        // Vẽ Quit.png
        if (quitButtonTexture) {
            SDL_RenderCopy(renderer, quitButtonTexture, NULL, &quitButtonRect);
        } else {
            cerr << "Lỗi: quitButtonTexture là nullptr!" << endl;
            SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
            SDL_RenderFillRect(renderer, &quitButton);
        }
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
