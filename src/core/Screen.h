#pragma once

#include <cstdint>
#include <map>
#include <cstring>

#include "Geometry.h"
#include <SFML/Graphics.hpp>

struct Frame {
    Frame() {}
	Frame(IntRect _rect):
        rect(_rect), offset(Vector2i(_rect.width / 2, _rect.height / 2)) {}
    Frame(IntRect _rect, Vector2i _offset):
        rect(_rect), offset(_offset) {}
    Frame(int x, int y, int width, int height):
        rect(IntRect(x, y, width, height)), offset(Vector2i(width / 2, height / 2)) {}

    IntRect rect;
    Vector2i offset = Vector2i(0, 0);
};

struct Texture {
    uint8_t* indexes;
    Size size;
    uint8_t palRow;
    Frame* frames;
    uint16_t framesLen;
};

struct Letter {
    uint32_t xPos;
    uint16_t width;
};

struct Font {
    uint8_t tex;
    IntRect startRect;
    uint16_t interval;
    std::map<char, Letter> letters;
};

class IScreen {
    public:
        virtual void init(Size size, int frameLock) = 0;
        virtual void clear() = 0;
        virtual void render() = 0;

        virtual void setPalRow(uint8_t row,
            const uint16_t* colors, uint8_t len = 16, uint8_t from = 0) = 0;
        virtual uint16_t getPalColor(uint8_t row, uint8_t index) = 0;

        virtual uint16_t getBgColor() = 0;
        virtual void     setBgColor(uint16_t color) = 0;

        virtual void setSize(Size size) = 0;
        virtual Size getSize() = 0;

        virtual void drawTextureRect(uint8_t texId,
                        IntRect texRect, Vector2f pos = {0, 0}, 
                        Vector2i offset = {0, 0}, float angle = 0.0, 
                        bool horFlip = false, bool verFlip = false) = 0;
        virtual void drawTextureRect(uint8_t texId,
                        Frame frame, Vector2f pos = {0, 0}, float angle = 0.0, 
                        bool horFlip = false, bool verFlip = false) = 0;

        virtual void loadTexture(const uint8_t* src, 
                                 uint16_t width, uint16_t height, 
                                 uint8_t palRow,
                                 uint8_t key,
                                 const Frame* frames = nullptr,
                                 uint16_t framesLen = 0) = 0;
        virtual void freeTexture(uint8_t key) = 0;
};

#define PAL_MAX_ROWS    4
#define PAL_MAX_COLUMNS 16

class Screen : public IScreen {
    public:
        void init(Size size, int frameLock);
        void clear();
        void render();

        void setPalRow(uint8_t row, const uint16_t* colors, 
                       uint8_t len = 16, uint8_t from = 0);
        uint16_t getPalColor(uint8_t row, uint8_t index) 
            { return (row < PAL_MAX_ROWS && index < PAL_MAX_COLUMNS) ? pal[row][index] : 0; }

        uint16_t getBgColor()               { return bgColor;  }
        void     setBgColor(uint16_t color) { bgColor = color; }

        void setSize(Size size) { this->size = size; }
        Size getSize() { return size; }

        void drawTextureRect(uint8_t texId,
                             IntRect texRect, Vector2f pos = {0, 0}, 
                             Vector2i offset = {0, 0}, float angle = 0.0, 
                             bool horFlip = false, bool verFlip = false);
        void drawTextureRect(uint8_t texId,
                             Frame frame, Vector2f pos = {0, 0}, float angle = 0.0, 
                             bool horFlip = false, bool verFlip = false);

        void drawRectangle(Vector2f pos, Size rsize = Size(16, 16), uint16_t color = 0xFFFF);

        const Texture* getTexture(uint8_t texture) 
            {return textures.count(texture) ? textures[texture] : nullptr; }

        void loadTexture(const uint8_t* src, 
                         uint16_t width, uint16_t height, 
                         uint8_t palRow,
                         uint8_t key,
                         const Frame* frames = nullptr,
                         uint16_t framesLen = 0);
        void loadTextureFromFile(const char* filename, 
                                 uint8_t key,
                                 const Frame* frames = nullptr,
                                 uint16_t framesLen = 0,
                                 uint16_t width = 0, uint16_t height = 0);
        void freeTexture(uint8_t key);

        void drawPal(Vector2f pos);

        void addFont(uint8_t key, const char* alphabet, uint8_t interval, 
                     uint8_t tex, IntRect startRect, uint16_t rectDivSpace=0, const uint16_t* widths = nullptr);
        void drawText(uint8_t fontKey, const char* str, Vector2f pos = Vector2f(0, 0));
        uint16_t getTextWidth(uint8_t fontKey, const char* str);

        void setBrightness(uint8_t _brightness) { brightness = _brightness; }

        sf::RenderWindow& getSfmlWindow() { return wnd; }
    private:
        uint16_t pal[PAL_MAX_ROWS][PAL_MAX_COLUMNS] = { 
    {0x0000, 0x2280, 0x44a0, 0x66c0, 0x88e0, 0xeee0, 0xaaa0, 0x8880, 0x4440, 0xea80, 0xa640, 0xe000, 0x8000, 0x4000, 0xee00, 0x0000},
    {0x0000, 0x2420, 0x4640, 0x6860, 0x8c80, 0xeee0, 0xaaa0, 0x8880, 0x4440, 0xae80, 0xa640, 0xee00, 0x8800, 0x4400, 0xe000, 0x0000},
    {0x2000, 0xeee0, 0x6200, 0x8400, 0xc600, 0xe800, 0xec00, 0x68a0, 0x68e0, 0x8ae0, 0xace0, 0x0400, 0x0600, 0x4a00, 0x8e00, 0x0000},
    {0x20a0, 0x24c0, 0x68e0, 0xace0, 0xcee0, 0xeee0, 0xcae0, 0xa8e0, 0x86e0, 0x8e00, 0x4a00, 0x2000, 0x6200, 0xc600, 0xec00, }
        };
        std::map<uint8_t, Texture*> textures;
        std::map<uint8_t, sf::Texture*> sfTextures;
        std::map<uint8_t, Font> fonts;

        Size size;
        uint8_t brightness = 0xFF;


        sf::RenderWindow wnd;
        uint16_t bgColor = 0x00000000;


        uint32_t color16to32RGBA(uint16_t src);
        uint32_t color16to32ABGR(uint16_t src);
};

