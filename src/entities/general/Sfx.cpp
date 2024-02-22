#include "Sfx.h"

void Sfx::update()
{
    if (dv_anim.getCurFrame() >= dv_anim.getLastFrame())
        dv_living = false;
    dv_anim.tick();
}
