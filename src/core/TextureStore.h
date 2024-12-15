#ifndef OS1R_TEXTURESTORE_H
#define OS1R_TEXTURESTORE_H

#include <string>
#include <vector>

#include "Artist.h"

namespace resource_store {

using UniqueID = unsigned long;

struct TextureStore {
    virtual UniqueID loadFromFile(const std::string &path) = 0;
    virtual UniqueID loadFromPixelBuffer(const std::vector<uint8_t>& pixelBuffer, size_t width) = 0;

    virtual const artist_api::Texture &get(UniqueID id) = 0;

    virtual ~TextureStore() = default;
};

} // namespace resource_store

#endif // OS1R_TEXTURESTORE_H
