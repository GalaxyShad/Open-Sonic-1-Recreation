
#include "Game.h"

#include "core/game_enviroment/ResourceStore.h"
#include "core/DeprecatedGameEnv.h"

#include "sfml_game_environment/SfmlGameEnvironment.h"

int main() {
    SfmlGameEnvironment env;
    env.init();
    ResourceStore store;

    Screen scr(env.sfmlArtist(), store);
    Audio audio(env.dj(), env.audioLoader(), store);
    DeprecatedGameEnvironment deprEnv = {
        .scr = scr,
        .audio = audio
    };

    Game game(env, deprEnv);
    game.init();

    while (game.isRunning()) {
        env.beginFrame();
        game.update();
        game.draw(env.artist());
        env.endFrame();
    }

    return 0;
}