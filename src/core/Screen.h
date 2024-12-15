#pragma once

#include <cstdint>
#include <cstring>
#include <map>


#include "Geometry.h"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>

struct Frame {
    Frame() {}
    Frame(irect _rect)
        : rect(_rect), offset(v2i(_rect.width / 2, _rect.height / 2)) {}
    Frame(irect _rect, v2i _offset) : rect(_rect), offset(_offset) {}
    Frame(int x, int y, int width, int height)
        : rect(irect(x, y, width, height)), offset(v2i(width / 2, height / 2)) {
    }

    irect rect;
    v2i offset = v2i(0, 0);
};

struct Texture {
    uint8_t *indexes;
    Size size;
    uint8_t palRow;
    Frame *frames;
    uint16_t framesLen;
};

struct Letter {
    uint32_t xPos;
    uint16_t width;
};

struct Font {
    uint8_t tex;
    irect startRect;
    uint16_t interval;
    std::map<char, Letter> letters;
};

class IScreen {
  public:
    virtual void init(Size size, int frameLock) = 0;
    virtual void clear() = 0;
    virtual void render() = 0;

    virtual void setBgColor(uint16_t color) = 0;

    virtual Size getSize() = 0;

    virtual void drawTextureRect(uint8_t texId, irect texRect, v2f pos = {0, 0},
                                 v2i offset = {0, 0}, float angle = 0.0,
                                 bool horFlip = false,
                                 bool verFlip = false) = 0;
    virtual void drawTextureRect(uint8_t texId, Frame frame, v2f pos = {0, 0},
                                 float angle = 0.0, bool horFlip = false,
                                 bool verFlip = false) = 0;

    virtual void loadTexture(const uint8_t *src, uint16_t width,
                             uint16_t height, uint8_t palRow, uint8_t key,
                             const Frame *frames = nullptr,
                             uint16_t framesLen = 0) = 0;
};

#define PAL_MAX_ROWS 4
#define PAL_MAX_COLUMNS 16

#include "SfmlArtist.h"
#include "TextureStore.h"

class Screen : public IScreen {
  public:
    explicit Screen(SfmlArtist &artist,
                    resource_store::TextureStore &textureStore);
    void init(Size size, int frameLock);
    void clear();
    void render();

    void setBgColor(uint16_t color) {}

    Size getSize() { return Size(427, 240); }

    void drawTextureRect(uint8_t texId, irect texRect, v2f pos = {0, 0},
                         v2i offset = {0, 0}, float angle = 0.0,
                         bool horFlip = false, bool verFlip = false);
    void drawTextureRect(uint8_t texId, Frame frame, v2f pos = {0, 0},
                         float angle = 0.0, bool horFlip = false,
                         bool verFlip = false);

    const Texture *getTexture(uint8_t texture) {
        return textures.count(texture) ? textures[texture] : nullptr;
    }

    void loadTexture(const uint8_t *src, uint16_t width, uint16_t height,
                     uint8_t palRow, uint8_t key, const Frame *frames = nullptr,
                     uint16_t framesLen = 0);
    void loadTextureFromFile(const char *filename, uint8_t key,
                             const Frame *frames = nullptr,
                             uint16_t framesLen = 0, uint16_t width = 0,
                             uint16_t height = 0);

    void addFont(uint8_t key, const char *alphabet, uint8_t interval,
                 uint8_t tex, irect startRect, uint16_t rectDivSpace = 0,
                 const uint16_t *widths = nullptr);
    void drawText(uint8_t fontKey, const char *str, v2f pos = v2f(0, 0));
    uint16_t getTextWidth(uint8_t fontKey, const char *str);

    sf::RenderWindow &getSfmlWindow() { return artist_.sfmlRenderWindow(); }

    void drawRadiusRectange(v2f pos, v2i radius, uint32_t color) {
        sf::RectangleShape rs;

        rs.setPosition(pos.x, pos.y);
        rs.setOrigin(radius.x, radius.y);
        rs.setSize(sf::Vector2f(radius.x * 2, radius.y * 2));
        rs.setFillColor(sf::Color(color));

//        artist_.sfmlRenderWindow().draw(rs);
//        wnd.draw(rs);
    }

  private:
    uint16_t pal[PAL_MAX_ROWS][PAL_MAX_COLUMNS] = {
        {0x0000, 0x2280, 0x44a0, 0x66c0, 0x88e0, 0xeee0, 0xaaa0, 0x8880, 0x4440,
         0xea80, 0xa640, 0xe000, 0x8000, 0x4000, 0xee00, 0x0000},
        {0x0000, 0x2420, 0x4640, 0x6860, 0x8c80, 0xeee0, 0xaaa0, 0x8880, 0x4440,
         0xae80, 0xa640, 0xee00, 0x8800, 0x4400, 0xe000, 0x0000},
        {0x2000, 0xeee0, 0x6200, 0x8400, 0xc600, 0xe800, 0xec00, 0x68a0, 0x68e0,
         0x8ae0, 0xace0, 0x0400, 0x0600, 0x4a00, 0x8e00, 0x0000},
        {
            0x20a0,
            0x24c0,
            0x68e0,
            0xace0,
            0xcee0,
            0xeee0,
            0xcae0,
            0xa8e0,
            0x86e0,
            0x8e00,
            0x4a00,
            0x2000,
            0x6200,
            0xc600,
            0xec00,
        }};

    std::map<uint8_t, unsigned long> sfTextures;
    std::map<uint8_t, Texture *> textures;

    std::map<uint8_t, Font> fonts;

    uint32_t color16to32ABGR(uint16_t src);

    SfmlArtist& artist_;
    resource_store::TextureStore& textureStore_;
};
