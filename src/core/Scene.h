#ifndef OS1R_SCENE_H
#define OS1R_SCENE_H

#include "Artist.h"

struct Scene {
    virtual void onStart() = 0;
    virtual void onUpdate() = 0;
    virtual void onDraw(artist_api::Artist& artist) = 0;
    virtual void onExit() = 0;

    virtual ~Scene() = default;
};

#endif // OS1R_SCENE_H
