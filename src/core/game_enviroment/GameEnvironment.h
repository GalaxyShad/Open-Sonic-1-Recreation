#ifndef OS1R_GAMEENVIRONMENT_H
#define OS1R_GAMEENVIRONMENT_H

#include "artist/Artist.h"
#include "artist/TextureLoader.h"

#include "dj/Dj.h"
#include "dj/AudioLoader.h"

#include "InputMgr.h"
#include "ResourceStore.h"

struct GameEnvironment {
    virtual void init() = 0;

    virtual void beginFrame() = 0;
    virtual void endFrame() = 0;

    virtual bool isRunning() = 0;

    virtual void exitGame() = 0;

    virtual artist_api::Artist& artist() = 0;
    virtual dj::Dj& dj() = 0;
    virtual IInputMgr& input() = 0;

    virtual resource_store::TextureLoader& textureLoader() = 0;
    virtual dj::AudioLoader& audioLoader() = 0;

    virtual ~GameEnvironment() = default;
};

#endif // OS1R_GAMEENVIRONMENT_H
