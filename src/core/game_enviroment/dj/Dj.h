#ifndef OS1R_DJ_H
#define OS1R_DJ_H

#include "DjResources.h"

namespace dj {

struct Dj {
    virtual void playSound(Sound& snd) = 0;
    virtual void stopSound(Sound& snd) = 0;

    virtual void playMusic(Music& mus) = 0;
    virtual void stopMusic(Music& mus) = 0;

    virtual int masterVolume() = 0;
    virtual void setMasterVolume(int percent) = 0;

    virtual int soundVolume() = 0;
    virtual void setSoundVolume(int percent) = 0;

    virtual int musicVolume() = 0;
    virtual void setMusicVolume(int percent) = 0;

    virtual ~Dj() = default;
};

}

#endif // OS1R_DJ_H
