#include "Game.h"

#include "core/Geometry.h"

#include <SFML/Graphics.hpp>
#include <iostream>

#define TEX_OBJECTS 0
#define TEX_GHZ_GIMM 1
#define TEX_HUD 2
#define TEX_LEVEL_TILES 255

#define SCREEN_WIDTH    426
#define SCREEN_HEIGHT   240

#define FRAME_LOCK 60

void Game::init() {
    scr.init(Size(SCREEN_WIDTH, SCREEN_HEIGHT), FRAME_LOCK);

    std::cout << "Loading textures..." << std::endl;

    scr.loadTextureFromFile("content/textures/texGhzObj.png", TEX_GHZ_GIMM, framesGhzGim, 8);
    scr.loadTextureFromFile("content/textures/texObj.png", TEX_OBJECTS, framesObjects, 139);
    scr.loadTextureFromFile("content/textures/texHud.png", TEX_HUD);

    scr.addFont(0, "abcdefghijklmnopqrstuvwxyz1234567890*.:-=!?<>", 1, TEX_HUD, IntRect(2, 150, 8, 7));
    scr.addFont(1, "abcdefghijklmnopqrstuvwxyz1234567890$.-=>", 1, TEX_HUD, IntRect(1, 35, 8, 7), 1);
    uint16_t f2widths[] = {15, 15, 15, 15, 14, 14, 16, 15, 6, 16, 15, 16, 15, 16, 15, 15, 14, 14, 15, 15, 15, 3};
    scr.addFont(2, "ABCDEFGHIKLMNOPRSTUYZ.", 1, TEX_HUD, IntRect(1, 1, 15, 16), 1, f2widths);
    scr.addFont(3, "0123456789:", 0, TEX_HUD, IntRect(1, 43, 8, 16), 1);

    std::cout << "Textures loaded succesuffuly!" << std::endl;

    audio.loadSound(SND_PATH SND_JUMP_FILE, SND_JUMP);
    audio.loadSound(SND_PATH SND_HURT_FILE, SND_HURT);
    audio.loadSound(SND_PATH SND_SKID_FILE, SND_SKID);
    audio.loadSound(SND_PATH SND_SPIKES_FILE, SND_SPIKES);
    audio.loadSound(SND_PATH SND_SHIELD_FILE, SND_SHIELD);
    audio.loadSound(SND_PATH SND_RING_FILE, SND_RING);
    audio.loadSound(SND_PATH SND_DASH_FILE, SND_DASH);
    audio.loadSound(SND_PATH SND_ROLL_FILE, SND_ROLL);
    audio.loadSound(SND_PATH SND_DESTROY_FILE, SND_DESTROY);
    audio.loadSound(SND_PATH SND_RING_LOSS_FILE, SND_RING_LOSS);
    audio.loadSound(SND_PATH SND_SPRING_FILE, SND_SPRING);

    audio.loadSound(SND_PATH SND_BEEP_FILE, SND_BEEP);
    audio.loadSound(SND_PATH SND_COUNT_END_FILE, SND_COUNT_END);
    audio.loadSound(SND_PATH SND_END_TABLE_FILE, SND_END_TABLE);
    audio.loadSound(SND_PATH SND_PLT_CRUSH_FILE, SND_PLT_CRUSH);

    audio.addMusic(0, "content/music/Title.ogg");
    audio.addMusic(1, "content/music/StageClear.ogg");
    audio.addMusic(2, "content/music/GHZ.ogg");

    audio.playMusic(0);
    ts = new TitleScreen(scr);
}

void Game::update() {
    if (!isRunning()) return;

    int prevLevel = curLevel;

    sf::RenderWindow& wnd = scr.getSfmlWindow();
    sf::Event event;
    while (wnd.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            wnd.close();
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::R) {
                lv.restart();
            } else if (event.key.code == sf::Keyboard::F) {
                wndSize++;
                if (wndSize > 3) 
                    wndSize = 0;

                setWndSize((ScreenScale)wndSize);
            } else if (event.key.code == sf::Keyboard::Q) {
                curLevel--;
            } else if (event.key.code == sf::Keyboard::E) {
                curLevel++;
            } 

            if (ts) menuKeyHandle(event.key);
        }
    }

    if (curLevel < 0)
        curLevel = 0;
    if (curLevel > 16)
        curLevel = 0;

    if (!ts) {
        if (curLevel != prevLevel || lv.isEnded()) {
            if (lv.isEnded()) curLevel++;
            lv.free();
            lv.create(strZones[curLevel / 3], strLevels[curLevel], curLevel % 3 + 1);
        }
    }

    audio.update();
    if (!ts) lv.update();

    scr.clear();

    if (ts) ts->draw();
    else    lv.draw();

    scr.render();
}

bool Game::isRunning() {
    return (bool)(scr.getSfmlWindow().isOpen());
}

