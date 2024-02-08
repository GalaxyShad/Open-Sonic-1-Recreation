#include "Screen.h"

#include <iostream>
#include <math.h>

void Screen::init(Size size, int frameLock) {
    this->size = size;
    wnd.create(sf::VideoMode(size.width, size.height), "Test");
    wnd.setFramerateLimit(frameLock);
}

void Screen::clear() {
    wnd.clear(sf::Color(color16to32RGBA(bgColor)));
}

void Screen::render() {
    if (brightness < 0xFF) {
        sf::RectangleShape rs(sf::Vector2f(size.width, size.height));
        rs.setFillColor(sf::Color(0, 0, 0, 0xFF-brightness));
        wnd.draw(rs);
    }
    wnd.display();
}

void Screen::setPalRow(uint8_t row, const uint16_t* colors, 
                       uint8_t len, uint8_t from) {

    if (!colors || row >= PAL_MAX_ROWS || 
        len >= PAL_MAX_COLUMNS || from >= PAL_MAX_COLUMNS)
            return; 

    memcpy(pal + ((row*PAL_MAX_COLUMNS+from)*2), 
           colors,
           len*2);
}

void Screen::drawTextureRect(uint8_t texId,
                     irect texRect, v2f pos, 
                     v2i offset, float angle, 
                     bool horFlip, bool verFlip) {
    

    if (!sfTextures.count(texId) ||
        pos.x-offset.x > size.width || pos.y-offset.y > size.height ||
        pos.x+offset.x+texRect.width < 0 || pos.y+offset.y+texRect.height < 0)
            return;

    sf::Sprite spr(*sfTextures[texId]);
    spr.setTextureRect(sf::IntRect(texRect.left + texRect.width*horFlip, 
                                   texRect.top + texRect.height*verFlip,
                                   texRect.width*(horFlip ? -1 : 1),
                                   texRect.height*(verFlip ? -1 : 1)));
    spr.setPosition(pos.x, pos.y);
    spr.setOrigin(
        (!horFlip || !offset.x) ? offset.x : texRect.width-offset.x, 
        (!verFlip || !offset.y) ? offset.y : texRect.height-offset.y
    );
    spr.setRotation(angle);

    wnd.draw(spr);
}

void Screen::drawTextureRect(uint8_t texId,
                     Frame frame, v2f pos, float angle, 
                     bool horFlip, bool verFlip) {
    
    drawTextureRect(
        texId, 
        frame.rect,
        pos,
        frame.offset,
        angle,
        horFlip,
        verFlip
    );            
}

void Screen::drawRectangle(v2f pos, Size rsize, uint16_t color) {
    if (pos.x + rsize.width < 0 || pos.y + rsize.height < 0 || 
        pos.x > size.width ||
        pos.y > size.height)
            return;

    sf::RectangleShape rs(sf::Vector2f(rsize.width, rsize.height));
    rs.setPosition(sf::Vector2f(pos.x, pos.y));
    rs.setFillColor(sf::Color(color16to32RGBA(color)));

    wnd.draw(rs);
}

void Screen::loadTexture(const uint8_t* src, 
                         uint16_t width, uint16_t height, 
                         uint8_t palRow,
                         uint8_t key,
                         const Frame* frames,
                         uint16_t framesLen) {
    
    if (!src || palRow >= PAL_MAX_ROWS) {
        std::cout << "[ERR] bad texture " << (int)key << std::endl;
        return;
    }

    Texture* tex = new Texture;
    if (!tex)
        std::cout << "failed to create texture" << std::endl;

    tex->frames = nullptr;
    tex->framesLen = 0;
    tex->indexes = nullptr;
    tex->palRow = 0;
    tex->size = Size(0,0);

    tex->size = Size(width, height);
    tex->palRow = palRow;
    tex->indexes = new uint8_t[width*height];
    if (!tex->indexes)
        std::cout << "failed to load texture indexes" << std::endl;

    memcpy(tex->indexes, src, width*height);

    if (frames) {
        tex->frames = new Frame[framesLen];
        if (!tex->frames)
            std::cout << "failed to load texture frames" << std::endl;

        tex->framesLen = framesLen;
        memcpy(tex->frames, frames, tex->framesLen*sizeof(Frame));
    }

    textures[key] = tex; 

    // Texture* texture = textures[key];
    int w, h;
    w = tex->size.width;
    h = tex->size.height;

    uint32_t* pixels = new uint32_t[w * h];
    if (!pixels) 
        std::cout << "[ERR] out of memory!" << std::endl;

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            uint8_t index  = tex->indexes[i * w + j];

            uint8_t row    = index / 16;
            uint8_t column = index % 16 + (row ? 1 : -1);
            
            if (column >= 16)   column = 0;
            if (row >= 4)       row = 0; 
            
            if (index) 
                pixels[i*w+j] = color16to32ABGR(pal[row][column]);
            else
                pixels[i*w+j] = 0x00000000;
        }
    }

    sf::Texture* sfTexture = new sf::Texture();
    sfTexture->create(w, h);
    sfTexture->update((uint8_t*)pixels);

    sfTextures[key] = sfTexture;
}

