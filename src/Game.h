#pragma once

#include "core/_index.hpp"

#include "Level.h"
#include "TitleScreen.h"
#include "level-loader.hpp"
#include "core/SfmlGameEnvironment.h"

#include "entities/TitleMenuEntity.h"

class IGame {
  public:
    virtual void init() = 0;
    virtual void update() = 0;
    virtual bool isRunning() = 0;
};

class Game : public IGame {
  public:
    Game(Screen &screen, TitleScreen &ts, SfmlGameEnvironment &env)
        : scr(screen), titleMenuEntity_(ts), env_(env) {}
    void init();
    void update();
    void draw(artist_api::Artist& artist);
    bool isRunning();

  private:
    Audio audio;
    Screen &scr;
    SfmlGameEnvironment& env_;

    TitleMenuEntity titleMenuEntity_;
};