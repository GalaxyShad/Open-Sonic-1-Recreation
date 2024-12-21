#include "SfmlArtist.h"
#include "SFML/Graphics/Color.hpp"
#include <string>

SfmlArtist::SfmlArtist(sf::RenderWindow &renderWindow)
    : renderWindow_(renderWindow) {}

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

    sf::Texture &sfmlTexture = ((SfmlTexture &)sprite.texture).internal;

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

void SfmlArtist::render() { renderWindow_.display(); }

void SfmlArtist::renderClear() { renderWindow_.clear(bgColor_); }

void SfmlArtist::drawText(const std::string &text,
                          artist_api::Vector2D<float> pos,
                          const artist_api::SpriteFont &font,
                          DrawTextProps props) {

    artist_api::Vector2D<float> charPos = pos;

    auto onLetter = [&](char ch, std::optional<artist_api::Sprite> spr,
                        const artist_api::Vector2D<float> &p) {
        if (spr.has_value()) {
            drawSprite(spr.value(), {.x = charPos.x + p.x, .y = charPos.y});
            return;
        }

        sf::RectangleShape emptyChRect(
            sf::Vector2f(font.spaceWidth(), font.lineHeight()));

        emptyChRect.setFillColor(sf::Color::White);
        emptyChRect.setPosition(charPos.x + p.x, charPos.y);

        renderWindow_.draw(emptyChRect);
    };

    auto start = text.begin();
    while (start != text.end()) {
        auto rowStart = start;
        auto rowWidth = 0.0f;

        if (props.horizontalAlign != artist_api::HorizontalAlign::LEFT) {
            auto calcWidth = [&rowWidth](char ch,
                                         std::optional<artist_api::Sprite> spr,
                                         const artist_api::Vector2D<float> &p) {
                rowWidth = p.x + spr->rect.width;
            };

            procTextRowAndReturnWidth(start, text.end(), font, calcWidth);

            if (props.horizontalAlign == artist_api::HorizontalAlign::RIGHT) {
                charPos.x = pos.x - rowWidth;
            } else {
                charPos.x = pos.x - rowWidth / 2;
            }
        }

        start = procTextRowAndReturnWidth(start, text.end(), font, onLetter);

        charPos.x = pos.x;
        charPos.y += font.lineHeight();

        if (start != text.end()) {
            ++start;
        }
    }
}

std::string::const_iterator SfmlArtist::procTextRowAndReturnWidth(
    std::string::const_iterator startString,
    std::string::const_iterator endString, const artist_api::SpriteFont &font,
    std::function<void(char letter, std::optional<artist_api::Sprite> letterSpr,
                       const artist_api::Vector2D<float> &pos)>
        onLetter) {

    artist_api::Vector2D<float> charPos = {.x = 0, .y = 0};
    std::string::const_iterator chPtr;

    for (chPtr = startString; chPtr != endString && *chPtr != '\n'; chPtr++) {
        char ch = *chPtr;

        if (ch == ' ') {
            charPos.x += font.spaceWidth();

            continue;
        }

        if (!font.hasChar(ch)) {
            onLetter(ch, std::nullopt, charPos);

            continue;
        }

        auto &charSprite = font.getCharSprite(ch);

        onLetter(ch, charSprite, charPos);

        charPos.x += charSprite.rect.width;
    }

    return chPtr;
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