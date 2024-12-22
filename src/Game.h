#pragma once

#include "core/_index.hpp"

#include "Level.h"
#include "TitleScreen.h"
#include "core/game_enviroment/EntityContext.h"
#include "core/game_enviroment/EntityV3.h"
#include "core/game_enviroment/SceneContext.h"
#include "entities/Entity.h"
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
         const LoadResourcesFunction &loadResourcesFun);

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

private:
    // clang-format off

    entity_v3::InitContext entInitCtx_ = {};

    entity_v3::UpdateContext entUpdCtx_ = entity_v3::UpdateContext {
        .input = env_.input(), 
        .sceneDirector = sceneDirector_
    };

    entity_v3::DrawContext entDrawCtx_ = entity_v3::DrawContext {
        .artist = env_.artist(), 
        .deprecatedScreen = deprScreen_
    };

    entity_v3::Contexts entCtxs_ = entity_v3::Contexts {
        .init = entInitCtx_, 
        .draw = entDrawCtx_, 
        .update = entUpdCtx_
    };

    ///////////////////////////////
    ///////////////////////////////

    SceneStartContext sceneStartCtx_ = SceneStartContext {
        .entityPool = entityPoolEventLoop_.pool()
    };

    SceneUpdateContext sceneUpdateCtx_ = SceneUpdateContext {
        .sceneDirector = sceneDirector_,
        .input = env_.input()
    };

    SceneDrawContext sceneDrawCtx_ = SceneDrawContext {
        .artist = env_.artist()
    };

    SceneExitContext sceneExitCtx_ = SceneExitContext {};

    SceneContexts sceneCtxs_ = SceneContexts {
        .start = sceneStartCtx_,
        .update = sceneUpdateCtx_,
        .draw = sceneDrawCtx_,
        .exit = sceneExitCtx_
    };

    // clang-format on
};