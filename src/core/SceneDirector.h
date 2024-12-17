#ifndef OS1R_SCENEDIRECTOR_H
#define OS1R_SCENEDIRECTOR_H

#include <vector>

#include "Scene.h"

using SceneUniqueID = long;

class SceneDirector final {
  public:
    SceneUniqueID add(std::unique_ptr<Scene> scene);

    void go(SceneUniqueID sceneId);

    void update();
    void draw(artist_api::Artist& artist);

  private:
    std::vector<std::unique_ptr<Scene>> sceneList_;
    SceneUniqueID currentSceneId_ = -1;
};

#endif // OS1R_SCENEDIRECTOR_H
