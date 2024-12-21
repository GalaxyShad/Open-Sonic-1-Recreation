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

#include "sonic/SonicResources.h"

class Audio {
public:
    explicit Audio(dj::Dj &dj, ResourceStore &store)
        : dj_(dj), store_(store) {}

    ResourceStore& store() {
        return store_;
    }

    dj::Dj& dj() {
        return dj_;
    }

private:
    dj::Dj& dj_;
    ResourceStore& store_;
};