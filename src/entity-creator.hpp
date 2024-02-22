#pragma once

#include "Terrain.h"
#include "entities/Entity.h"
#include "entity-placement.hpp"
#include "new-terrain.hpp"

#include <list>

class IEntityCreator {
public:
    virtual Entity* create(EntityPlacement entPlacement) = 0;
};

class EntityCreatorSonic1 : public IEntityCreator {
public:
    EntityCreatorSonic1(std::list<Entity*>& entityList, terrain::Terrain& terrain) 
        : m_entityList(entityList)
        , m_terrain(terrain)
    {} 

    Entity* create(EntityPlacement entPlacement);

private:
    std::list<Entity*>& m_entityList;
    terrain::Terrain& m_terrain;

    Entity* createEnemies(EntityPlacement eplc);
    Entity* createGeneral(EntityPlacement eplc);
    Entity* createOther(EntityPlacement eplc);
};