#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include "Bg.h"
#include "Camera.h"
#include "core/game_enviroment/InputMgr.h"
#include "core/game_enviroment/artist/ArtistStructs.h"
#include <memory>

const Frame framesTitleSonic[] = {
    Frame(257, 0, 72, 88),  Frame(330, 8, 80, 80),  Frame(411, 8, 80, 80),
    Frame(492, 0, 80, 88),  Frame(257, 97, 72, 88), Frame(330, 89, 88, 96),
    Frame(419, 89, 88, 96), Frame(508, 89, 88, 96),
};

class TitleScreen {
public:
    TitleScreen(Screen &scr) : scr(scr), cam(scr) {

        bgTexture_ =
            scr.textureLoader().loadFromFile("content/textures/texTitle.png");

        pos = v2f((float)scr.getSize().width / 2,
                  (float)scr.getSize().height / 2);

        yAnimShift = 8;
    }

    void draw() {

        yAnimShift -= 6;
        if (yAnimShift < -48) {
            yAnimShift = -48;
        }

        artist_api::Sprite sprBackRing = {.texture = *bgTexture_,
                                    .rect = {.width = 256, .height = 82}};
        sprBackRing.setOffsetCenter();

        artist_api::Sprite sprFrontRing = {.texture = *bgTexture_,
                                    .rect = {.y = 83, .width = 256, .height = 82}};
        sprBackRing.setOffsetCenter();


        scr.artist().drawSprite(sprBackRing, {.x = pos.x, .y = pos.y});
        scr.artist().drawSprite(sprFrontRing, {.x = pos.x, .y = pos.y});
    }

private:
    Screen &scr;
    Camera cam;

    v2f pos;

    float currentFrame_ = 0;
    float frameSpeed_ = 0.125f;

    std::unique_ptr<artist_api::Texture> bgTexture_;

    float yAnimShift;
};

#endif // TITLESCREEN_H
