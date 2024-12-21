#ifndef OS1R_ENTITYPOOL_H
#define OS1R_ENTITYPOOL_H

#include <list>

#include "EntityV3.h"

namespace entity_v3 {

class EntityPool {
public:
    EntityPool(std::list<std::unique_ptr<Entity>> &entityList)
        : entityList_(entityList) {}

    Entity &instantiate(std::unique_ptr<Entity> e) {
        e->onInit({});

        entityList_.emplace_back(std::move(e));

        return *entityList_.back();
    }

    void clear() {
        shouldBeCleared_ = true;
    }

    void destroy(Entity &&ent) {}

    inline bool isShouldBeCleared() {
        return shouldBeCleared_;
    }

private:
    std::list<std::unique_ptr<Entity>> &entityList_;
    bool shouldBeCleared_ = false;
};

} // namespace entity_v3

#endif // OS1R_ENTITYPOOL_H
