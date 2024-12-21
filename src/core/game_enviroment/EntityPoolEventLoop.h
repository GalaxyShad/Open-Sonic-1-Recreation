#ifndef OS1R_ENTITYPOOLEVENTLOOP_H
#define OS1R_ENTITYPOOLEVENTLOOP_H

#include "GameEnvironment.h"
#include "../DeprecatedGameEnv.h"
#include "SceneDirector.h"

namespace entity_v3 {
class EntityPoolEventLoop {
public:
    EntityPoolEventLoop(GameEnvironment &env, DeprecatedGameEnvironment &denv,
                        SceneDirector &sceneDirector)
        : gameEnv_(env), deprGameEnv(denv), pool_(entityList_),
          sceneDirector_(sceneDirector) {}

    void update() {
        if (entityList_.empty()) return;

        for (auto &e : entityList_) {
            e->onUpdate(
                {.input = gameEnv_.input(), .sceneDirector = sceneDirector_});
        }

        if (pool_.isShouldBeCleared()) {
            entityList_.clear();
        }
    }

    void draw() {
        if (entityList_.empty()) return;

        for (auto &e : entityList_) {
            e->onDraw({.artist = gameEnv_.artist(),
                       .deprecatedScreen = deprGameEnv.scr});
        }
    }

    EntityPool& pool() {
        return pool_;
    }

private:
    GameEnvironment &gameEnv_;
    SceneDirector &sceneDirector_;
    DeprecatedGameEnvironment &deprGameEnv;
    EntityPool pool_;
    std::list<std::unique_ptr<Entity>> entityList_;
};
}

#endif // OS1R_ENTITYPOOLEVENTLOOP_H
