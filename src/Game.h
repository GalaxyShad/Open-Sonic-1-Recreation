#pragma once

#include "core/_index.hpp"

#include "Level.h"
#include "TitleScreen.h"
#include "level-loader.hpp"
#include "sfml_game_environment/SfmlGameEnvironment.h"

#include "entities/TitleMenuEntity.h"

#include "core/game_enviroment/SceneDirector.h"

#include "core/DeprecatedGameEnv.h"
#include "scenes/TitleScreenScene.h"

class Game {
public:
    Game(SfmlGameEnvironment &env, DeprecatedGameEnvironment &deprEnv)
        : sceneDirector_(env), depEnv_(deprEnv) {
        sceneDirector_.add(std::make_unique<TitleScreenScene>(deprEnv));
    }
    void init();
    void update();
    void draw(artist_api::Artist &artist);
    bool isRunning();

private:
    DeprecatedGameEnvironment &depEnv_;
    SceneDirector sceneDirector_;
};