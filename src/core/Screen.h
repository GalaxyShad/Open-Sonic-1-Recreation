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
    Size size;
    Frame *frames;
    uint16_t framesLen;
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
};

#include "core/game_enviroment/ResourceStore.h"
#include "sfml_game_environment/SfmlArtist.h"

class Screen : public IScreen {
public:
    explicit Screen(SfmlArtist &artist, ResourceStore &store)
        : artist_(artist), store_(store) {}
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
        return &textures_[texture];
    }

    void loadTextureFromFile(const char *filename, uint8_t key,
                             const Frame *frames = nullptr,
                             uint16_t framesLen = 0, uint16_t width = 0,
                             uint16_t height = 0);


    void bindTexture(uint8_t key, ResourceID resId);
    void bindTextureFrames(uint8_t key, const Frame* frames, size_t framesLen);
    void bindFont(uint8_t key, ResourceID resId);

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
    std::map<uint8_t, Texture> textures_;
    std::map<uint8_t, std::vector<Frame>> textureFrames_;
    std::map<uint8_t, ResourceID> sfTextures_;
    std::map<uint8_t, ResourceID> fonts_;

    SfmlArtist &artist_;
    ResourceStore &store_;
};
