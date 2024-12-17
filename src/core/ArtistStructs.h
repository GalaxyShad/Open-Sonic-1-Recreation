#ifndef OS1R_ARTISTSTRUCTS_H
#define OS1R_ARTISTSTRUCTS_H

#include "IStorableResource.h"

namespace artist_api {
struct Texture : public IStorableResource {
    ~Texture() override = default;
};

template <typename T> struct Vector2D {
    T x, y;
};

struct Rect {
    float x, y, width, height;
};

struct Sprite {
    const Texture &texture;
    Rect rect;
    Vector2D<float> offset;
};
} // namespace artist_api

#endif // OS1R_ARTISTSTRUCTS_H
