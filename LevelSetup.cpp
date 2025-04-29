
#include "Game.h"
#include "Player.h"

void Game::setupLevel1() {
    const float activeZoneX = (SCREEN_WIDTH - ACTIVE_ZONE_WIDTH) / 2.0f; // Tọa độ mép trái vùng hoạt động
    const float activeZoneY = (SCREEN_HEIGHT - ACTIVE_ZONE_HEIGHT) / 2.0f;
    const float floorY = activeZoneY + ACTIVE_ZONE_HEIGHT;

    delete player;
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
