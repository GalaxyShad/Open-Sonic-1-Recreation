#include "SceneDirector.h"

#include "Scene.h"

SceneUniqueID SceneDirector::add(std::unique_ptr<Scene> scene) {
    sceneList_.push_back(std::move(scene));

    if (currentSceneId_ == -1) {
        currentSceneId_ = 0;
        sceneList_[currentSceneId_]->onStart({
            .entityPool = entityPool_
        });
    }

    return sceneList_.size() - 1;
}

void SceneDirector::go(SceneUniqueID sceneId) {


    if (currentSceneId_ != -1) {
        sceneList_[currentSceneId_]->onExit(sceneCtxs_.exit);
    }

    currentSceneId_ = sceneId;

    entityPool_.clear();

    sceneList_[currentSceneId_]->onStart(sceneCtxs_.start);
}

void SceneDirector::update() {
    sceneList_[currentSceneId_]->onUpdate(sceneCtxs_.update);
}

void SceneDirector::draw() {
    sceneList_[currentSceneId_]->onDraw(sceneCtxs_.draw);
}
