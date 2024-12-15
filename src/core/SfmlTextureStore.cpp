#include "SfmlTextureStore.h"

#include <format>

resource_store::UniqueID
SfmlTextureStore::loadFromFile(const std::string &path) {
    sf::Texture tex;

    auto isSuccessful = tex.loadFromFile(path);

    if (!isSuccessful) {
        throw std::invalid_argument(std::format("Texture with path {} failed to load", path));
    }

    textureList_.emplace_back(tex);

    return id_++;
}

const artist_api::Texture &SfmlTextureStore::get(resource_store::UniqueID id) {
    return textureList_[id];
}

resource_store::UniqueID
SfmlTextureStore::loadFromPixelBuffer(const std::vector<uint8_t> &pixelBuffer, size_t width) {
    constexpr int RGBA_WORD_SIZE = 4;

    auto height = (pixelBuffer.size() / RGBA_WORD_SIZE) / width;

    sf::Texture tex;
    tex.create(width, height);
    tex.update(pixelBuffer.data());

    return 0;
}
