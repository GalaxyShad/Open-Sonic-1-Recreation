#pragma once

#include <list>

#include "entities/Player.h"
#include "Camera.h"
#include "Terrain.h"
#include "Bg.h"

#include "LevelInformer.h"
#include "entities/Monitor.h"
#include "entities/Spring.h"
#include "entities/Ring.h"
#include "entities/Spikes.h"
#include "entities/LayerSwitcher.h"
#include "entities/EnemiesGHZ.h"
#include "entities/GimmicksGHZ.h"

#define TILE_HEIGHTS_BUFF_SIZE 4096
#define TILE_ANGLES_BUFF_SIZE  256

class Level {
    public:
        Level(Screen& scr, IInputMgr& input, Audio& audio) : 
            cam(scr), scr(scr), input(input), audio(audio) {}
        void create(std::string fZone, std::string fAct, int act);
        void free();
        void restart() { free(); create(sZone, sAct, act); };

        bool isEnded() { return end; }

        void update();
        void draw();
    private:
        Terrain trn;
        Camera cam;

        Screen& scr;
        IInputMgr& input;
        Audio& audio;

		std::list<Entity*> entities;
		std::list<Entity*>::iterator it;
        Bg* bg = nullptr;
        LevelInformer* lvInformer = nullptr;

        uint8_t* verHeights = nullptr;
        uint8_t* horHeights = nullptr;
        uint8_t* angles = nullptr;

        uint8_t* tilesLayout = nullptr;
        uint8_t* tiles16 = nullptr;
        uint16_t* tilesBig = nullptr;

        Vector2f plStartPos = Vector2f(32, 32);

        std::string sZone, sAct;

        int rings = 0;
        int score = 0;
        int time = 0;
        int tick = 0;
        bool isTimeStopped = false;
        bool isFadeOut = false;
        bool isFadeDeath = false;

        float ringFrame;

        bool end;
        int act;

        uint8_t fade;

        void drawHud();

        bool loadTerrainTiles(const char* fnVer, const char* fnHor, const char* fnAngles);
        bool loadTerrainZone(const char* fn16, const char* fnBig);
        bool loadTerrainAct(const char* fnLayout, const char* fnStartPos);

        void loadObjects(const char* filename);

};