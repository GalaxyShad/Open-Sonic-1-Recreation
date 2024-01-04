#pragma once

#include "Entity.h"
#include "entity-placement.hpp"

#include <list>

class EntityCreator {
public:
    EntityCreator(std::list<Entity*>& entityList) 
        : m_entityList(entityList) {} 

    Entity* create(EntityPlacement entPlacement);

private:
    std::list<Entity*>& m_entityList;

    Entity* createEnemies(EntityPlacement eplc);
    Entity* createGeneral(EntityPlacement eplc);
    Entity* createOther(EntityPlacement eplc);
};