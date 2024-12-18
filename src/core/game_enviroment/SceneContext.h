#ifndef OS1R_SCENECONTEXT_H
#define OS1R_SCENECONTEXT_H

#include "core/game_enviroment/artist/Artist.h"
#include "core/game_enviroment/InputMgr.h"
#include "ISceneDirector.h"

struct SceneStartContext {

};

struct SceneUpdateContext {
    IInputMgr& input;
    ISceneDirector& sceneDirector;
};

struct SceneDrawContext {
    artist_api::Artist& artist;
};

struct SceneExitContext {

};

#endif // OS1R_SCENECONTEXT_H
