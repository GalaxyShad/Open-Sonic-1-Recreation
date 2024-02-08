#pragma once
#include "../Entity.h"

class Sfx : public Entity {
    public:
        Sfx(v2f _pos, AnimMgr _anim) : Entity(_pos) {anim = _anim;}
        void update();
};