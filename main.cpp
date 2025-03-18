#include <SDL.h>
#include <iostream>

using namespace std;

const int SCREEN_WIDTH = 1089;
const int SCREEN_HEIGHT = 612;

const int ACTIVE_ZONE_HEIGHT = 121;
const int ACTIVE_ZONE_WIDTH = 856;

const int PLAYER_HEIGHT = 32;
const int PLAYER_WIDTH = 12;

const int GATE_WIDTH = 30;
const int GATE_HEIGHT = 35;

const int HOLE_WIDTH = 40;
const int HOLE_HEIGHT = SCREEN_HEIGHT - ((SCREEN_HEIGHT - ACTIVE_ZONE_HEIGHT) / 2 + ACTIVE_ZONE_HEIGHT);

const int FLAT_HEIGHT = 320; // Chiều cao của mặt sàn (nhân vật di chuyển)

bool init();
void close();

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;

bool init() {
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        success = false;
    } else {
        gWindow = SDL_CreateWindow("Fall Gravity", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == nullptr) {
            cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
            success = false;
        } else {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == nullptr) {
                cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
                success = false;
            } else {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            }
        }
    }

    return success;
}

void close() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    gRenderer = nullptr;

    SDL_Quit();
}

void drawActiveZone() {
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x99, 0xFF); // Vàng nhạt
    SDL_Rect activeZone = { (SCREEN_WIDTH - ACTIVE_ZONE_WIDTH) / 2, (SCREEN_HEIGHT - ACTIVE_ZONE_HEIGHT) / 2, ACTIVE_ZONE_WIDTH, ACTIVE_ZONE_HEIGHT };
    SDL_RenderFillRect(gRenderer, &activeZone);
}

void drawPlayer(int x, int y) {
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF); // Xanh nước biển
    SDL_Rect player = { x, y, PLAYER_WIDTH, PLAYER_HEIGHT };
    SDL_RenderFillRect(gRenderer, &player);
}

void drawGate(int x, int y) {
    SDL_SetRenderDrawColor(gRenderer, 0x80, 0x80, 0x80, 0xFF); // Xám
    SDL_Rect gate = { x, y, GATE_WIDTH, GATE_HEIGHT };
    SDL_RenderFillRect(gRenderer, &gate);
}

void drawHole(int x, int y) {
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x99, 0xFF); // Đen
    SDL_Rect hole = { x, y, HOLE_WIDTH, HOLE_HEIGHT };
    SDL_RenderFillRect(gRenderer, &hole);
}

int main(int argc, char* args[]) {
    if (!init()) {
        cerr << "Failed to initialize!" << endl;
    } else {
        bool quit = false;
        SDL_Event e;

        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }

            SDL_SetRenderDrawColor(gRenderer, 0xD2, 0xB4, 0x8C, 0xFF); // Nâu nhạt
            SDL_RenderClear(gRenderer);

            drawActiveZone();
            drawPlayer((SCREEN_WIDTH - ACTIVE_ZONE_WIDTH) / 2 + 30, (SCREEN_HEIGHT - ACTIVE_ZONE_HEIGHT) / 2 + ACTIVE_ZONE_HEIGHT - PLAYER_HEIGHT);
            drawGate((SCREEN_WIDTH - ACTIVE_ZONE_WIDTH) / 2 + ACTIVE_ZONE_WIDTH - 50, (SCREEN_HEIGHT - ACTIVE_ZONE_HEIGHT) / 2 + ACTIVE_ZONE_HEIGHT - GATE_HEIGHT);
            drawHole((SCREEN_WIDTH - ACTIVE_ZONE_WIDTH) / 2 + 726, (SCREEN_HEIGHT - ACTIVE_ZONE_HEIGHT) / 2 + ACTIVE_ZONE_HEIGHT);

            SDL_RenderPresent(gRenderer);
        }
    }

    close();
    return 0;
}
