#include "SfmlTextureLoader.h"

#include <format>

#include "SFML/Graphics.hpp"

std::unique_ptr<artist_api::Texture>
SfmlTextureLoader::loadFromFile(const std::string &path) {
    sf::Texture tex;

    auto isSuccessful = tex.loadFromFile(path);

    if (!isSuccessful) {
        throw std::invalid_argument(std::format("Texture with path {} failed to load", path));
    }

    return std::unique_ptr<artist_api::Texture>(new SfmlTexture(tex));
}


std::unique_ptr<artist_api::Texture>
SfmlTextureLoader::loadFromPixelBuffer(const std::vector<uint8_t> &pixelBuffer, size_t width) {
    constexpr int RGBA_WORD_SIZE = 4;

    auto height = (pixelBuffer.size() / RGBA_WORD_SIZE) / width;

    sf::Texture tex;
    tex.create(width, height);
    tex.update(pixelBuffer.data());

    return std::unique_ptr<artist_api::Texture>(new SfmlTexture(tex));
}
