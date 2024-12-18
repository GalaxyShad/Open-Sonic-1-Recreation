#ifndef OS1R_SCENEDIRECTOR_H
#define OS1R_SCENEDIRECTOR_H

#include <vector>

#include "core/game_enviroment/GameEnvironment.h"
#include "ISceneDirector.h"
#include "Scene.h"



class SceneDirector : public ISceneDirector {
public:
    explicit SceneDirector(GameEnvironment& e) : env_(e) {}

    SceneUniqueID add(std::unique_ptr<Scene> scene);

    void go(SceneUniqueID sceneId) override;

    void update();
    void draw();

private:
    std::vector<std::unique_ptr<Scene>> sceneList_;
    SceneUniqueID currentSceneId_ = -1;
    GameEnvironment& env_;
};

#endif // OS1R_SCENEDIRECTOR_H
