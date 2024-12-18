#ifndef OS1R_TITLESCREENSCENE_H
#define OS1R_TITLESCREENSCENE_H

#include "core/game_enviroment/Scene.h"
#include "entities/TitleMenuEntity.h"
#include "sfml_game_environment/SfmlGameEnvironment.h"

#include "core/DeprecatedGameEnv.h"

class TitleScreenScene : public Scene {
public:
    explicit TitleScreenScene(DeprecatedGameEnvironment &de)
        : titleScreenDeprecated_(de.scr), deprEnv_(de) {}

    void onStart(const SceneStartContext &ctx) override {
        ctx.entityPool.instantiate(std::make_unique<TitleMenuEntity>(titleScreenDeprecated_));

        deprEnv_.scr.loadTextureFromFile("content/textures/texTitleBg.png", 253);
    }

    void onUpdate(const SceneUpdateContext &ctx) override {
        
    }

    void onDraw(const SceneDrawContext &ctx) override {
        deprEnv_.scr.drawTextureRect(253, irect(0, 0, 427, 240));
    }

private:
    TitleScreen titleScreenDeprecated_;
    DeprecatedGameEnvironment &deprEnv_;
};

#endif // OS1R_TITLESCREENSCENE_H
