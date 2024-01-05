#pragma once

#include "Camera.h"
#include "core/Audio.h"

class LevelInformer {
    public:
        enum Type { T_TITLE_CARD, T_ROUND_CLEAR };
        LevelInformer(const char* zone, int act, Screen& scr, Audio& audio, Type type, 
            int* score = nullptr, int rings = 0, int time = 0) : 
            zone(zone), act(act), scr(scr), audio(audio), type(type) {
                tick   = (type == T_TITLE_CARD) ? 0 : -100;
                yShift = (type == T_TITLE_CARD) ? 0 : -20;

                int w = scr.getSize().width;
                xAct = w;
                xShape = w;
                xFontTop = 0;
                xFontBottom = 0;

                xScore = w;
                xTimeBonus = w+64;
                xRingBonus = w+128;

                xCenter = w / 2;

                if (score) {
                    this->score = score;
                    ringBonus = rings * 100;
                    if      (time <  30)                timeBonus = 50000;
                    else if (time >= 30 && time < 45)   timeBonus = 10000;
                    else if (time >= 45 && time < 60)   timeBonus = 5000;
                    else if (time >= 60 && time < 90)   timeBonus = 4000;
                    else if (time >= 90 && time < 120)  timeBonus = 3000;
                    else if (time >= 120 && time < 180) timeBonus = 2000;
                    else if (time >= 180 && time < 240) timeBonus = 1000;
                    else if (time >= 240 && time < 300) timeBonus = 500;
                    else                                timeBonus = 0;
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
                    audio.playSound(SND_BEEP);

                if (timeBonus == 0 && ringBonus == 0) {
                    audio.playSound(SND_COUNT_END);
                    tick = 0;
                }
            }


            tick++;

            if (xShape < xCenter+25) xShape = xCenter+25;
            if (xAct < xCenter+32)  xAct = xCenter+32;

            if (xScore < xCenter-80)      xScore = xCenter-80;
            if (xTimeBonus < xCenter-80)  xTimeBonus = xCenter-80;
            if (xRingBonus < xCenter-80)  xRingBonus = xCenter-80;

            if (xFontTop > xCenter+75) xFontTop = xCenter+75;
            if (xFontBottom > xCenter+58) xFontBottom = xCenter+58;

            scr.drawTextureRect(2, IntRect(191, 77, 56, 56), Vector2f(xShape, 69+yShift));
            scr.drawTextureRect(2, IntRect(2, 121, 24, 8), Vector2f(xAct, 110+yShift));

            switch (act) {
                case 1: scr.drawTextureRect(2, IntRect(34, 113, 7, 24), 
                                            Vector2f(xAct + 32, 94+yShift)); 
                        break;
                case 2: scr.drawTextureRect(2, IntRect(51, 113, 14, 24), 
                                            Vector2f(xAct + 29, 94+yShift)); 
                        break;
                case 3: scr.drawTextureRect(2, IntRect(75, 113, 14, 24), 
                                            Vector2f(xAct + 29, 94+yShift)); 
                        break;                    
            }

            scr.drawText(2, (type == T_TITLE_CARD) ? zone : "SONIC HAS", 
                Vector2f(xFontTop - scr.getTextWidth(2, 
                (type == T_TITLE_CARD) ? zone : "SONIC HAS"), 72+yShift));
            scr.drawText(2, (type == T_TITLE_CARD) ? "ZONE" : "PASSED", 
                Vector2f(xFontBottom - scr.getTextWidth(2, 
                (type == T_TITLE_CARD) ? "ZONE" : "PASSED"), 92+yShift));

            if (type != T_ROUND_CLEAR) return;

            const uint8_t shift = 16;
            scr.drawTextureRect(2, IntRect(231, 60, 16, 16), Vector2f(xScore+29, 100+shift));
            scr.drawTextureRect(2, IntRect(231, 60, 16, 16), Vector2f(xTimeBonus+70, 116+shift));
            scr.drawTextureRect(2, IntRect(231, 60, 16, 16), Vector2f(xRingBonus+70, 132+shift));

            char strbuff[9];
            scr.drawTextureRect(2, IntRect(1, 60, 40, 16), Vector2f(xScore, 100+shift));
            snprintf(strbuff, 9, "%8d", *score);
            scr.drawText(3, strbuff, Vector2f(xScore+96, 100+shift ));
            scr.drawTextureRect(2, IntRect(42, 60, 32, 16), Vector2f(xTimeBonus, 116+shift));
            scr.drawTextureRect(2, IntRect(116, 60, 40, 16), Vector2f(xTimeBonus+40, 116+shift));
            snprintf(strbuff, 9, "%8d", timeBonus);
            scr.drawText(3, strbuff, Vector2f(xTimeBonus+96, 116+16));
            scr.drawTextureRect(2, IntRect(75, 60, 32, 16), Vector2f(xRingBonus, 132+shift));
            scr.drawTextureRect(2, IntRect(116, 60, 40, 16), Vector2f(xRingBonus+40, 132+shift));
            snprintf(strbuff, 9, "%8d", ringBonus);
            scr.drawText(3, strbuff, Vector2f(xRingBonus+96, 132+shift));
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
        Screen& scr;
        Audio& audio;
        const char* zone;
        int act, tick;
        int xCenter;
        int yShift = 0;
        float xFontTop, xFontBottom, xAct, 
            xShape, xScore, xTimeBonus, xRingBonus;
        
        int* score = nullptr;

        int timeBonus = 0;
        int ringBonus = 0;

        Type type;
};