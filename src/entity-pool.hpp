#pragma once

#include "Camera.h"
#include "entities/Entity.h"

class EntityPool {
public:
    EntityPool(Camera& camera, std::list<Entity*>& entities) 
        : m_camera(camera)
        , m_pool(entities) 
    {}

    void init();
    void create(Entity* entity);
    void destroy(Entity& entity);
    void update();
    void draw();

    std::list<Entity*>& legacy_rawPool() { return m_pool; }

private:    
    std::list<Entity*>& m_pool;
    std::list<Entity*>  m_entitiesToDestroy;

    Camera& m_camera;

private:
    bool isEntityInsideView(Entity& entity);
    void legacyUpdate(Entity& entity);
    void legacyCleanup();
    void cleanup();
};