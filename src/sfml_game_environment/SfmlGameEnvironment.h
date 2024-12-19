#ifndef OS1R_SFMLGAMEENVIRONMENT_H
#define OS1R_SFMLGAMEENVIRONMENT_H

#include "SFML/Graphics.hpp"

#include "SfmlArtist.h"
#include "SfmlAudioLoader.h"
#include "SfmlDj.h"
#include "SfmlTextureLoader.h"

#include "core/game_enviroment/GameEnvironment.h"
#include "core/game_enviroment/InputMgr.h"

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

    inline void exitGame() override { renderWindow_.close(); }

    artist_api::Artist &artist() override { return artist_; }
    dj::Dj &dj() override { return dj_; }

    IInputMgr &input() override { return input_; }
    resource_store::TextureLoader &textureLoader() override {
        return textureLoader_;
    }
    dj::AudioLoader &audioLoader() override { return audioLoader_; }

    SfmlArtist &sfmlArtist() { return artist_; }

private:
    sf::RenderWindow renderWindow_;
    SfmlArtist artist_;
    SfmlTextureLoader textureLoader_;
    SfmlInput input_;
    SfmlDj dj_;
    SfmlAudioLoader audioLoader_;

    enum class WindowScale { X1 = 1, X2, X3, FULL_SCREEN };

    int currentWindowScale_ = 1;

private:
    void handleEvent(sf::Event event);
    void handleKeyPressed(sf::Event::KeyEvent keyEvent);

    void scaleWindow(WindowScale scale);
};

#endif // OS1R_SFMLGAMEENVIRONMENT_H
