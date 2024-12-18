#include "SpriteFont.h"

artist_api::SpriteFont
artist_api::SpriteFont::fromSpriteRow(const Texture &tex, Rect startRect,
                                      FromSpriteRowParams params) {

    std::vector<std::pair<char, Sprite>> res;
    res.reserve(params.alphabet.size());

    Sprite letterSprite = {.texture = tex, .rect = startRect};

    for (char letter : params.alphabet) {
        res.emplace_back(letter, letterSprite);

        letterSprite.rect.x += letterSprite.rect.width + (float)params.pixelsBetweenRects;
    }

    return SpriteFont(res);
}
artist_api::SpriteFont::SpriteFont(
    const std::vector<std::pair<char, Sprite>> &x) : firstCharSprite(x[0].second) {

    for (auto& i : x) {
        symMap_.emplace(i.first, i.second);
    }
}
