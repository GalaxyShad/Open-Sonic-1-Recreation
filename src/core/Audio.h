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
        : dj_(dj), store_(store) {}

    void bindSound(uint8_t key, ResourceID res) {
        resMap_[key] = res;
    }

    void bindMusic(uint8_t key, ResourceID mus) {
        resMap_[key + 128] = mus;
    }

    void playSound(uint8_t key) {
        auto& sndResource = store_.get<dj::Sound>(resMap_[key]);

        dj_.playSound(sndResource);
    }

    void playMusic(uint8_t key) {
        auto& musResource = store_.get<dj::Music>(resMap_[128 + key]);

        dj_.playMusic(musResource);
    }

    void update() {

    }

    void free() {

    }

private:
    dj::Dj& dj_;
    ResourceStore& store_;
    std::map<uint8_t, ResourceID> resMap_;

};