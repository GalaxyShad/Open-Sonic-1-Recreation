#pragma once

#include <list>

#include "terrain-loader.hpp"

#include "entities/_index.hpp"

#include "Camera.h"
#include "Terrain.h"
#include "Bg.h"
#include "LevelInformer.h"

#include "player-sensor.hpp"

#define TILE_HEIGHTS_BUFF_SIZE 4096
#define TILE_ANGLES_BUFF_SIZE  256

enum class GameType {
    SONIC_1,
    SONIC_CD,
    SONIC_2,
    SONIC_3K,
};



class Level {
public:
    Level(
        terrain::Terrain& terrain,
        std::list<Entity*>& entities,
        GameType gameType,
        Screen& scr, 
        IInputMgr& input, 
        Audio& audio,
        std::string& zoneName,
        std::string& zoneNameShort,
        int act,
        v2f playerStartPosition,
        terrain::Store<terrain::Tile>& storeTile
    ) 
        : m_terrain(terrain)
        , m_entities(entities)
        , cam(scr)
        , m_screen(scr)
        , m_input(input)
        , m_audio(audio) 
        , m_gameType(gameType)
        , m_zoneName(zoneName)
        , m_zoneNameShort(zoneNameShort)
        , m_act(act)
        , m_playerStartPosition(playerStartPosition)
        , m_terrainDrawer(cam, m_terrain.getChunkStore(), m_terrain.getLayout(), 255, storeTile)
        , bg(m_terrainDrawer)
        , trn(m_terrain)
        , m_playerSensor(playerStartPosition, v2i(9, 19), v2i(10, 0), m_terrain)
    {}
    void create();
    void free();

    bool isPlayerDied() { return playerDied; }
    bool isEnded() { return end; }

    void update();

    void draw();

    terrain::TerrainDrawer& getTerrainDrawer() { return m_terrainDrawer; } 

private:
    terrain::Terrain &m_terrain;
    std::list<Entity*>& m_entities;

    GameType    m_gameType;
    std::string m_zoneName;
    std::string m_zoneNameShort;
    int         m_act;

    Screen&     m_screen;
    IInputMgr&  m_input;
    Audio&      m_audio;

    v2f m_playerStartPosition;

    terrain::TerrainDrawer m_terrainDrawer;
    Bg bg;

    PlayerSensor m_playerSensor;

    Terrain trn;
    Camera cam;


    std::list<Entity*>::iterator it;
    LevelInformer* lvInformer = nullptr;

    int rings = 0;
    int score = 0;
    int time = 0;
    int tick = 0;
    bool isTimeStopped = false;
    bool isFadeOut = false;
    bool isFadeDeath = false;

    bool playerDied = false;

    float ringFrame;

    bool end;

    uint8_t fade;

private:
    void drawHud();
    void createZoneSpecific();
    void updateLevelSpecific();


};