#ifndef OS1R_SFMLARTIST_H
#define OS1R_SFMLARTIST_H

#include "SFML/Graphics/Color.hpp"
#include "core/game_enviroment/artist/Artist.h"

#include "SFML/Graphics.hpp"
#include "core/game_enviroment/artist/ArtistStructs.h"

struct SfmlTexture : public artist_api::Texture {
    explicit SfmlTexture(const sf::Texture &tex) : internal(tex) {}
    sf::Texture internal;
};

class SfmlArtist : public artist_api::Artist {
public:
    explicit SfmlArtist(sf::RenderWindow &renderWindow);

    void setBackgroundColor(artist_api::Color color) override {
        bgColor_ = sf::Color(color.rgba());
    }

    void drawTextureRect(const artist_api::Texture &texture,
                         artist_api::Rect rect, artist_api::Vector2D<float> pos,
                         artist_api::Artist::TransformProps transform = {
                             .scale = {.x = 1.f, .y = 1.f}}) override;

    void drawSprite(const artist_api::Sprite &sprite,
                    artist_api::Vector2D<float> pos,
                    artist_api::Artist::TransformProps transform = {
                        .scale = {.x = 1.f, .y = 1.f}}) override;

    void drawText(const std::string &text, artist_api::Vector2D<float> pos,
                  const artist_api::SpriteFont &font) override;

    void drawRectangleRadius(artist_api::Vector2D<float> radius,
                             artist_api::Vector2D<float> pos,
                             DrawRectangleProps props = {}) override;

    void renderClear();
    void render();

    inline sf::RenderWindow &sfmlRenderWindow() { return renderWindow_; }

private:
    sf::RenderWindow &renderWindow_;
    sf::Color bgColor_;
};

#endif // OS1R_SFMLARTIST_H
