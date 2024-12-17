#include "SceneDirector.h"

SceneUniqueID SceneDirector::add(std::unique_ptr<Scene> scene) {
    sceneList_.push_back(std::move(scene));

    return sceneList_.size() - 1;
}

void SceneDirector::go(SceneUniqueID sceneId) {
    if (currentSceneId_ != -1) {
        sceneList_[currentSceneId_]->onExit();
    }

    currentSceneId_ = sceneId;

    sceneList_[currentSceneId_]->onStart();
}

void SceneDirector::update() {
    sceneList_[currentSceneId_]->onUpdate();
}

void SceneDirector::draw(artist_api::Artist& artist) {
    sceneList_[currentSceneId_]->onDraw(artist);
}
