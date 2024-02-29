#pragma once

#include "entities/Entity.h"
#include "entity-placement.hpp"
#include "entity-pool.hpp"
#include "new-terrain.hpp"

#include <list>

class IEntityCreator {
public:
    virtual Entity* create(EntityPlacement entPlacement) = 0;
};

class EntityCreatorSonic1 : public IEntityCreator {
public:
    EntityCreatorSonic1(EntityPool& entityPool, terrain::Terrain& terrain) 
        : m_entityList(entityPool)
        , m_terrain(terrain)
    {} 

    Entity* create(EntityPlacement entPlacement);

private:
    EntityPool& m_entityList;
    terrain::Terrain& m_terrain;

    Entity* createEnemies(EntityPlacement eplc);
    Entity* createGeneral(EntityPlacement eplc);
    Entity* createOther(EntityPlacement eplc);
};