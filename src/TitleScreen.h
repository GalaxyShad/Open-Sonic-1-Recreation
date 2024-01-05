#pragma once

#include "core/InputMgr.h"
#include "Camera.h"
#include "Bg.h"

const Frame framesTitleSonic[] = {
    Frame(257, 0, 72, 88),
    Frame(330, 8, 80, 80),
    Frame(411, 8, 80, 80),
    Frame(492, 0, 80, 88),
    Frame(257, 97, 72, 88),
    Frame(330, 89, 88, 96),
    Frame(419, 89, 88, 96),
    Frame(508, 89, 88, 96),
};

class TitleScreen {
    public:
        TitleScreen(Screen& scr) : scr(scr), cam(scr)  {
            scr.loadTextureFromFile("content/textures/texTitle.png", 254, framesTitleSonic, 8);
            scr.loadTextureFromFile("content/textures/texTitleBg.png", 253);

            cam.create(Vector2f(0, 0), scr.getSize());

            anim.create(254);
            anim.set(0, 0, 0.25);

            pos = Vector2f(scr.getSize().width / 2, scr.getSize().height / 2);

            tick = 0;
            yAnimShift = 8;
            needToPressStart = true;

            menuItemsCount = 0;
            menuCursor = 0;
        }
        ~TitleScreen() {
            scr.freeTexture(254);
        }

        void startPress() { needToPressStart = false; }
        bool isStartPressed() { return !needToPressStart; }

        void setItems(const char** items, uint16_t itemsCount) { 
            menuItemsCount = itemsCount;

            for (int i = 0; i < menuItemsCount; i++) {
                strcpy(menuItems[i], items[i]);
            }  
            
        }
        void setCursorPos(uint16_t pos) { menuCursor = (pos < menuItemsCount) ? pos : 0; }

        void draw() {
            scr.drawTextureRect(253, IntRect(0, 0, 427, 240));

            yAnimShift -= 6;
            if (yAnimShift < -48) {
                yAnimShift = -48;
                if (anim.getLastFrame() != 5 && anim.getLastFrame() != 7) 
                    anim.set(0, 5, 0.125);
            }

            tick++;

            if (anim.getLastFrame() == 5 && anim.getCurFrame() >= 5.75)
                anim.set(6, 7, 0.125);

            anim.tick();
            scr.drawTextureRect(254, IntRect(0, 0, 256, 82), Vector2f(pos.x-128, pos.y - 82));
            cam.draw(anim, Vector2f(pos.x, pos.y + yAnimShift));
            scr.drawTextureRect(254, IntRect(0, 83, 256, 74), Vector2f(pos.x-128, pos.y - 12));

            if (needToPressStart && tick % 40 < 20)
                scr.drawTextureRect(254, IntRect(56, 158, 144, 8), Vector2f(pos.x-72, pos.y + 64));
            else if (!needToPressStart) {
                for (int i = 0; i < menuItemsCount; i++) {
                    uint16_t w = scr.getTextWidth(0, menuItems[i]);

                    if (menuCursor == i) {
                        scr.drawText(0, ">",  Vector2f(pos.x - w / 2 - 16, pos.y + 64 + 12*i));
                        scr.drawText(0, "<",  Vector2f(pos.x + w / 2 + 5, pos.y + 64 + 12*i));
                    }
                        
                    scr.drawText(0, menuItems[i],  Vector2f(pos.x-w / 2, pos.y + 64 + 12*i));
                }
            }
        }
    private:
        Screen& scr;
        Camera cam;
        AnimMgr anim;
        Vector2f pos;

        int tick;
        float yAnimShift;

        bool needToPressStart;

        char menuItems[4][32] = {0};
        uint16_t menuItemsCount;
        uint16_t menuCursor;
};