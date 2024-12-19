#ifndef OS1R_SFMLDJ_H
#define OS1R_SFMLDJ_H

#include <SFML/Audio.hpp>
#include "core/game_enviroment/dj/Dj.h"
#include <list>

struct SfmlSound : public dj::Sound {
    sf::SoundBuffer buffer;
};

struct SfmlMusic : public dj::Music {
    sf::Music music;
};

class SfmlDj : public dj::Dj {
public:
    void playSound(dj::Sound &snd) override;
    void stopSound(dj::Sound &snd) override;
    void stopMusic(dj::Music &mus) override;
    void playMusic(dj::Music &mus) override;

    int masterVolume() override { return masterVolume_; }
    void setMasterVolume(int percent) override { masterVolume_ = percent; }

    int musicVolume() override { return musicVolume_; }
    void setMusicVolume(int percent) override { musicVolume_ = percent; }

    int soundVolume() override { return soundVolume_; }
    void setSoundVolume(int percent) override { soundVolume_ = percent; }

    void removeStoppedSounds();

private:
    std::list<sf::Sound> sndList_;
    std::list<sf::Music> musList_;

    int masterVolume_ = 20;
    int soundVolume_ = 100;
    int musicVolume_ = 100;
};

#endif // OS1R_SFMLDJ_H
