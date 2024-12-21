#ifndef OS1R_TITLESCREENSCENE_H
#define OS1R_TITLESCREENSCENE_H

#include "core/game_enviroment/Scene.h"
#include "core/game_enviroment/artist/ArtistStructs.h"
#include "entities/TitleMenuEntity.h"
#include "sfml_game_environment/SfmlGameEnvironment.h"

#include "core/DeprecatedGameEnv.h"
#include <memory>

class TitleScreenScene : public Scene {
public:
    explicit TitleScreenScene(DeprecatedGameEnvironment &de)
        : titleScreenDeprecated_(de.scr), deprEnv_(de) {}

    void onStart(const SceneStartContext &ctx) override {
        ctx.entityPool.instantiate(
            std::make_unique<TitleMenuEntity>(titleScreenDeprecated_));

        texTitleBg_ = deprEnv_.scr.textureLoader().loadFromFile(
            "content/textures/texTitleBg.png");
    }

    void onUpdate(const SceneUpdateContext &ctx) override {}

    void onDraw(const SceneDrawContext &ctx) override {
        ctx.artist.drawSprite(
            artist_api::Sprite{.texture = *texTitleBg_,
                               .rect = {.width = 427, .height = 240}},
            {});
    }

private:
    TitleScreen titleScreenDeprecated_;
    DeprecatedGameEnvironment &deprEnv_;
    unique_ptr<artist_api::Texture> texTitleBg_;
};

#endif // OS1R_TITLESCREENSCENE_H
