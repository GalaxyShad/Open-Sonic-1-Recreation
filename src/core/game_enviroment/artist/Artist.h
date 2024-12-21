#ifndef OS1R_ARTIST_H
#define OS1R_ARTIST_H

#include "ArtistStructs.h"
#include "SpriteFont.h"
#include <optional>

namespace artist_api {

enum class HorizontalAlign { LEFT, CENTER, RIGHT };
enum class VerticalAlign { TOP, CENTER, BOTTOM };

struct Artist {
    struct TransformProps {
        float angle;

        bool flipHorizontal;
        bool flipVertical;

        Vector2D<float> scale;

        std::optional<Color> blending;
    };

    virtual void setBackgroundColor(Color color) = 0;

    virtual void drawTextureRect(const Texture &texture, Rect rect,
                                 Vector2D<float> pos,
                                 TransformProps transform = {}) = 0;

    virtual void drawSprite(const Sprite &sprite, Vector2D<float> pos,
                            TransformProps transform = {}) = 0;

    struct DrawTextProps {
        HorizontalAlign horizontalAlign;
    };

    virtual void drawText(const std::string &text, Vector2D<float> pos,
                          const SpriteFont &font, DrawTextProps props = {}) = 0;

    struct DrawRectangleProps {
        std::optional<Color> fillColor;
        std::optional<Color> borderColor;
        float borderThickness;
    };

    virtual void drawRectangleRadius(Vector2D<float> radius,
                                     Vector2D<float> pos,
                                     DrawRectangleProps props = {}) = 0;

    virtual ~Artist() = default;
};

} // namespace artist_api

#endif // OS1R_ARTIST_H
