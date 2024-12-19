#include "SfmlAudioLoader.h"

#include "SfmlDj.h"

std::unique_ptr<dj::Sound>
SfmlAudioLoader::loadSound(const std::string &soundFilePath) {
    auto res = std::make_unique<SfmlSound>();

    res->buffer.loadFromFile(soundFilePath);

    return std::move(res);
}

std::unique_ptr<dj::Music>
SfmlAudioLoader::loadMusic(const std::string &soundFilePath) {
    auto res = std::make_unique<SfmlMusic>();

    res->music.openFromFile(soundFilePath);

    return std::move(res);
}
