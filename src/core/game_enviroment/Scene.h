#ifndef OS1R_SCENE_H
#define OS1R_SCENE_H

#include "core/game_enviroment/artist/Artist.h"
#include "core/game_enviroment/InputMgr.h"



struct Scene {
    struct StartContext {

    };

    struct UpdateContext {
        IInputMgr& input;
    };

    struct DrawContext {
        artist_api::Artist& artist;
    };

    struct ExitContext {

    };

    virtual void onStart(const StartContext& ctx) {};
    virtual void onUpdate(const UpdateContext& ctx) {};
    virtual void onDraw(const DrawContext& ctx) {};
    virtual void onExit(const ExitContext& ctx) {};

    virtual ~Scene() = default;
};

#endif // OS1R_SCENE_H
