#ifndef OS1R_SCENEDIRECTOR_H
#define OS1R_SCENEDIRECTOR_H

#include <vector>

#include "core/game_enviroment/GameEnvironment.h"
#include "ISceneDirector.h"
#include "Scene.h"
#include "EntityPool.h"



class SceneDirector : public ISceneDirector {
public:
    explicit SceneDirector(GameEnvironment& e, entity_v3::EntityPool& entityPool) : env_(e), entityPool_(entityPool) {}

    SceneUniqueID add(std::unique_ptr<Scene> scene);

    void go(SceneUniqueID sceneId) override;

    void update();
    void draw();

private:
    std::vector<std::unique_ptr<Scene>> sceneList_;
    SceneUniqueID currentSceneId_ = -1;
    GameEnvironment& env_;
    entity_v3::EntityPool& entityPool_;
};

#endif // OS1R_SCENEDIRECTOR_H
