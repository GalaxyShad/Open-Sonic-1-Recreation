#include "SfmlArtist.h"
#include "SFML/Graphics/Color.hpp"

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

    if (transform.blending.has_value()) {
        sfmlSprite.setColor(sf::Color(transform.blending->rgba()));
    }

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
    renderWindow_.clear(bgColor_);
    // renderWindow_.clear(sf::Color::Cyan);
}

void SfmlArtist::drawText(const std::string &text,
                          artist_api::Vector2D<float> pos,
                          const artist_api::SpriteFont &font) {

    artist_api::Vector2D<float> charPos = pos;

    for (auto ch : text) {
        if (ch == '\r') {
            charPos.x = pos.x;

            continue;
        }

        if (ch == '\n') {
            charPos.x = pos.x;
            charPos.y += font.lineHeight();

            continue;
        }

        if (ch == ' ') {
            charPos.x += font.spaceWidth();

            continue;
        }

        if (!font.hasChar(ch)) {
            sf::RectangleShape emptyChRect(sf::Vector2f(font.spaceWidth(), font.lineHeight()));
            emptyChRect.setFillColor(sf::Color::White);
            emptyChRect.setPosition(charPos.x, charPos.y);

            renderWindow_.draw(emptyChRect);

            continue;
        }

        auto& charSprite = font.getCharSprite(ch);

        drawSprite(charSprite, charPos);

        charPos.x += charSprite.rect.width;
    }
}

void SfmlArtist::drawRectangleRadius(artist_api::Vector2D<float> radius,
                                     artist_api::Vector2D<float> pos,
                                     DrawRectangleProps props) {
    sf::RectangleShape rs;

    rs.setPosition(pos.x, pos.y);
    rs.setOrigin(radius.x, radius.y);
    rs.setSize(sf::Vector2f(radius.x * 2, radius.y * 2));

    sf::Color color = props.fillColor.has_value()
                          ? sf::Color(props.fillColor->rgba())
                          : sf::Color::White;
    sf::Color outline = props.borderColor.has_value()
                            ? sf::Color(props.borderColor->rgba())
                            : sf::Color::Transparent;

    rs.setFillColor(color);
    rs.setOutlineColor(outline);
    rs.setOutlineThickness(props.borderThickness);

    renderWindow_.draw(rs);
}
