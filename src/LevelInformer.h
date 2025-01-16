#pragma once

#include "Camera.h"
#include "core/Audio.h"
#include "core/Geometry.h"
#include "core/game_enviroment/artist/ArtistStructs.h"
#include "core/game_enviroment/artist/SpriteFont.h"
#include "entities/Entity.h"
#include "entity-pool.hpp"
#include "sonic/SonicResources.h"
#include <format>

class TitleCardSonic1 : public Entity {
public:
    TitleCardSonic1(const std::string &zonename, int act, float screenWidth,
                    EntityPool &entityPool)
        : m_act(act), m_zonename(zonename), m_entityPool(entityPool) {
        m_textActX = screenWidth;
        m_shapeX = screenWidth;
        m_textTopX = 0;
        m_textBottomX = 0;
    };

    EntityTypeID type() override { return EntityTypeID::LEVEL_INFORMER; }

    void draw(Camera &cam) override {
        auto &scr = cam.getScr();

        if (m_tick < 200) {
            if (m_tick > 0) {
                m_textTopX += 8;
                m_shapeX -= 8;
                if (m_tick > 30)
                    m_textBottomX += 8;
                if (m_tick > 50)
                    m_textActX -= 8;
            }
        } else {
            m_textTopX -= 16;
            m_textBottomX -= 16;
            m_shapeX += 16;
            m_textActX += 16;
        }

        float centerX = 427 / 2;

        m_tick++;

        if (m_shapeX < centerX + 25 )
            m_shapeX = centerX + 25;
        if (m_textActX < centerX + 32)
            m_textActX = centerX + 32;

        if (m_textTopX > centerX + 75)
            m_textTopX = centerX + 75;
        if (m_textBottomX > centerX + 58)
            m_textBottomX = centerX + 58;


        auto& sprAct = scr.store().get<artist_api::Animation>(
            scr.store().map<SonicResources>().animations.act.act
        ).frames[0];
        auto& sprActBack = scr.store().get<artist_api::Animation>(
            scr.store().map<SonicResources>().animations.act.ellipse
        ).frames[0];

        scr.artist().drawSprite(
            sprActBack,
            artist_api::Vector2D<float>{.x = m_shapeX, .y = 69});
        scr.artist().drawSprite(
            sprAct,
            artist_api::Vector2D<float>{.x = m_textActX, .y = 110});

        v2f _v2f=v2f(m_textActX + 29, 94);

        auto& actResources = scr.store().map<SonicResources>().animations.act;
        auto actDigitResource = actResources.d1;
        switch (m_act) {
            case 1:
                _v2f.x+=3;
                actDigitResource = actResources.d1;
                break;
            case 2:
                actDigitResource = actResources.d2;
                break;
            case 3:
                actDigitResource = actResources.d3;
                break;
        }

        auto& sprActDigit = scr.store().get<artist_api::Animation>(actDigitResource).frames[0];

        scr.artist().drawSprite(
            sprActDigit,
            artist_api::Vector2D<float>{.x = _v2f.x, .y = _v2f.y});


        auto &font = scr.store().get<artist_api::SpriteFont>(
            scr.store().map<SonicResources>().fonts.s1TitleCard);

        scr.artist().drawText(
            m_zonename, {.x = m_textTopX, .y = 72}, font,
            {.horizontalAlign = artist_api::HorizontalAlign::RIGHT});
        scr.artist().drawText(
            "ZONE", {.x = m_textBottomX, .y = 92}, font,
            {.horizontalAlign = artist_api::HorizontalAlign::RIGHT});

        if (m_tick > 220) {
            m_entityPool.destroy(*this);
        }
    }

private:
    EntityPool &m_entityPool;
    const std::string &m_zonename;
    int m_act;
    int m_tick = 0;

    float m_textTopX;
    float m_textBottomX;
    float m_textActX;
    float m_shapeX;
};

class LevelInformer : Entity {
public:
    enum Type { T_TITLE_CARD, T_ROUND_CLEAR };
    LevelInformer(const char *zone, int act, Screen &scr, Audio &audio,
                  Type type, int *score = nullptr, int rings = 0, int time = 0)
        : zone(zone), act(act), scr(scr), audio(audio), type(type),

