#ifndef OS1R_ENTITYPOOL_H
#define OS1R_ENTITYPOOL_H

#include <list>
#include <memory>
#include <stack>

#include "EntityV3.h"
#include "core/game_enviroment/EntityContext.h"

namespace entity_v3 {

class EntityPool {
public:
    struct EntityPoolNode {
        std::unique_ptr<entity_v3::Entity> ent;
        bool shouldBeDestroyed;
    };

public:
    EntityPool(std::list<EntityPoolNode> &entityList,
               entity_v3::InitContext &entInitCtx)
        : entityList_(entityList), initCtx_(entInitCtx) {}

    Entity &instantiate(std::unique_ptr<Entity> e) {
        e->onInit(initCtx_);

        entityList_.push_back(
            EntityPoolNode{.ent = std::move(e), .shouldBeDestroyed = false});

        return *entityList_.back().ent;
    }

    void destroy(Entity &ent) {}

    void clear() {
        for (auto &e : entityList_) {
            e.shouldBeDestroyed = true;
        }
    }

private:
    std::list<EntityPoolNode> &entityList_;
    entity_v3::InitContext &initCtx_;
};

} // namespace entity_v3

#endif // OS1R_ENTITYPOOL_H
