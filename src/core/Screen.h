#pragma once

#include <cstdint>
#include <cstring>
#include <map>

#include "Geometry.h"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Vector2.hpp"
#include "core/game_enviroment/artist/Artist.h"
#include "core/game_enviroment/artist/ArtistStructs.h"
#include "core/game_enviroment/artist/TextureLoader.h"
#include <SFML/Graphics.hpp>
#include <vector>

struct Frame {
    Frame() {}
    Frame(irect _rect)
        : rect(_rect), offset(v2i(_rect.width / 2, _rect.height / 2)) {}
    Frame(irect _rect, v2i _offset) : rect(_rect), offset(_offset) {}
    Frame(int x, int y, int width, int height)
        : rect(irect(x, y, width, height)), offset(v2i(width / 2, height / 2)) {
    }

    irect rect;
    v2i offset = v2i(0, 0);
};

struct Texture {
    Size size;
    Frame *frames;
    uint16_t framesLen;
};

#include "core/game_enviroment/ResourceStore.h"
#include "sfml_game_environment/SfmlArtist.h"

class Screen {
public:
    explicit Screen(SfmlArtist &artist, ResourceStore &store,
                    resource_store::TextureLoader &loader)
        : artist_(artist), store_(store), loader_(loader) {}

    /////////////////
    /////////////////

    Size getSize() { return Size(427, 240); }

    void drawTextureRect(uint8_t texId, irect texRect, v2f pos = {0, 0},
                         v2i offset = {0, 0}, float angle = 0.0,
                         bool horFlip = false, bool verFlip = false);

    const Texture *getTexture(uint8_t texture) { return &textures_[texture]; }

    /////////////////
    /////////////////

    inline const std::vector<Frame> &frames(uint8_t key) { return textureFrames_[key];
    }

    void bindTexture(uint8_t key, ResourceID resId);
    void bindTextureFrames(uint8_t key, const Frame *frames, size_t framesLen);

    ResourceID &getTextureResource(uint8_t key) { return sfTextures_[key]; }
    ResourceStore &store() { return store_; }
    artist_api::Artist &artist() { return artist_; }
    resource_store::TextureLoader &textureLoader() { return loader_; }

private:
    std::map<uint8_t, Texture> textures_;
    std::map<uint8_t, std::vector<Frame>> textureFrames_;
    std::map<uint8_t, ResourceID> sfTextures_;

    SfmlArtist &artist_;
    resource_store::TextureLoader &loader_;
    ResourceStore &store_;
};
