#ifndef OS1R_SFMLTEXTURESTORE_H
#define OS1R_SFMLTEXTURESTORE_H

#include "TextureStore.h"
#include "SfmlArtist.h"
#include <SFML/Graphics.hpp>

class SfmlTextureStore : public resource_store::TextureStore {
  public:
    resource_store::UniqueID
    loadFromFile(const std::string &path) override;
    resource_store::UniqueID loadFromPixelBuffer(const std::vector<uint8_t> &pixelBuffer, size_t width) override;

    const artist_api::Texture & get(resource_store::UniqueID id) override;

  private:
    std::vector<SfmlTexture> textureList_;
    long id_;
};

#endif // OS1R_SFMLTEXTURESTORE_H
