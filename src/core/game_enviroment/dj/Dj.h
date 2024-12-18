#ifndef OS1R_DJ_H
#define OS1R_DJ_H

#include "DjResources.h"

namespace dj {

struct Dj {
    virtual void playSound(Sound& snd) = 0;
    virtual void stopSound(Sound& snd) = 0;

    virtual void playMusic(Music& mus) = 0;
    virtual void stopMusic(Music& mus) = 0;

    virtual ~Dj() = default;
};

}

#endif // OS1R_DJ_H
