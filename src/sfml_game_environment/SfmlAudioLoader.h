#ifndef OS1R_SFMLAUDIOLOADER_H
#define OS1R_SFMLAUDIOLOADER_H

#include "core/game_enviroment/dj/AudioLoader.h"

class SfmlAudioLoader : public dj::AudioLoader {
public:
    std::unique_ptr<dj::Sound> loadSound(const std::string &soundFilePath) override;
    std::unique_ptr<dj::Music> loadMusic(const std::string &soundFilePath) override;
};

#endif // OS1R_SFMLAUDIOLOADER_H
