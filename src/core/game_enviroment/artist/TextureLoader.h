#ifndef OS1R_TEXTURELOADER_H
#define OS1R_TEXTURELOADER_H

#include <string>
#include <vector>

#include "Artist.h"

namespace resource_store {

struct TextureLoader {
    virtual std::unique_ptr<artist_api::Texture> loadFromFile(const std::string &path) = 0;
    virtual std::unique_ptr<artist_api::Texture> loadFromPixelBuffer(const std::vector<uint8_t>& pixelBuffer, size_t width) = 0;

    virtual ~TextureLoader() = default;
};

} // namespace resource_store

#endif // OS1R_TEXTURELOADER_H
