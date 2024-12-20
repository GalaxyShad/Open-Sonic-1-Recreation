#include "SpriteFont.h"

artist_api::SpriteFont
artist_api::SpriteFont::fromSpriteRow(const Sprite &startSprite,
                                      const std::string &alphabet,
                                      FromSpriteRowParams params) {

    std::vector<std::pair<char, Sprite>> res;
    res.reserve(alphabet.size());

    Sprite letterSprite = startSprite;

    for (char letter : alphabet) {
        res.emplace_back(letter, letterSprite);

        letterSprite.rect.x +=
            letterSprite.rect.width + (float)params.pixelsBetweenRects;
    }

    return SpriteFont(res);
}
artist_api::SpriteFont::SpriteFont(
    const std::vector<std::pair<char, Sprite>> &x)
    : firstCharSprite(x[0].second) {

    for (auto &i : x) {
        symMap_.emplace(i.first, i.second);
    }
}
artist_api::SpriteFont artist_api::SpriteFont::fromSpriteRowWidths(
    const artist_api::Sprite &spr,
    const std::vector<std::pair<char, int>> &alphabet,
    FromSpriteRowParams params) {

    std::vector<std::pair<char, Sprite>> res;
    res.reserve(alphabet.size());

    Sprite letterSprite = spr;

    for (auto &a : alphabet) {
        letterSprite.rect.width = static_cast<float>(a.second);

        res.emplace_back(a.first, letterSprite);

        letterSprite.rect.x +=
            letterSprite.rect.width + params.pixelsBetweenRects;
    }

    return SpriteFont(res);
}
