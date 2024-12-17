#include "Screen.h"
#include "SfmlTextureLoader.h"

#include <iostream>
#include <math.h>

void Screen::init(Size size, int frameLock) {
}

void Screen::clear() {
        artist_.renderClear();
}

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
    Texture* tex = new Texture;

    tex->frames = nullptr;
    tex->framesLen = 0;
    tex->size = Size(0,0);

    if (frames) {
        tex->frames = new Frame[framesLen];

        tex->framesLen = framesLen;
        memcpy(tex->frames, frames, tex->framesLen*sizeof(Frame));
    }

    textures[key] = tex;

    SfmlTextureLoader loader;
    sfTextures[key] = store_.load(loader.loadFromFile(filename));
}

void Screen::addFont(uint8_t key, const char *alphabet, uint8_t interval,
                     uint8_t tex, irect startRect, uint16_t rectDivSpace,
                     const uint16_t *widths) {
    if (!alphabet)
        return;

    Font font;
    font.interval = interval;
    font.startRect = startRect;
    font.tex = tex;

    irect letterRect = startRect;
    size_t len = strlen(alphabet);
    for (int i = 0; i < len; i++) {
        char c = alphabet[i];

        if (widths)
            letterRect.width = widths[i];

        Letter letter;
        letter.width = letterRect.width;
        letter.xPos = letterRect.left;

        font.letters[c] = letter;

        letterRect.left += letterRect.width + rectDivSpace;
    }

    fonts[key] = font;
}

void Screen::drawText(uint8_t fontKey, const char *str, v2f pos) {
    if (!fonts.count(fontKey))
        return;

    Font font = fonts[fontKey];
    irect letterRect = font.startRect;

    auto resourceId = sfTextures[font.tex];
    auto &texFont = store_.get<artist_api::Texture>(resourceId);

    v2f letterPos = pos;

    size_t len = strlen(str);
    for (int i = 0; i < len; i++) {
        char c = str[i];

        Letter letter = font.letters[str[0]];
        if (font.letters.count(c)) {
            letter = font.letters[c];

            letterRect.left = letter.xPos;
            letterRect.width = letter.width;
        }

        if (c == '\n') {
            letterPos.x = pos.x;
            letterPos.y += letterRect.height + font.interval;
            continue;
        }

        if (c != ' ') {
            artist_api::Sprite spr = {
                .texture = texFont,
                .rect = {
                    (float)letterRect.left,
                    (float)letterRect.top,
                    (float)letterRect.width,
                    (float)letterRect.height
                }
            };

            artist_.drawSprite(spr, {.x = letterPos.x, .y = letterPos.y });
        } else {
            letterRect = font.startRect;
        }

        letterPos.x += letterRect.width + font.interval;
    }
}

uint16_t Screen::getTextWidth(uint8_t fontKey, const char *str) {
    if (!fonts.count(fontKey))
        return 0;

    Font font = fonts[fontKey];
    irect letterRect = font.startRect;
    int x = 0;

    size_t len = strlen(str);
    for (int i = 0; i < len; i++) {
        char c = str[i];

        Letter letter = font.letters[str[0]];
        if (font.letters.count(c)) {
            letter = font.letters[c];

            letterRect.left = letter.xPos;
            letterRect.width = letter.width;
        }

        if (c == '\n') {
            x = 0;
            continue;
        }

        if (c == ' ')
            letterRect = font.startRect;

        x += letterRect.width + font.interval;
    }

    return x;
}

