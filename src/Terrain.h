#pragma once

#include <cstdint>
#include <cmath>
 
#include "core/Geometry.h"
#include "entities/Entity.h"
#include "Camera.h"

#include "new-terrain.hpp"

enum TileType { TILE_EMPTY, TILE_TOP, TILE_LBR, TILE_FULL };

struct Block {
    uint8_t height;
    uint8_t type;

    Block(uint8_t _height, uint8_t _type):
        height(_height), type(_type) {}
};

struct Tile {
    uint8_t verHeight[16], horHeight[16];
    v2i pos;
    TileType type;
    float angle = 0;
    uint8_t height = 0;

    uint8_t getHeight(v2i _pos, float ang, bool collideLBR=true) {
        if (round(ang / 90) == 0 || round(ang / 90) == 2) {
            if ((type == TileType::TILE_FULL) || 
                (type == TileType::TILE_TOP && std::round(ang / 90) == 0) ||
                (type == TileType::TILE_LBR && std::round(ang / 90) == 2))
                    height = verHeight[int(_pos.x - ((_pos.x / 16) * 16))];
        } else {
            if (type == TileType::TILE_FULL || (type == TileType::TILE_LBR && collideLBR))
                height = horHeight[int(_pos.y - ((_pos.y / 16) * 16))];
        }
        
        if (height <= 16)
            return height;
        else
            return 256 - height;

        return 0;
    };

};

class Terrain {
    public:
        Terrain(terrain::Terrain& terrain) 
            : m_terrain(terrain) 
        {}

        void createLayeringObjs(std::list<Entity*>& entList);

        int getTileVerHeight(v2i pos);
        int getTileHorHeight(v2i pos);
        float getTileAngle(v2i pos);
        TileType getTileType(v2i pos);  
        Tile getTile(v2i pos);

        Size getSize()  { return Size(m_terrain.getLayout().getWidth(), m_terrain.getLayout().getHeight()); }
        void setLayer(uint8_t _layer) { layer = _layer; }
    private:
        terrain::Terrain& m_terrain;
        uint8_t layer = 0;
};
