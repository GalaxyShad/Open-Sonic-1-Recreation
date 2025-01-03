#pragma once

#include "core/game_enviroment/ResourceStore.h"
#include "entities/Entity.h"
#include "entity-placement.hpp"
#include "entity-pool.hpp"
#include "new-terrain.hpp"

class IEntityCreator {
public:
    virtual Entity* create(EntityPlacement entPlacement) = 0;
};

class EntityCreatorSonic1 : public IEntityCreator {
public:
    EntityCreatorSonic1(EntityPool& entityPool, terrain::Terrain& terrain, ResourceStore& store) 
        : m_entityList(entityPool)
        , m_terrain(terrain)
        , store_(store)
    {} 

    Entity* create(EntityPlacement entPlacement);

private:
    EntityPool& m_entityList;
    terrain::Terrain& m_terrain;

    ResourceStore& store_;

    Entity* createEnemies(EntityPlacement eplc);
    Entity* createGeneral(EntityPlacement eplc);
    Entity* createOther(EntityPlacement eplc);
};