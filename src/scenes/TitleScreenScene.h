#ifndef OS1R_TITLESCREENSCENE_H
#define OS1R_TITLESCREENSCENE_H

#include "core/game_enviroment/Scene.h"
#include "entities/TitleMenuEntity.h"
#include "sfml_game_environment/SfmlGameEnvironment.h"

#include "core/DeprecatedGameEnv.h"

class TitleScreenScene : public Scene {
public:
    explicit TitleScreenScene(DeprecatedGameEnvironment &de)
        : titleScreenDeprecated_(de.scr), deprEnv_(de),
          menuEntity_(titleScreenDeprecated_) {}

    void onUpdate(const SceneUpdateContext &ctx) override {
        menuEntity_.onUpdate(
            {.input = ctx.input, .sceneDirector = ctx.sceneDirector});
    }

    void onDraw(const SceneDrawContext &ctx) override {
        menuEntity_.onDraw(
            {.artist = ctx.artist, .deprecatedScreen = deprEnv_.scr});
    }

private:
    TitleMenuEntity menuEntity_;
    TitleScreen titleScreenDeprecated_;
    DeprecatedGameEnvironment &deprEnv_;
};

#endif // OS1R_TITLESCREENSCENE_H
