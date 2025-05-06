#include "Game.h"
#include "Player.h"
#include <iostream>

using namespace std;

// Cập nhật trạng thái trò chơi
void Game::update() {
    // Xử lý đầu vào và cập nhật nhân vật
    const Uint8* keyState = SDL_GetKeyboardState(NULL);
    player->handleInput(event, keyState);
    player->update(floorSegments);

    // Kiểm tra game over (nhân vật rơi khỏi màn hình)
    if (player->getRect().y > SCREEN_HEIGHT) {
        gameOver = true;
        cout << "Game Over! Press any key to restart!" << endl;
    }

    // Kiểm tra qua level (nhân vật chạm cổng)
    SDL_Rect playerRect = player->getRect();
    if (SDL_HasIntersection(&playerRect, &gate)) {
        levelPassed = true;
        playerVisible = false;
        if (currentLevel == 1) {
            cout << "Level 1 Passed! Moving to Level 2!" << endl;
        } else if (currentLevel == 2) {
            cout << "Level 2 Passed! Game Complete!" << endl;
        }
    }
}

// Vẽ texture hoặc hình chữ nhật thay thế nếu texture không tải được
void Game::renderTextureOrRect(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b) {
    if (texture) {
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    } else {
        cerr << "Lỗi: Texture là nullptr!" << endl;
        SDL_SetRenderDrawColor(renderer, r, g, b, 0xFF);
        SDL_RenderFillRect(renderer, &rect);
    }
}

// Vẽ các thành phần trò chơi lên màn hình
void Game::render() {
    // Xóa màn hình với nền nâu nhạt
    SDL_SetRenderDrawColor(renderer, 0xD2, 0xB4, 0x8C, 0xFF);
    SDL_RenderClear(renderer);

    if (isInMenu) {
        // Vẽ menu
        renderTextureOrRect(renderer, backgroundTexture, backgroundRect, 0x00, 0x00, 0x00);
        renderTextureOrRect(renderer, startButtonTexture, startButtonRect, 0x00, 0xFF, 0x00); // Xanh lá nếu lỗi
        renderTextureOrRect(renderer, quitButtonTexture, quitButtonRect, 0xFF, 0x00, 0x00);   // Đỏ nếu lỗi
    } else {
        // Vẽ màn chơi
        renderTextureOrRect(renderer, logoTexture, logoRect, 0xFF, 0xFF, 0xFF);               // Trắng nếu lỗi
        renderTextureOrRect(renderer, instructionTexture, instructionRect, 0xFF, 0xFF, 0xFF); // Trắng nếu lỗi

        // Vẽ vùng hoạt động (vàng nhạt)
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x99, 0xFF);
        SDL_Rect activeZone = {
            (SCREEN_WIDTH - ACTIVE_ZONE_WIDTH) / 2,
            (SCREEN_HEIGHT - ACTIVE_ZONE_HEIGHT) / 2,
            ACTIVE_ZONE_WIDTH,
            ACTIVE_ZONE_HEIGHT
        };
        SDL_RenderFillRect(renderer, &activeZone);

        // Vẽ các đoạn sàn (màu tím đậm)
        SDL_SetRenderDrawColor(renderer, 0x8A, 0x67, 0x8F, 0xFF);
        for (const auto& segment : floorSegments) {
            SDL_RenderFillRect(renderer, &segment);
        }

        // Vẽ nhân vật nếu được hiển thị
        if (playerVisible) {
            player->render(renderer);
        }

        // Vẽ cổng bằng texture
        renderTextureOrRect(renderer, gateTexture, gate, 0x80, 0x80, 0x80); // Xám nếu lỗi
    }

    // Cập nhật màn hình
    SDL_RenderPresent(renderer);
}

// Vòng lặp chính của trò chơi
void Game::run() {
    while (isRunning) {
        // Xử lý sự kiện
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            } else if (isInMenu && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                // Xử lý click chuột trong menu
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                SDL_Point mousePoint = {mouseX, mouseY};
                if (SDL_PointInRect(&mousePoint, &startButton)) {
                    isInMenu = false; // Bắt đầu trò chơi
                } else if (SDL_PointInRect(&mousePoint, &quitButton)) {
                    isRunning = false; // Thoát trò chơi
                }
            } else if ((gameOver || levelPassed) && event.type == SDL_KEYDOWN) {
                // Xử lý chuyển level hoặc khởi động lại
                if (gameOver) {
                    setupLevel1();
                    currentLevel = 1;
                    gameOver = false;
                    playerVisible = true;
                } else if (levelPassed && currentLevel == 1) {
                    setupLevel2();
                    currentLevel = 2;
                    levelPassed = false;
                    playerVisible = true;
                } else if (levelPassed && currentLevel == 2) {
                    setupLevel1();
                    currentLevel = 1;
                    levelPassed = false;
                    playerVisible = true;
                }
            }
        }

        // Cập nhật trạng thái nếu không game over hoặc qua level
        if (!gameOver && !levelPassed && !isInMenu) {
            update();
        }

        // Vẽ khung hình
        render();
    }
}
