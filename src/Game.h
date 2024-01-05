#pragma once

#include "core/_index.hpp"

#include "Level.h"
#include "TitleScreen.h"

class IGame {
    public:
        virtual void init() = 0;
        virtual void update() = 0;
        virtual bool isRunning() = 0;
};

class Game: public IGame {
    public: 
        void init();
        void update();
        bool isRunning();
    private:
        enum MenuTab {T_MAIN, T_SETTINGS, T_PLAY};
        enum ScreenScale {SS_NORMAL, SS_X2, SS_X3, SS_FULL_SCREEN};

        Audio audio;
        Screen scr;
        Keyboard keyboard;
        Level lv = Level(scr, keyboard, audio);
        TitleScreen* ts = nullptr;

        int curLevel = 0;

        int menuPos = 0;
        int menuItemsCount = 0;
        MenuTab menuTab = T_MAIN;

        int wndSize = SS_NORMAL;

        const char* strLevels[17] = {
            "ghz1", "ghz2", "ghz3", 
            "mz1", "mz2", "mz3", 
            "syz1", "syz2", "syz3", 
            "lz1", "lz2", "lz3",
            "slz1", "slz2", "slz3",
            "sbz1", "sbz2",
        };
        const char* strZones[6] = { 
            "GHZ", "MZ", "SYZ", "LZ", "SLZ", "SBZ" 
        };

        void menuKeyHandle(sf::Event::KeyEvent key);
        void menuSettingUpdate();
        void menuPlayUpdate();

        void setWndSize(ScreenScale scale);
};