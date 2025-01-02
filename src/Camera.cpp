#include "Camera.h"
#include "core/Screen.h"
#include "core/game_enviroment/artist/ArtistStructs.h"

#include <string>

void Camera::create(v2f _pos, Size _levelSize, bool free) {
    levelSize = _levelSize;
    bottomBorder = levelSize.height * 256;
    rightBorder = levelSize.width * 256;
    pos = _pos;
    size = Size(427, 240);
    m_free = free;
}

void Camera::update() {
    if (m_free)
        return;

    if (pos.x < 0.0)
        pos.x = 0.0;
    if (pos.y < 0.0)
        pos.y = 0.0;
    if (pos.x + size.width > levelSize.width * 256)
        pos.x = levelSize.width * 256 - size.width;
    if (pos.y + size.height > levelSize.height * 256)
        pos.y = levelSize.height * 256 - size.height;

    if (pos.x + size.width > rightBorder)
        pos.x = rightBorder - size.width;
    if (pos.y + size.height > bottomBorder)
        pos.y = bottomBorder - size.height;
}

void Camera::draw(uint8_t tex, irect texRect, v2f _pos, v2i offset, float angle,
                  bool horFlip, bool verFlip) {
    
    auto &texRes = scr.getTextureResource(tex);
    auto &t = scr.store().get<artist_api::Texture>(texRes);

    scr.artist().drawSprite({.texture = t,
                        .rect = {.x = (float)texRect.left,
                                 .y = (float)texRect.top,
                                 .width = (float)texRect.width,
                                 .height = (float)texRect.height},
                        .offset = {.x = (float)offset.x, .y = (float)offset.y}},
                       artist_api::Vector2D<float>{.x = _pos.x - pos.x, .y = _pos.y - pos.y},
                       artist_api::Artist::TransformProps{
                           .angle = angle,
                           .flipHorizontal = horFlip,
                           .flipVertical = verFlip,
                       });    
}

void Camera::draw(uint8_t tex, Frame frame, v2f _pos, float angle, bool horFlip,
                  bool verFlip) {
    auto &texRes = scr.getTextureResource(tex);
    auto &t = scr.store().get<artist_api::Texture>(texRes);

    scr.artist().drawSprite(
        artist_api::Sprite{
            .texture = t,
            .rect = {.x = static_cast<float>(frame.rect.left),
                     .y = static_cast<float>(frame.rect.top),
                     .width = static_cast<float>(frame.rect.width),
                     .height = static_cast<float>(frame.rect.height)},
            .offset = {.x = static_cast<float>(frame.offset.x),
                       .y = static_cast<float>(frame.offset.y)}},
        artist_api::Vector2D<float>{.x = _pos.x - pos.x, .y = _pos.y - pos.y},
        {.angle = angle, .flipHorizontal = horFlip, .flipVertical = verFlip});
}

void Camera::draw(const AnimMgr &anim, v2f _pos, float angle, bool horFlip,
                  bool verFlip) {

    uint16_t aFrame = (uint16_t)anim.getCurFrame();
    uint8_t aTextureID = anim.getTexture();

    auto &texFrames = scr.frames(aTextureID);

    bool flip = horFlip;
    if (anim.getReversive())
        flip = anim.getFlip();

    if (texFrames.size()) {

        auto &texRes = scr.getTextureResource(aTextureID);
        auto &tex = scr.store().get<artist_api::Texture>(texRes);
        auto &frame = scr.frames(aTextureID)[aFrame];

        scr.artist().drawSprite(
            artist_api::Sprite{
                .texture = tex,
                .rect = {.x = static_cast<float>(frame.rect.left),
                         .y = static_cast<float>(frame.rect.top),
                         .width = static_cast<float>(frame.rect.width),
                         .height = static_cast<float>(frame.rect.height)},
                .offset = {.x = static_cast<float>(frame.offset.x),
                           .y = static_cast<float>(frame.offset.y)}},
            artist_api::Vector2D<float>{.x = _pos.x - pos.x,
                                        .y = _pos.y - pos.y},
            {.angle = angle, .flipHorizontal = flip, .flipVertical = verFlip});
    }
}