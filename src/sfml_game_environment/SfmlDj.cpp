#include "SfmlDj.h"

void SfmlDj::playSound(dj::Sound &snd) {
    auto& sfmlSound = (SfmlSound&)snd;

    auto& a = sndList_.emplace_back(sfmlSound.buffer);

    float vol = (float)masterVolume_ / 100.f;

    a.setVolume(vol);
    a.play();
}

void SfmlDj::stopSound(dj::Sound &snd) {}

void SfmlDj::playMusic(dj::Music &mus) {
    auto& sfmlMusic = (SfmlMusic&)mus;

    float vol = (float)masterVolume_ / 100.f;

    sfmlMusic.music.setVolume(vol);
    sfmlMusic.music.play();
}

void SfmlDj::stopMusic(dj::Music &mus) {
    auto& sfmlMusic = (SfmlMusic&)mus;

    sfmlMusic.music.stop();
}

void SfmlDj::removeStoppedSounds() {
    sndList_.remove_if([&](sf::Sound &item) {
        return item.getStatus() == sf::SoundSource::Stopped;
    });
}