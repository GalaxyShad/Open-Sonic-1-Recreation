#ifndef OS1R_ENTITYCONTEXT_H
#define OS1R_ENTITYCONTEXT_H

#include "core/Artist.h"
#include "core/Screen.h"
#include "core/InputMgr.h"

namespace entity_v3 {

class InitContext {};

struct DrawContext final {
    artist_api::Artist &artist;
    Screen &deprecatedScreen;
};

struct UpdateContext final {
    IInputMgr& input;
};

} // namespace entity_v3

#endif // OS1R_ENTITYCONTEXT_H
