#ifndef OS1R_GAMEENVIRONMENT_H
#define OS1R_GAMEENVIRONMENT_H

struct GameEnvironment {
    virtual void init() = 0;

    virtual void beginFrame() = 0;
    virtual void endFrame() = 0;

    virtual bool isRunning() = 0;

    virtual void exitGame() = 0;

    virtual ~GameEnvironment() = default;
};

#endif // OS1R_GAMEENVIRONMENT_H
