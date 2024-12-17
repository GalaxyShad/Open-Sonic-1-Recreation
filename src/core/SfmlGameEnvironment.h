#ifndef OS1R_SFMLGAMEENVIRONMENT_H
#define OS1R_SFMLGAMEENVIRONMENT_H

#include <SFML/Graphics.hpp>

#include "GameEnvironment.h"
#include "SfmlArtist.h"
#include "SfmlTextureStore.h"
#include "InputMgr.h"

class SfmlGameEnvironment : public GameEnvironment {
  private:
    static constexpr int WINDOW_WIDTH = 426;
    static constexpr int WINDOW_HEIGHT = 240;
    static constexpr char WINDOW_TITLE[] = "OS1R / SFML";

  public:
    SfmlGameEnvironment() : artist_(renderWindow_) {}

    void init() override;
    void beginFrame() override;
    void endFrame() override;

    inline bool isRunning() override { return renderWindow_.isOpen(); }

    inline SfmlArtist& artist() { return artist_; }
    inline SfmlTextureStore& textureStore() { return textureStore_; }
    inline SfmlInput& input() { return input_; }

  private:
    sf::RenderWindow renderWindow_;
    SfmlArtist artist_;
    SfmlTextureStore textureStore_;
    SfmlInput input_;

    enum class WindowScale {
        X1 = 1,
        X2,
        X3,
        FULL_SCREEN
    };

    int currentWindowScale_ = 1;

  private:
    void handleEvent(sf::Event event);
    void handleKeyPressed(sf::Event::KeyEvent keyEvent);

    void scaleWindow(WindowScale scale);
};

#endif // OS1R_SFMLGAMEENVIRONMENT_H
