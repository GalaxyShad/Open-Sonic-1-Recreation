#include "Game.h"

#define TEX_OBJECTS 0
#define TEX_GHZ_GIMM 1

void Game::loop() {
    while (env_.isRunning()) {
        env_.beginFrame();
        gameLogicLoop();
        drawLoop();
        env_.endFrame();
    }
}

void Game::gameLogicLoop() {
    entityPoolEventLoop_.update();
    sceneDirector_.update();
}

void Game::drawLoop() {
    sceneDirector_.draw();
    entityPoolEventLoop_.draw();
}

int Game::RunGameWithSfmlBackend(const LoadResourcesFunction &loadResFun) {
    SfmlGameEnvironment env;

    Game game(env, env.sfmlArtist(), loadResFun);

    game.loop();

    return 0;
}

Game::Game(GameEnvironment &env, SfmlArtist &sfmlArtist,
           const LoadResourcesFunction &loadResourcesFun)
    : sceneDirector_(env, entityPoolEventLoop_.pool()),
      deprAudio_(env.dj(), store_),
      deprScreen_(sfmlArtist, store_),
      deprEnv_(
          DeprecatedGameEnvironment{.scr = deprScreen_, .audio = deprAudio_}),
      env_(env), entityPoolEventLoop_(env, deprEnv_, sceneDirector_) {
    env.init();

    store_.loadMapping(loadResourcesFun(store_, env, deprEnv_));

    sceneDirector_.add(std::make_unique<TitleScreenScene>(deprEnv_));
    sceneDirector_.add(std::make_unique<GameScene>(env, deprEnv_));
}