void Game::menuKeyHandle(sf::Event::KeyEvent key) {
    if (key.code == sf::Keyboard::Enter) {
        if (!ts->isStartPressed()) {
            ts->startPress();
            menuTab = T_MAIN;
        } else {
            switch (menuTab) {
                case T_MAIN: 
                    switch (menuPos) {
                        case 0: menuTab = T_PLAY; break;
                        case 1: menuTab = T_SETTINGS; break;
                        case 2: scr.getSfmlWindow().close(); return; break;
                    }
                    break;
                case T_SETTINGS:
                    if (menuPos == 3) menuTab = T_MAIN;
                    break;
                case T_PLAY:
                    if (menuPos == 1) {
                        menuTab = T_MAIN;
                    } else {
                        lv.create(strZones[curLevel / 3], 
                            strLevels[curLevel], curLevel % 3 + 1); 
                        delete ts;
                        ts = nullptr;
                        return;
                    }
                    break;
            }
        }

        menuPos = 0;
        ts->setCursorPos(menuPos);

        switch (menuTab) {
            case T_MAIN: {
                const char* mitems[] = {"play", "options", "exit"};
                menuItemsCount = 3;
                ts->setItems(mitems, 3);
                break;
            }
            case T_SETTINGS: {
                menuSettingUpdate();
                break;
            }
            case T_PLAY: {
                menuPlayUpdate();
                break;
            }
        }

        return;
    } 

    if ((key.code == sf::Keyboard::Left) || (key.code == sf::Keyboard::Right)) {
        switch (menuTab) {
            case T_MAIN:
                break;
            case T_SETTINGS:
                switch (menuPos) {
                    case 0: 
                    case 1: {
                        int vol;
                        if (menuPos == 0) vol = audio.getVolumeMusic();
                        if (menuPos == 1) vol = audio.getVolumeSound();

                        if (key.code == sf::Keyboard::Left) vol--;
                        if (key.code == sf::Keyboard::Right) vol++;

                        if (vol < 0.f) vol = 0.f;
                        if (vol > 100.f) vol = 100.f;

                        if (menuPos == 0) {
                            audio.setVolumeMusic(vol);
                            audio.playMusic(0);
                        }
                        if (menuPos == 1) {
                            audio.setVolumeSound(vol);
                            audio.playSound(SND_RING);
                        }
                        break;
                    }
                    case 2: {
                        if (key.code == sf::Keyboard::Left)  wndSize--;
                        if (key.code == sf::Keyboard::Right) wndSize++;

                        if (wndSize > 3) wndSize = 0;
                        else if (wndSize < 0) wndSize = 3;

                        setWndSize((ScreenScale)wndSize);
                        break;
                    }
                }
                menuSettingUpdate();
                break;
            case T_PLAY:
                if (menuPos != 0) break;

                if (key.code == sf::Keyboard::Left)  curLevel--;
                if (key.code == sf::Keyboard::Right) curLevel++;

                if (curLevel < 0)  curLevel = 0;
                if (curLevel > 16) curLevel = 0;

                menuPlayUpdate();
                break;
        }

        audio.playSound(SND_BEEP);
        return;
    }
  
    if (key.code == sf::Keyboard::Up) {
        menuPos--;
        audio.playSound(SND_BEEP);
    } else if (key.code == sf::Keyboard::Down) {
        menuPos++;
        audio.playSound(SND_BEEP);
    }

    if (menuPos >= menuItemsCount)
        menuPos = 0;
    else if (menuPos < 0)
        menuPos = menuItemsCount-1;

    ts->setCursorPos(menuPos);
}

void Game::menuSettingUpdate() {
    char* buff[4];
    int volSnd = (int)audio.getVolumeSound();
    int volMus = (int)audio.getVolumeMusic();

    buff[0] = new char[24]; 
    buff[1] = new char[24]; 
    buff[2] = new char[24]; 
    buff[3] = new char[24]; 
    snprintf(buff[0], 24, "music volume: %3d", volMus);
    snprintf(buff[1], 24, "sound volume: %3d", volSnd);
    (wndSize < 3) ? snprintf(buff[2], 24, "screen size:   x%d", wndSize+1) : 
        snprintf(buff[2], 24, "screen size: full");
    snprintf(buff[3], 24, "back");
    menuItemsCount = 4;
    ts->setItems((const char**)buff, 4);

    delete buff[0];
    delete buff[1];
    delete buff[2];
    delete buff[3];
}

void Game::menuPlayUpdate() {
    char* buff[2];
    buff[0] = new char[24]; 
    buff[1] = new char[24]; 
    snprintf(buff[0], 24, "level: %s", strLevels[curLevel]);
    snprintf(buff[1], 24, "back");
    menuItemsCount = 2;
    ts->setItems((const char**)buff, 2);

    delete buff[0];
    delete buff[1];
}

void Game::setWndSize(ScreenScale scale) {
    sf::RenderWindow& wnd = scr.getSfmlWindow();
    switch(scale) {
        case SS_NORMAL:
            wnd.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), 
                "Title");
            break;
        case SS_X2:
            wnd.create(sf::VideoMode(SCREEN_WIDTH*2, SCREEN_HEIGHT*2), 
                "Title");
            break;
        case SS_X3:
            wnd.create(sf::VideoMode(SCREEN_WIDTH*3, SCREEN_HEIGHT*3), 
                "Title");
            break;
        case SS_FULL_SCREEN:
            wnd.create(sf::VideoMode::getDesktopMode(), "Title", sf::Style::Fullscreen);
            break;
    }

    wnd.setFramerateLimit(FRAME_LOCK);
    wnd.setView(sf::View(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)));
}