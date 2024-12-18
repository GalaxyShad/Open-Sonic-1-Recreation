#ifndef OS1R_SCENE_H
#define OS1R_SCENE_H

#include "SceneContext.h"

struct Scene {
    virtual void onStart(const SceneStartContext & ctx) {};
    virtual void onUpdate(const SceneUpdateContext & ctx) {};
    virtual void onDraw(const SceneDrawContext & ctx) {};
    virtual void onExit(const SceneExitContext & ctx) {};

    virtual ~Scene() = default;
};

#endif // OS1R_SCENE_H
