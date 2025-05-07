
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

void Game::setupLevel2() {
    const float activeZoneX = (SCREEN_WIDTH - ACTIVE_ZONE_WIDTH) / 2.0f;
    const float activeZoneY = (SCREEN_HEIGHT - ACTIVE_ZONE_HEIGHT) / 2.0f;
    const float floorY = activeZoneY + ACTIVE_ZONE_HEIGHT;

    // Reset player
    delete player;
    player = new Player(activeZoneX + 30.0f, floorY - 32.0f, 32, 32, renderer);

    // Reset sàn và gai
    floorSegments.clear();
    spikes.clear();

    floorSegments.clear();

    // Sàn đầu
    const int startFloorWidth = 150;
    floorSegments.push_back({(int)activeZoneX, (int)(floorY - FLOOR_THICKNESS), startFloorWidth, FLOOR_THICKNESS});

    // HOLE_WIDTH: 50

    // Sàn thứ nhất
    const int stepWidth = 70;
    const int stepHeight = 15;
    const float step1X = activeZoneX + startFloorWidth + HOLE_WIDTH;
    const float step1Y = floorY - stepHeight;
    floorSegments.push_back({(int)step1X, (int)(step1Y - FLOOR_THICKNESS), stepWidth, FLOOR_THICKNESS});

    // Sàn thứ 2
    const int shortPlatformWidth1 = 70;
    const float step2X = step1X + stepWidth + HOLE_WIDTH;
    const float step2Y = step1Y - stepHeight;
    floorSegments.push_back({(int)step2X, (int)(step2Y - FLOOR_THICKNESS), shortPlatformWidth1, FLOOR_THICKNESS});

    // Sàn thứ 3
    const int shortPlatformWidth2 = 70;
    const float step3X = step2X + stepWidth + 30;
    const float step3Y = step2Y - stepHeight;
    floorSegments.push_back({(int)step3X, (int)(step3Y - FLOOR_THICKNESS), shortPlatformWidth2, FLOOR_THICKNESS});

    // Sàn cuối
    const int finalFloorWidth = 316;
    const float finalFloorX = step3X + shortPlatformWidth2 + HOLE_WIDTH;
    floorSegments.push_back({(int)finalFloorX, (int)(floorY - FLOOR_THICKNESS), finalFloorWidth, FLOOR_THICKNESS});

    // Thêm gai trên sàn cuối
    const int spikeWidth = 20;
    const int spikeHeight = 15;
    spikes.push_back({(int)(finalFloorX + 50), (int)(floorY - FLOOR_THICKNESS - spikeHeight), spikeWidth, spikeHeight});

    // Đặt cổng
    gate = {(int)(finalFloorX + finalFloorWidth - 50), (int)(floorY - 45), 45, 45};
    playerVisible = true;
}
