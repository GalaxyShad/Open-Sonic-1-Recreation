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

#include "scenes/GameScene.h"
#include "scenes/TitleScreenScene.h"

#include "sonic/SonicResources.h"

using LoadResourcesFunction = std::function<std::unique_ptr<IStorableResource>(
    ResourceStore &, GameEnvironment &, DeprecatedGameEnvironment &)>;

class Game {
public:
    static int RunGameWithSfmlBackend(const LoadResourcesFunction &loadResFun);

private:
    Game(GameEnvironment &env, SfmlArtist &sfmlArtist,
         const LoadResourcesFunction& loadResourcesFun);

    void loop();

private:
    void gameLogicLoop();
    void drawLoop();

private:
    GameEnvironment &env_;

    Audio deprAudio_;
    Screen deprScreen_;
    DeprecatedGameEnvironment deprEnv_;

    entity_v3::EntityPoolEventLoop entityPoolEventLoop_;
    SceneDirector sceneDirector_;
    ResourceStore store_;
};