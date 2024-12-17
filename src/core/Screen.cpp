#include "Screen.h"
#include "SfmlTextureLoader.h"

#include <iostream>
#include <math.h>

void Screen::init(Size size, int frameLock) {}

void Screen::clear() { artist_.renderClear(); }

void Screen::render() { artist_.render(); }

void Screen::drawTextureRect(uint8_t texId, irect texRect, v2f pos, v2i offset,
                             float angle, bool horFlip, bool verFlip) {

    auto resourceId = sfTextures[texId];

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
    Texture *tex = new Texture;

    tex->frames = nullptr;
    tex->framesLen = 0;
    tex->size = Size(0, 0);

    if (frames) {
        tex->frames = new Frame[framesLen];

        tex->framesLen = framesLen;
        memcpy(tex->frames, frames, tex->framesLen * sizeof(Frame));
    }

    textures[key] = tex;

    SfmlTextureLoader loader;
    sfTextures[key] = store_.load(loader.loadFromFile(filename));
}

void Screen::addFont(uint8_t key, const char *alphabet, uint8_t interval,
                     uint8_t tex, irect startRect, uint16_t rectDivSpace,
                     const uint16_t *widths) {

    auto textureResourceId = sfTextures[tex];

    auto &texture = store_.get<artist_api::Texture>(textureResourceId);

    artist_api::Rect rect = {
        .x = (float)startRect.left,
        .y = (float)startRect.top,
        .width = (float)startRect.width,
        .height = (float)startRect.height,

    };

    auto *font =
        new artist_api::SpriteFont(artist_api::SpriteFont::fromSpriteRow(
            texture, rect, {.alphabet = alphabet, .widthList = {}}));

    fonts[key] = store_.load(std::unique_ptr<artist_api::SpriteFont>(font));
}

void Screen::drawText(uint8_t fontKey, const char *str, v2f pos) {
    if (!fonts.count(fontKey))
        return;

    auto fontResourceId = fonts[fontKey];

    auto &font = store_.get<artist_api::SpriteFont>(fontResourceId);

    artist_.drawText(str, {.x = pos.x, .y = pos.y}, font);
}

uint16_t Screen::getTextWidth(uint8_t fontKey, const char *str) {}
