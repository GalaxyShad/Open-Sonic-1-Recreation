#ifndef OS1R_ENTITYPOOLEVENTLOOP_H
#define OS1R_ENTITYPOOLEVENTLOOP_H

#include "core/game_enviroment/EntityContext.h"
#include "core/game_enviroment/EntityPool.h"
#include "core/game_enviroment/EntityV3.h"

#include <memory>

namespace entity_v3 {
class EntityPoolEventLoop {
public:
public:
    EntityPoolEventLoop(entity_v3::Contexts &entCtxs)
        : pool_(entityList_, entCtxs.init), entCtxs_(entCtxs) {}

    void update() {
        if (entityList_.empty())
            return;

        for (auto &e : entityList_) {
            e.ent->onUpdate(entCtxs_.update);
        }

        for (auto it = entityList_.begin(); it != entityList_.end();) {
            if (it->shouldBeDestroyed) {
                it = entityList_.erase(it);
            } else {
                ++it;
            }
        }
    }

    void draw() {
        if (entityList_.empty())
            return;

        for (auto &e : entityList_) {
            e.ent->onDraw(entCtxs_.draw);
        }
    }

    EntityPool &pool() { return pool_; }

private:
    entity_v3::Contexts &entCtxs_;
    EntityPool pool_;

    std::list<EntityPool::EntityPoolNode> entityList_;
};
} // namespace entity_v3

#endif // OS1R_ENTITYPOOLEVENTLOOP_H
