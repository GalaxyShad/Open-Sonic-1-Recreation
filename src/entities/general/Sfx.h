#pragma once
#include "../Entity.h"

class Sfx : public Entity {
    public:
        Sfx(v2f _pos, AnimMgr _anim) : Entity(_pos) {dv_anim = _anim;}
        void d_update();
};