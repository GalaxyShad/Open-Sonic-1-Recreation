#pragma once

#include "entities/Entity.h"
#include "entity-placement.hpp"

#include <list>

class IEntityCreator {
public:
    virtual Entity* create(EntityPlacement entPlacement) = 0;
};

class EntityCreatorSonic1 : public IEntityCreator {
public:
    EntityCreatorSonic1(std::list<Entity*>& entityList) 
        : m_entityList(entityList) {} 

    Entity* create(EntityPlacement entPlacement);

private:
    std::list<Entity*>& m_entityList;

    Entity* createEnemies(EntityPlacement eplc);
    Entity* createGeneral(EntityPlacement eplc);
    Entity* createOther(EntityPlacement eplc);
};