          sndBeep_(audio.store().get<dj::Sound>(
              audio.store().map<SonicResources>().sounds.beep)),
          sndCountEnd_(audio.store().get<dj::Sound>(
              audio.store().map<SonicResources>().sounds.ding))

    {
        tick = (type == T_TITLE_CARD) ? 0 : -100;
        yShift = (type == T_TITLE_CARD) ? 0 : -20;

        int w = 427;
        xAct = w;
        xShape = w;
        xFontTop = 0;
        xFontBottom = 0;

        xScore = w;
        xTimeBonus = w + 64;
        xRingBonus = w + 128;

        xCenter = w / 2;

        if (score) {
            this->score = score;
            ringBonus = rings * 100;
            if (time < 30)
                timeBonus = 50000;
            else if (time >= 30 && time < 45)
                timeBonus = 10000;
            else if (time >= 45 && time < 60)
                timeBonus = 5000;
            else if (time >= 60 && time < 90)
                timeBonus = 4000;
            else if (time >= 90 && time < 120)
                timeBonus = 3000;
            else if (time >= 120 && time < 180)
                timeBonus = 2000;
            else if (time >= 180 && time < 240)
                timeBonus = 1000;
            else if (time >= 240 && time < 300)
                timeBonus = 500;
            else
                timeBonus = 0;
        }
    }

