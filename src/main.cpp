
#include "Game.h"

#include "core/SfmlGameEnvironment.h"

int main() {
    SfmlGameEnvironment env;
    env.init();

    Screen scr(env.artist(), env.textureStore());
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