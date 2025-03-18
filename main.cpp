#include "Game.h"
#include <iostream>

using namespace std;

int main(int argc, char* args[]) {
    Game game;
    if (!game.init()) {
        cerr << "Failed to initialize!" << endl;
        return -1;
    }
    game.run();
    return 0;
}
