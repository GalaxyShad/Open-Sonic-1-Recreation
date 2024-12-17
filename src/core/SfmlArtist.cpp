#include "SfmlArtist.h"

SfmlArtist::SfmlArtist(sf::RenderWindow &renderWindow)
    : renderWindow_(renderWindow) {
    
}

void SfmlArtist::drawTextureRect(const artist_api::Texture &texture,
                                 artist_api::Rect rect,
                                 artist_api::Vector2D<float> pos,
                                 artist_api::Artist::TransformProps transform) {
    drawSprite(
        {.texture = texture, .rect = rect, .offset = {.x = 0.f, .y = 0.f}}, pos,
        transform);
}

void SfmlArtist::drawSprite(const artist_api::Sprite &sprite,
                            artist_api::Vector2D<float> pos,
                            artist_api::Artist::TransformProps transform) {

    sf::Texture &sfmlTexture = ((SfmlTexture&)sprite.texture).internal;

    sf::IntRect sfmlSpriteRect((int)sprite.rect.x, (int)sprite.rect.y,
                               (int)sprite.rect.width, (int)sprite.rect.height);

    sf::Sprite sfmlSprite(sfmlTexture, sfmlSpriteRect);

    sfmlSprite.setPosition(pos.x, pos.y);
    sfmlSprite.setRotation(transform.angle);

//    sf::Vector2f sfmlScale(transform.scale.x, transform.scale.y);
    sf::Vector2f sfmlScale(1, 1);
    sfmlScale.x *= (transform.flipHorizontal) ? -1.f : 1.f;
    sfmlScale.y *= (transform.flipVertical) ? -1.f : 1.f;
    sfmlSprite.setOrigin(sprite.offset.x, sprite.offset.y);
    sfmlSprite.setScale(sfmlScale);

    renderWindow_.draw(sfmlSprite);
}

void SfmlArtist::render() {
    renderWindow_.display();
}

void SfmlArtist::renderClear() {
    renderWindow_.clear();
}
