#include "Screen.h"
#include "sfml_game_environment/SfmlTextureLoader.h"

#include <iostream>
#include <math.h>

void Screen::init(Size size, int frameLock) {}

void Screen::clear() { artist_.renderClear(); }

void Screen::render() { artist_.render(); }

void Screen::drawTextureRect(uint8_t texId, irect texRect, v2f pos, v2i offset,
                             float angle, bool horFlip, bool verFlip) {

    auto resourceId = sfTextures_[texId];

    auto &tex = store_.get<artist_api::Texture>(resourceId);

    artist_.drawSprite({.texture = tex,
                        .rect = {.x = (float)texRect.left,
                                 .y = (float)texRect.top,
                                 .width = (float)texRect.width,
                                 .height = (float)texRect.height},
                        .offset = {.x = (float)offset.x, .y = (float)offset.y}},
                       artist_api::Vector2D<float>{.x = pos.x, .y = pos.y},
                       artist_api::Artist::TransformProps{
                           .angle = angle,
                           .flipHorizontal = horFlip,
                           .flipVertical = verFlip,
                       });
}

void Screen::drawTextureRect(uint8_t texId, Frame frame, v2f pos, float angle,
                             bool horFlip, bool verFlip) {

    drawTextureRect(texId, frame.rect, pos, frame.offset, angle, horFlip,
                    verFlip);
}

void Screen::loadTextureFromFile(const char *filename, uint8_t key,
                                 const Frame *frames, uint16_t framesLen,
                                 uint16_t width, uint16_t height) {

    SfmlTextureLoader loader;

    bindTextureFrames(key, frames, framesLen);
    bindTexture(key, store_.load(loader.loadFromFile(filename)));
}

void Screen::addFont(uint8_t key, const char *alphabet, uint8_t interval,
                     uint8_t tex, irect startRect, uint16_t rectDivSpace,
                     const uint16_t *widths) {

    auto textureResourceId = sfTextures_[tex];

    auto &texture = store_.get<artist_api::Texture>(textureResourceId);

    artist_api::Rect rect = {
        .x = (float)startRect.left,
        .y = (float)startRect.top,
        .width = (float)startRect.width,
        .height = (float)startRect.height,

    };

    auto *font =
        new artist_api::SpriteFont(artist_api::SpriteFont::fromSpriteRow(
            artist_api::Sprite{.texture = texture, .rect = rect}, alphabet));

    bindFont(key, store_.load(std::unique_ptr<artist_api::SpriteFont>(font)));
}

void Screen::drawText(uint8_t fontKey, const char *str, v2f pos) {
    if (!fonts_.count(fontKey))
        return;

    auto fontResourceId = fonts_[fontKey];

    auto &font = store_.get<artist_api::SpriteFont>(fontResourceId);

    artist_.drawText(str, {.x = pos.x, .y = pos.y}, font);
}

uint16_t Screen::getTextWidth(uint8_t fontKey, const char *str) {}

void Screen::bindTexture(uint8_t key, ResourceID resId) {
    sfTextures_[key] = resId;
}

void Screen::bindFont(uint8_t key, ResourceID resId) { fonts_[key] = resId; }

void Screen::bindTextureFrames(uint8_t key, const Frame *frames,
                               size_t framesLen) {
    textureFrames_[key] = std::vector<Frame>(frames, frames + framesLen);
    textures_[key] = {
        .frames = textureFrames_[key].data(),
        .framesLen = (uint16_t)framesLen,
    };
}
