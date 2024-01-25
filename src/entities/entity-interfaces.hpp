#pragma once

#include "../Camera.h"
#include "./sonic-1/object-ids-s1.hpp"

enum class ObjectID_General {
    Player = 1,
    Ring,
    Spring,
};

struct EntityId {
    ObjectID_S1 sonic1Id = ObjectID_S1::NONE;
    // ObjectID_S2 sonic2Id;
    // ObjectID_S3 sonic3Id;

    ObjectID_General id;
};

class IEntityUpdatable {
public:
    virtual void update() = 0;
};

class IEntityDrawable {
public:
    virtual void draw(Camera& camera) = 0;
    virtual int getDrawPriority() = 0;
};

class IEntity {
public:
    virtual EntityId id() = 0;
};

