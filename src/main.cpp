
#include "Game.h"

#include "core/ResourceStore.h"

#include "core/SfmlGameEnvironment.h"

int main() {
    SfmlGameEnvironment env;
    env.init();

    ResourceStore store;

    Screen scr(env.artist(), store);
    TitleScreen titleScreen(scr);
    Game game(scr, titleScreen, env);
    game.init();

    while (game.isRunning()) {
        env.beginFrame();
        game.update();
        game.draw(env.artist());
        env.endFrame();
    }

    return 0;
}