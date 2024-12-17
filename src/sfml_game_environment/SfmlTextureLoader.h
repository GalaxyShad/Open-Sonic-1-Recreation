#ifndef OS1R_SFMLTEXTURELOADER_H
#define OS1R_SFMLTEXTURELOADER_H

#include "SfmlArtist.h"
#include "core/Screen.h"
#include "core/game_enviroment/artist/TextureLoader.h"

class SfmlTextureLoader : public resource_store::TextureLoader {
public:
    std::unique_ptr<artist_api::Texture>
    loadFromFile(const std::string &path) override;
    std::unique_ptr<artist_api::Texture>
    loadFromPixelBuffer(const std::vector<uint8_t> &pixelBuffer,
                        size_t width) override;
};

#endif // OS1R_SFMLTEXTURELOADER_H
