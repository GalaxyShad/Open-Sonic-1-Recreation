#pragma once

#include "../Camera.h"
#include "./sonic-1/object-ids-s1.hpp"

enum class ObjectID_General {
    None,
    Player = 1,
    Ring,
    Spring,
};

struct EntityId {
    ObjectID_General id;

    ObjectID_S1 idSonic1 = ObjectID_S1::NONE;
    // ObjectID_S2 idSonic2;
    // ObjectID_S3 idSonic3;
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

