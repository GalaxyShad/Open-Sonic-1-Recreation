#ifndef OS1R_ISCENEDIRECTOR_H
#define OS1R_ISCENEDIRECTOR_H

using SceneUniqueID = long;

struct ISceneDirector {
    virtual void go(SceneUniqueID sceneId) = 0;

    virtual ~ISceneDirector() = default;
};

#endif // OS1R_ISCENEDIRECTOR_H
