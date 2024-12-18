#pragma once

#include "core/_index.hpp"

#include "Level.h"
#include "TitleScreen.h"
#include "level-loader.hpp"
#include "sfml_game_environment/SfmlGameEnvironment.h"

#include "entities/TitleMenuEntity.h"

#include "core/game_enviroment/SceneDirector.h"

#include "core/DeprecatedGameEnv.h"
#include "core/game_enviroment/EntityPoolEventLoop.h"

#include "scenes/TitleScreenScene.h"
#include "scenes/GameScene.h"

class Game {
public:
    Game(SfmlGameEnvironment &env, DeprecatedGameEnvironment &deprEnv)
        : sceneDirector_(env, entityPoolEventLoop_.pool()), depEnv_(deprEnv), entityPoolEventLoop_(env, deprEnv, sceneDirector_) {
        sceneDirector_.add(std::make_unique<TitleScreenScene>(deprEnv));
        sceneDirector_.add(std::make_unique<GameScene>(env, deprEnv));
    }
    void init();
    void update();
    void draw(artist_api::Artist &artist);
    bool isRunning();

private:
    DeprecatedGameEnvironment &depEnv_;
    entity_v3::EntityPoolEventLoop entityPoolEventLoop_;
    SceneDirector sceneDirector_;
};