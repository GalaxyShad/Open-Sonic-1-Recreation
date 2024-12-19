#pragma once

#include <iterator>
#include <list>
#include <map>
#include <string>

#include <cstdint>
#include <cstring>

#include "AudioMappings.h"

#include "SFML/Audio/SoundBuffer.hpp"
#include "core/game_enviroment/ResourceStore.h"
#include "core/game_enviroment/dj/AudioLoader.h"
#include "core/game_enviroment/dj/Dj.h"

class Audio {
public:
    explicit Audio(dj::Dj &dj, dj::AudioLoader &audioLoader,
                   ResourceStore &store)
        : dj_(dj), audioLoader_(audioLoader), store_(store) {}

    int loadSound(const char *file, uint8_t key) {
        auto snd = audioLoader_.loadSound(file);
        resMap_[key] = store_.load(std::move(snd));

        return 1;
    }

    int addMusic(uint8_t key, std::string file) {
        auto mus = audioLoader_.loadMusic(file);
        resMap_[128 + key] = store_.load(std::move(mus));

        return 1;
    }

    void playSound(uint8_t key) {
        auto& sndResource = store_.get<dj::Sound>(resMap_[key]);

        dj_.playSound(sndResource);
    }

    void stopSound(uint8_t key) {
        // if (!soundsBuff.count(key))
        //     return;

        // sf::Sound s(*soundsBuff[key]);
        // snd.stop();
    }

    void playMusic(uint8_t key) {
        auto& musResource = store_.get<dj::Music>(resMap_[128 + key]);

        dj_.playMusic(musResource);
    }

    void stopMusic() {

    }

    void update() {

    }

    void free() {

    }

private:
    dj::Dj& dj_;
    dj::AudioLoader& audioLoader_;
    ResourceStore& store_;
    std::map<uint8_t, ResourceID> resMap_;

};