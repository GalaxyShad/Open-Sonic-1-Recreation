#include "entity-pool.hpp"
#include "entities/Entity.h"
#include "entities/Player.h"

void EntityPool::init() {
    for (auto &it : m_pool) {
        it->init();
    }
}

void EntityPool::create(Entity *entity) {
    m_pool.push_back(entity);
    entity->init();
}

void EntityPool::destroy(Entity &entity) {
    m_entitiesToDestroy.push_back(&entity);
}

void EntityPool::update() {
    for (auto &it : m_pool) {
        if (!isEntityInsideView(*it))
            continue;

        it->update();

        legacyUpdate(*it);

        if (it->chitbox().exists()) {
            auto &hb = it->chitbox().get();

            for (auto &e : m_pool) {
                if (!isEntityInsideView(*e))
                    continue;

                if (e->chitbox().exists() &&
                    hb.isOverlappingWith(e->chitbox().get())) {
                    it->onHitboxCollision(*e);
                }
            }
        }

        if (!isEntityInsideView(*it))
            it->onOutOfView();
    }

    legacyCleanup();
    cleanup();
}

void EntityPool::draw() {
    for (auto &it : m_pool) {
        it->draw(m_camera);

        if (it->chitbox().exists())
			it->chitbox().get().draw(m_camera);
		
		if (it->d_isInCamera(m_camera)) {
			it->d_draw(m_camera);
		}
    }
}

bool EntityPool::isEntityInsideView(Entity &entity) {
    if (entity.type() == EntityTypeID::DEPRECATED)
        return entity.d_isInCamera(m_camera);

    if (!entity.chitbox().exists())
        return true;

    const int DEACT_DELTA = 224;

    auto &hitbox = entity.chitbox().get();
    auto viewPos = m_camera.getCenterPosition();

    return (hitbox.getLeft()   > viewPos.x - DEACT_DELTA) &&
           (hitbox.getRight()  < viewPos.x + DEACT_DELTA) &&
           (hitbox.getTop()    > viewPos.y - DEACT_DELTA) &&
           (hitbox.getBottom() < viewPos.y + DEACT_DELTA);
}

void EntityPool::legacyUpdate(Entity &entity) {
    if (entity.d_isInCamera(m_camera) || entity.d_getType() == TYPE_PLAYER) {

        entity.d_update();
        entity.d_reactingToOthers(m_pool);
        if (entity.d_getType() == TYPE_PLAYER) {
            auto& pl = entity.castTo<Player>();

            if (pl.isEndLv()) {
                // TODO stop time
                // TODO instanciate level informer
            }

            if (pl.isDied()) {
                // TODO death fader
            }
        }
    } else if (entity.d_isLiving()) {
        entity.onOutOfView();
    }
}

void EntityPool::legacyCleanup() {
    for (auto it = m_pool.begin(); it != m_pool.end();) {
        Entity *ent = *it;
        if (!ent->d_isLiving()) {
            it = m_pool.erase(it);
            delete ent;
        } else {
            it++;
        }
    }
}

void EntityPool::cleanup() {
    for (auto &it : m_entitiesToDestroy) {
        m_pool.remove(it);
        it->onDestroy();
        delete it;
    }

    m_entitiesToDestroy.clear();
}