void Screen::loadTextureFromFile(const char* filename, 
                                 uint8_t key,
                                 const Frame* frames,
                                 uint16_t framesLen,
                                 uint16_t width, uint16_t height) {
    
    Texture* tex = new Texture;
    if (!tex)
        std::cout << "failed to create texture" << std::endl;

    tex->frames = nullptr;
    tex->framesLen = 0;
    tex->indexes = nullptr;
    tex->palRow = 0;
    tex->size = Size(0,0);

    if (frames) {
        tex->frames = new Frame[framesLen];
        if (!tex->frames)
            std::cout << "failed to load texture frames" << std::endl;

        tex->framesLen = framesLen;
        memcpy(tex->frames, frames, tex->framesLen*sizeof(Frame));
    }

    textures[key] = tex; 

    int w, h;
    w = tex->size.width;
    h = tex->size.height;

    sf::Texture* sfTexture = new sf::Texture();
    sfTexture->loadFromFile(filename);

    tex->size = Size(sfTexture->getSize().x, sfTexture->getSize().y);

    sfTextures[key] = sfTexture;
}

void Screen::freeTexture(uint8_t key) {
    if (!textures.count(key))
        return;

    Texture* tex = textures[key];
    if (tex->indexes) delete tex->indexes;
    if (tex->frames && tex->framesLen) delete tex->frames;
    tex->framesLen = 0;
    tex->palRow = 0;
    tex->size = Size(0, 0);
    delete tex;
    sf::Texture* sfTex = sfTextures[key];
    delete sfTex;
    textures.erase(key);
    sfTextures.erase(key);
}

uint32_t Screen::color16to32RGBA(uint16_t src) {
    uint8_t r, g, b;
    r = ((src & 0xF000) >> 8) & 0xF0;
    g = ((src & 0x0F00) >> 4) & 0xF0;
    b = ((src & 0x00F0) >> 0) & 0xF0;

    uint32_t dst = 0x000000FF | (b << 8) | (g << 16) | (r << 24);
    
    return dst;
}

uint32_t Screen::color16to32ABGR(uint16_t src) {
    uint8_t r, g, b;
    r = ((src & 0xF000) >> 8) & 0xF0;
    g = ((src & 0x0F00) >> 4) & 0xF0;
    b = ((src & 0x00F0) >> 0) & 0xF0;

    uint32_t dst = 0xFF000000 | (r) | (g << 8) | (b << 16);
    
    return dst;
}

void Screen::drawPal(v2f pos) {

    for (int i = 0; i < PAL_MAX_ROWS; i++)
        for (int j = 0; j < PAL_MAX_COLUMNS; j++) {
            sf::RectangleShape rs(sf::Vector2f(16, 16));
            rs.setFillColor(sf::Color(color16to32RGBA(pal[i][j])));
            rs.setPosition(pos.x+16*j, pos.y+16*i);
            wnd.draw(rs);
        }
}

void Screen::addFont(uint8_t key, const char* alphabet, uint8_t interval, 
                     uint8_t tex, irect startRect, uint16_t rectDivSpace, const uint16_t* widths) {
    if (!alphabet) return;

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


void Screen::drawText(uint8_t fontKey, const char* str, v2f pos) {
    if (!fonts.count(fontKey)) return;

    if ((pos.x < 0) || (pos.y < 0) || (pos.x >= wnd.getDefaultView().getSize().x) || (pos.y >= wnd.getDefaultView().getSize().y)) 
        return;

    Font font = fonts[fontKey];
    irect letterRect = font.startRect;
    sf::Texture* texFont = sfTextures[font.tex];

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
            sf::Sprite sLetter(*texFont);
            sLetter.setPosition(letterPos.x, letterPos.y);
            sLetter.setTextureRect(sf::IntRect(letterRect.left, letterRect.top, 
                                               letterRect.width, letterRect.height));

            wnd.draw(sLetter);
        } else {
            letterRect = font.startRect;
        }

        letterPos.x += letterRect.width + font.interval;
    }
}

uint16_t Screen::getTextWidth(uint8_t fontKey, const char* str) {
    if (!fonts.count(fontKey)) return 0;

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





