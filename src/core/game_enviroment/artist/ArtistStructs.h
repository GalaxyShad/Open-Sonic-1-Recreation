#ifndef OS1R_ARTISTSTRUCTS_H
#define OS1R_ARTISTSTRUCTS_H

#include "core/game_enviroment/IStorableResource.h"
#include <cstddef>
#include <vector>

using u8 = unsigned char;
using u32 = unsigned int;

namespace artist_api {

template <typename T> struct Vector2D {
    T x, y;
};

struct Rect {
    float x, y, width, height;
};

struct Color final {

    static Color fromARGB(u32 argb) {
        return Color{.alpha = static_cast<u8>((argb >> 24) & 0xFF),
                     .red = static_cast<u8>((argb >> 16) & 0xFF),
                     .green = static_cast<u8>((argb >> 8) & 0xFF),
                     .blue = static_cast<u8>((argb >> 0) & 0xFF)};
    }

    static Color fromARGB(u8 a, u8 r, u8 g, u8 b) {
        return Color{.alpha = a, .red = r, .green = g, .blue = b};
    }

    static Color fromRGB(u32 rgb) {
        return Color{.alpha = 0xFF,
                     .red = static_cast<u8>((rgb >> 16) & 0xFF),
                     .green = static_cast<u8>((rgb >> 8) & 0xFF),
                     .blue = static_cast<u8>((rgb >> 0) & 0xFF)};
    }

    static Color fromRGB(u8 r, u8 g, u8 b) {
        return Color{.alpha = 0xFF, .red = r, .green = g, .blue = b};
    }

    static Color BLACK() {
        return Color::fromRGB(0);
    }

    static Color RED() {
        return Color::fromRGB(0xFF0000);
    }

    static Color GREEN() {
        return Color::fromRGB(0x00FF00);
    }

    static Color BLUE() {
        return Color::fromRGB(0x0000FF);
    }

    static Color WHITE() {
        return Color::fromRGB(0xFFFFFF);
    }

    inline u32 argb() {
        return (alpha << 24) | (red << 16) | (green << 8) | blue;
    }
    inline u32 rgba() {
        return (red << 24) | (green << 16) | (blue << 8) | alpha;
    }

    u8 alpha, red, green, blue;
};

struct ImageBitmap {
    enum class PixelFormat {
        ARGB,
    };

    std::vector<u8> buffer_;
    Vector2D<size_t> size_;
    PixelFormat pixelFormat_ = PixelFormat::ARGB;
};

class Image final {
public:
    enum class PixelFormat {
        ARGB,
    };
public:
    Image(Vector2D<size_t> size) { 
        bitmap_.size_.x = size.x,
        bitmap_.size_.y = size.y,
        bitmap_.buffer_.resize(size.x * size.y * 4); 
    }

    void setPixel(int x, int y, Color color) {
        auto pos = (y * bitmap_.size_.x + x) * 4;

        bitmap_.buffer_[pos + 3] = color.alpha;
        bitmap_.buffer_[pos + 0] = color.red;
        bitmap_.buffer_[pos + 1] = color.green;
        bitmap_.buffer_[pos + 2] = color.blue;
    }

    Color getPixel(int x, int y) {
        auto pos = (y * bitmap_.size_.x + x) * 4;

        Color px = {.alpha = bitmap_.buffer_[pos + 3],
                    .red = bitmap_.buffer_[pos + 0],
                    .green = bitmap_.buffer_[pos + 1],
                    .blue = bitmap_.buffer_[pos + 2]};

        return px;
    }

    const ImageBitmap& bitmap() { return bitmap_; }

private:
    ImageBitmap bitmap_;
};

struct Texture : public IStorableResource {
    ~Texture() override = default;
};

struct Sprite {
    const Texture &texture;
    Rect rect;
    Vector2D<float> offset;

    static Sprite withCenterOffset(const Texture& tex, Rect rect) {
        Sprite spr = {
            .texture = tex,
            .rect = rect
        };

        spr.setOffsetCenter();

        return spr;
    }

    void setOffsetCenter() {
        offset = {
            .x = rect.width / 2,
            .y = rect.height / 2,
        };
    }
};

struct StorableSprite : public IStorableResource, public Sprite {

};

struct Animation {
    std::vector<Sprite> frames;
};


} // namespace artist_api

#endif // OS1R_ARTISTSTRUCTS_H
