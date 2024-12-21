#pragma once

#include "Camera.h"
#include "core/Audio.h"
#include "entities/Entity.h"
#include "entity-pool.hpp"

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

        float centerX = scr.getSize().width / 2;

        m_tick++;

        if (m_shapeX < centerX + 25)
            m_shapeX = centerX + 25;
        if (m_textActX < centerX + 32)
            m_textActX = centerX + 32;

        if (m_textTopX > centerX + 75)
            m_textTopX = centerX + 75;
        if (m_textBottomX > centerX + 58)
            m_textBottomX = centerX + 58;

        scr.drawTextureRect(2, irect(191, 77, 56, 56), v2f(m_shapeX, 69));
        scr.drawTextureRect(2, irect(2, 121, 24, 8), v2f(m_textActX, 110));

        switch (m_act) {
        case 1:
            scr.drawTextureRect(2, irect(34, 113, 7, 24),
                                v2f(m_textActX + 32, 94));
            break;
        case 2:
            scr.drawTextureRect(2, irect(51, 113, 14, 24),
                                v2f(m_textActX + 29, 94));
            break;
        case 3:
            scr.drawTextureRect(2, irect(75, 113, 14, 24),
                                v2f(m_textActX + 29, 94));
            break;
        }

        scr.drawText(
            2, m_zonename.c_str(),
            v2f(m_textTopX - scr.getTextWidth(2, m_zonename.c_str()), 72));
        scr.drawText(2, "ZONE",
                     v2f(m_textBottomX - scr.getTextWidth(2, "ZONE"), 92));

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

        sndBeep_(audio.store().get<dj::Sound>(audio.store().map<SonicResources>().sounds.beep)),
          sndCountEnd_(audio.store().get<dj::Sound>(audio.store().map<SonicResources>().sounds.ding))

    {
        tick = (type == T_TITLE_CARD) ? 0 : -100;
        yShift = (type == T_TITLE_CARD) ? 0 : -20;

        int w = scr.getSize().width;
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

        scr.drawTextureRect(2, irect(191, 77, 56, 56),
                            v2f(xShape, 69 + yShift));
        scr.drawTextureRect(2, irect(2, 121, 24, 8), v2f(xAct, 110 + yShift));

        switch (act) {
        case 1:
            scr.drawTextureRect(2, irect(34, 113, 7, 24),
                                v2f(xAct + 32, 94 + yShift));
            break;
        case 2:
            scr.drawTextureRect(2, irect(51, 113, 14, 24),
                                v2f(xAct + 29, 94 + yShift));
            break;
        case 3:
            scr.drawTextureRect(2, irect(75, 113, 14, 24),
                                v2f(xAct + 29, 94 + yShift));
            break;
        }

        scr.drawText(2, (type == T_TITLE_CARD) ? zone : "SONIC HAS",
                     v2f(xFontTop - scr.getTextWidth(2, (type == T_TITLE_CARD)
                                                            ? zone
                                                            : "SONIC HAS"),
                         72 + yShift));
        scr.drawText(2, (type == T_TITLE_CARD) ? "ZONE" : "PASSED",
                     v2f(xFontBottom -
                             scr.getTextWidth(
                                 2, (type == T_TITLE_CARD) ? "ZONE" : "PASSED"),
                         92 + yShift));

        if (type != T_ROUND_CLEAR)
            return;

        const uint8_t shift = 16;
        scr.drawTextureRect(2, irect(231, 60, 16, 16),
                            v2f(xScore + 29, 100 + shift));
        scr.drawTextureRect(2, irect(231, 60, 16, 16),
                            v2f(xTimeBonus + 70, 116 + shift));
        scr.drawTextureRect(2, irect(231, 60, 16, 16),
                            v2f(xRingBonus + 70, 132 + shift));

        char strbuff[9];
        scr.drawTextureRect(2, irect(1, 60, 40, 16), v2f(xScore, 100 + shift));
        snprintf(strbuff, 9, "%8d", *score);
        scr.drawText(3, strbuff, v2f(xScore + 96, 100 + shift));
        scr.drawTextureRect(2, irect(42, 60, 32, 16),
                            v2f(xTimeBonus, 116 + shift));
        scr.drawTextureRect(2, irect(116, 60, 40, 16),
                            v2f(xTimeBonus + 40, 116 + shift));
        snprintf(strbuff, 9, "%8d", timeBonus);
        scr.drawText(3, strbuff, v2f(xTimeBonus + 96, 116 + 16));
        scr.drawTextureRect(2, irect(75, 60, 32, 16),
                            v2f(xRingBonus, 132 + shift));
        scr.drawTextureRect(2, irect(116, 60, 40, 16),
                            v2f(xRingBonus + 40, 132 + shift));
        snprintf(strbuff, 9, "%8d", ringBonus);
        scr.drawText(3, strbuff, v2f(xRingBonus + 96, 132 + shift));
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

    dj::Sound& sndBeep_;
    dj::Sound& sndCountEnd_;

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