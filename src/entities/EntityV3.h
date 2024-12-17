#ifndef OS1R_ENTITYV3_H
#define OS1R_ENTITYV3_H

#include <vector>

#include "EntityContext.h"

namespace entity_v3 {

using TagID = unsigned long;
using EntityID = unsigned long;
using ComponentID = unsigned long;

constexpr bool NO_TAGS = false;

struct Component {
    virtual ComponentID type() = 0;

    virtual ~Component() = default;
};

static std::vector<Component> NO_COMPONENTS;

struct Entity {
    virtual void onInit(const InitContext &ctx) {}
    virtual void onUpdate(const UpdateContext &ctx) {}
    virtual void onDraw(const DrawContext &ctx) {}

    virtual bool hasTag(TagID tag) = 0;
    virtual const std::vector<Component>& components() = 0;

    virtual EntityID type() = 0;

    virtual ~Entity() = default;
};

} // namespace entity_v3

#endif // OS1R_ENTITYV3_H
