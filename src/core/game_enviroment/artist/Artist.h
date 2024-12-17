#ifndef OS1R_ARTIST_H
#define OS1R_ARTIST_H

#include "ArtistStructs.h"
#include "SpriteFont.h"

namespace artist_api {

struct Artist {
    struct TransformProps {
        float angle;

        bool flipHorizontal;
        bool flipVertical;

        Vector2D<float> scale;
    };

    virtual void drawTextureRect(const Texture &texture, Rect rect,
                                 Vector2D<float> pos,
                                 TransformProps transform = {}) = 0;

    virtual void drawSprite(const Sprite& sprite, Vector2D<float> pos, TransformProps transform = {}) = 0;

    virtual void drawText(const std::string& text, Vector2D<float> pos, const SpriteFont& font) = 0;

    virtual ~Artist() = default;
};

} // namespace artist_api

#endif // OS1R_ARTIST_H
