#ifndef OS1R_SCENEDIRECTOR_H
#define OS1R_SCENEDIRECTOR_H

#include <vector>

#include "EntityPool.h"
#include "ISceneDirector.h"
#include "Scene.h"
#include "core/game_enviroment/GameEnvironment.h"
#include "core/game_enviroment/SceneContext.h"

class SceneDirector : public ISceneDirector {
public:
    explicit SceneDirector(entity_v3::EntityPool &entityPool,
                           SceneContexts &sceneCtxs)
        : sceneCtxs_(sceneCtxs), entityPool_(entityPool) {}

    SceneUniqueID add(std::unique_ptr<Scene> scene);

    void go(SceneUniqueID sceneId) override;

    void update();
    void draw();

private:
    std::vector<std::unique_ptr<Scene>> sceneList_;
    SceneUniqueID currentSceneId_ = -1;
    SceneContexts &sceneCtxs_;
    entity_v3::EntityPool &entityPool_;
};

#endif // OS1R_SCENEDIRECTOR_H
