#ifndef OS1R_ARTIST_H
#define OS1R_ARTIST_H

namespace artist_api {

struct Texture {
    virtual ~Texture() = default;
};

template <typename T>
struct Vector2D {
    T x, y;
};

struct Rect {
    float x, y, width, height;
};

struct Sprite {
    const Texture& texture;
    Rect rect;
    Vector2D<float> offset;
};

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

    virtual ~Artist() = default;
};

} // namespace artist_api

#endif // OS1R_ARTIST_H
