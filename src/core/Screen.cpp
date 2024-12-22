#include "Screen.h"
#include "sfml_game_environment/SfmlTextureLoader.h"

#include <iostream>
#include <math.h>


void Screen::bindTexture(uint8_t key, ResourceID resId) {
    sfTextures_[key] = resId;
}

void Screen::bindTextureFrames(uint8_t key, const Frame *frames,
                               size_t framesLen) {
    textureFrames_[key] = std::vector<Frame>(frames, frames + framesLen);
    textures_[key] = {
        .frames = textureFrames_[key].data(),
        .framesLen = (uint16_t)framesLen,
    };
}
