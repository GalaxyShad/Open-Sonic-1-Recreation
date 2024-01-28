#pragma once

#include <list>

#include "entities/_index.hpp"

#include "Camera.h"
#include "Terrain.h"
#include "Bg.h"
#include "LevelInformer.h"

#define TILE_HEIGHTS_BUFF_SIZE 4096
#define TILE_ANGLES_BUFF_SIZE  256

class Level {
    public:
        Level(Screen& scr, IInputMgr& input, Audio& audio) : 
            cam(scr), scr(scr), input(input), audio(audio),
            trn(*loadTerrainTest()) {}
        void create(std::string fZone, std::string fAct, int act);
        void free();
        void restart() { free(); create(sZone, sAct, act); };

        bool isEnded() { return end; }

        void update();
        void draw();
    private:
        terrain::Terrain* m_terrain;

        std::unique_ptr<terrain::Store<terrain::Tile>> m_storeTiles;
        std::unique_ptr<terrain::Store<terrain::Block>> m_storeBlocks;
        std::unique_ptr<terrain::Store<terrain::Chunk>> m_storeChunks;
        
        std::unique_ptr<terrain::Layout> m_layout;

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

        bool loadTerrainZone(const char* fn16, const char* fnBig);
        bool loadTerrainAct(const char* fnLayout, const char* fnStartPos);

        void loadObjects(const char* filename);

        terrain::Terrain* loadTerrainTest() {
            std::string sCollide   = "content/levels/collide/ghz.bin";
	        std::string sMap256    = "content/levels/map256/GHZ.bin";
	        std::string sLayout    = "content/levels/layout/ghz1.bin";

            terrain::TerrainLoaderSonic1 loader(terrain::TerrainLoaderSonic1FilePaths {
                .angles = "content/levels/collide/Angle Map.bin",
                .verticalHeights = "content/levels/collide/Collision Array (Normal).bin",
                .horizontalHeights = "content/levels/collide/Collision Array (Rotated).bin",

                .blocks = sCollide.c_str(),
                .chunks = sMap256.c_str(),

                .layout = sLayout.c_str()
            });

            m_storeTiles = loader.loadTiles();
            m_storeBlocks = loader.loadBlocks(*m_storeTiles.get());
            m_storeChunks = loader.loadChunks(*m_storeBlocks.get());
            
            m_layout = loader.loadLayout(*m_storeChunks.get());

            return new terrain::Terrain(*m_layout.get());
        }

};