#ifndef OS1R_SFMLARTIST_H
#define OS1R_SFMLARTIST_H

#include "Artist.h"

#include <SFML/Graphics.hpp>

struct SfmlTexture : public artist_api::Texture {
    explicit SfmlTexture(const sf::Texture &tex) : internal(tex) {}
    sf::Texture internal;
};

class SfmlArtist : public artist_api::Artist {
  public:
    SfmlArtist();

    void drawTextureRect(const artist_api::Texture &texture,
                         artist_api::Rect rect, artist_api::Vector2D<float> pos,
                         artist_api::Artist::TransformProps transform = {
                             .scale = {.x = 1.f, .y = 1.f}}) override;

    void drawSprite(const artist_api::Sprite &sprite,
                    artist_api::Vector2D<float> pos,
                    artist_api::Artist::TransformProps transform = {
                        .scale = {.x = 1.f, .y = 1.f}}) override;

    void renderClear();
    void renderLoop();

    inline sf::RenderWindow &sfmlRenderWindow() { return renderWindow_; }

  private:
    sf::RenderWindow renderWindow_;
};

#endif // OS1R_SFMLARTIST_H
