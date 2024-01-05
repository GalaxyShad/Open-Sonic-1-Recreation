#include "Sfx.h"

void Sfx::update()
{
    if (anim.getCurFrame() >= anim.getLastFrame())
        living = false;
    anim.tick();
}