    void draw() {
        if (tick < 200) {
            if (tick > 0) {
                xFontTop += 8;
                xShape -= 8;
                if (tick > 30)
                    xFontBottom += 8;
                if (tick > 50)
                    xAct -= 8;
                if (tick > 80 && type == T_ROUND_CLEAR) {
                    xScore -= 8;
                    xTimeBonus -= 8;
                    xRingBonus -= 8;
                }
            }
        } else {
            if (type == T_TITLE_CARD) {
                xFontTop -= 16;
                xFontBottom -= 16;
                xShape += 16;
                xAct += 16;
            }
        }

        if (tick % 5 == 0 && tick > 250) {
            if (timeBonus > 0) {
                timeBonus -= 100;
                *score += 100;
            }

            if (ringBonus > 0) {
                ringBonus -= 100;
                *score += 100;
            }

            if (timeBonus > 0 || ringBonus > 0)
                audio.dj().playSound(sndBeep_);

            if (timeBonus == 0 && ringBonus == 0) {
                audio.dj().playSound(sndCountEnd_);
                tick = 0;
            }
        }

        tick++;

        if (xShape < xCenter + 25)
            xShape = xCenter + 25;
        if (xAct < xCenter + 32)
            xAct = xCenter + 32;

        if (xScore < xCenter - 80)
            xScore = xCenter - 80;
        if (xTimeBonus < xCenter - 80)
            xTimeBonus = xCenter - 80;
        if (xRingBonus < xCenter - 80)
            xRingBonus = xCenter - 80;

        if (xFontTop > xCenter + 75)
            xFontTop = xCenter + 75;
        if (xFontBottom > xCenter + 58)
            xFontBottom = xCenter + 58;


        auto &t = scr.store().get<artist_api::Texture>(
            scr.store().map<SonicResources>().textures.objects
        );

        scr.artist().drawSprite({.texture = t,
            .rect = {.x = 191.0,
                    .y = 77.0,
                    .width = 56.0,
                    .height = 56.0}},
        artist_api::Vector2D<float>{.x = xShape, .y = 69 + (float)yShift});
        scr.artist().drawSprite({.texture = t,
            .rect = {.x = 2.0,
                    .y = 121.0,
                    .width = 24.0,
                    .height = 8.0}},
        artist_api::Vector2D<float>{.x = xAct, .y = 110 + (float)yShift});

        irect _irect;
        v2f _v2f=v2f(xAct + 29, yShift);
        switch (act) {
            case 1:
                _v2f.x+=3;
                _irect=irect(34,113,7,24);
                break;
            case 2:
                _irect=irect(51,113,14,24);
                break;
            case 3:
                _irect=irect(75,113,14,24);
                break;
        }
        scr.artist().drawSprite({.texture = t,
            .rect = {.x = (float)_irect.left,
                    .y = (float)_irect.top,
                    .width = (float)_irect.width,
                    .height = (float)_irect.height}},
        artist_api::Vector2D<float>{.x = _v2f.x, .y = _v2f.y});


        auto &font = scr.store().get<artist_api::SpriteFont>(
            scr.store().map<SonicResources>().fonts.s1TitleCard);

        scr.artist().drawText(
            "SONIC HAS", {.x = xFontTop, .y = 72.f + yShift}, font,
            {.horizontalAlign = artist_api::HorizontalAlign::RIGHT});
        scr.artist().drawText(
            "PASSED", {.x = xFontBottom, .y = 92.f + yShift}, font,
            {.horizontalAlign = artist_api::HorizontalAlign::RIGHT});

        if (type != T_ROUND_CLEAR)
            return;


        const uint8_t shift = 16;
        artist_api::Sprite _sprit = {
            .texture = t,
            .rect = {.x = 231.0,
                    .y = 60.0,
                    .width = 16,
                    .height = 16
                    }
        };
        scr.artist().drawSprite(_sprit,
        artist_api::Vector2D<float>{.x = xScore + 29, .y = 100 + shift});
        scr.artist().drawSprite(_sprit,
        artist_api::Vector2D<float>{.x = xTimeBonus + 70, .y = 116 + shift});
        scr.artist().drawSprite(_sprit,
        artist_api::Vector2D<float>{.x = xRingBonus + 70, .y = 132 + shift});

        scr.artist().drawSprite({.texture = t,
            .rect = {.x = 1.0,
                    .y = 60.0,
                    .width = 40,
                    .height = 16}},
        artist_api::Vector2D<float>{.x = xScore, .y = 100 + shift});
        scr.artist().drawText(std::format("{:8d}", *score),
                              {xScore + 96, 100 + shift}, font);


        scr.artist().drawSprite({.texture = t,
            .rect = {.x = 42.0,
                    .y = 60.0,
                    .width = 32,
                    .height = 16}},
        artist_api::Vector2D<float>{.x = xTimeBonus, .y = 116 + shift});
        scr.artist().drawSprite({.texture = t,
            .rect = {.x = 116.0,
                    .y = 60.0,
                    .width = 40,
                    .height = 16}},
        artist_api::Vector2D<float>{.x = xTimeBonus + 40, .y = 116 + shift});

        scr.artist().drawText(std::format("{:8d}", timeBonus),
                              {xScore + 96, 116 + 16}, font);


        scr.artist().drawSprite({.texture = t,
            .rect = {.x = 75.0,
                    .y = 60.0,
                    .width = 32,
                    .height = 16}},
        artist_api::Vector2D<float>{.x = xRingBonus, .y = 132 + shift});
        scr.artist().drawSprite({.texture = t,
            .rect = {.x = 116.0,
                    .y = 60.0,
                    .width = 40,
                    .height = 16}},
        artist_api::Vector2D<float>{.x = xRingBonus + 40, .y = 132 + shift});

        scr.artist().drawText(std::format("{:8d}", ringBonus),
                              {xRingBonus + 96, 132 + shift}, font);
    }

    bool isDone() {
        if (type == T_TITLE_CARD)
            return (tick > 220);
        else
            return (timeBonus == 0 && ringBonus == 0 && tick > 50);
    }

    int getTick() { return tick; }
    Type getType() { return type; }

private:
    Screen &scr;
    Audio &audio;

    dj::Sound &sndBeep_;
    dj::Sound &sndCountEnd_;

    const char *zone;
    int act, tick;
    int xCenter;
    int yShift = 0;
    float xFontTop, xFontBottom, xAct, xShape, xScore, xTimeBonus, xRingBonus;

    int *score = nullptr;

    int timeBonus = 0;
    int ringBonus = 0;

    Type type;
};