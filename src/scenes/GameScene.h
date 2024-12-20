#ifndef OS1R_GAMESCENE_H
#define OS1R_GAMESCENE_H

#include "core/DeprecatedGameEnv.h"
#include "core/game_enviroment/GameEnvironment.h"
#include "core/game_enviroment/Scene.h"

#include "../Level.h"
#include "../level-loader.hpp"

class GameScene : public Scene {
public:
    explicit GameScene(GameEnvironment &env,
                       DeprecatedGameEnvironment &deprEnv);

public:
    void onStart(const SceneStartContext &ctx) override {
        lv = levelLoader_.loadFromSonic1(ZoneSonic1::GREEN_HILL, 1);
        lv->create();
    }

    void onUpdate(const SceneUpdateContext &ctx) override {
        if (!lv) return;

        lv->update();

        if (ctx.input.isKeyPressed(InputKey::GAME_RESTART)) {
            ctx.sceneDirector.go(0); // FIXME add proper identifier
        }

        if (ctx.input.isKeyPressed(InputKey::SCENE_RESTART)) {
            ctx.sceneDirector.go(1); // FIXME add proper identifier
        }
    }

    void onDraw(const SceneDrawContext &ctx) override {
        if (!lv) return;

        lv->draw();
    }

    void onExit(const SceneExitContext &ctx) override {
        lv->free();
        delete lv;
    }
private:
    Level *lv = nullptr;

    GameEnvironment &env_;
    DeprecatedGameEnvironment &deprEnv_;
    LevelLoader levelLoader_;
};

#endif // OS1R_GAMESCENE_H
