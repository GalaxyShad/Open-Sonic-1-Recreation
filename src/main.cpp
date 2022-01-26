
#include "Game.h"

int main() {
    Game game;
    game.init();
    
    while (game.isRunning()) {
        game.update();
    }

    return 0;
}