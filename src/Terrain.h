#pragma once

#include <cstdint>
#include <cmath>
 
#include "core/Geometry.h"
#include "entities/Entity.h"
#include "Camera.h"

enum TileType { TILE_EMPTY, TILE_TOP, TILE_LBR, TILE_FULL };

struct Block {
    uint8_t height;
    uint8_t type;

    Block(uint8_t _height, uint8_t _type):
        height(_height), type(_type) {}
};

struct Tile {
    uint8_t verHeight[16], horHeight[16];
    Vector2i pos;
    TileType type;
    float angle = 0;
    uint8_t height = 0;

    uint8_t getHeight(Vector2i _pos, float ang, bool collideLBR=true) {
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
        void create(const uint8_t* verHeights, const uint8_t* horHeights, const uint8_t* angles,
                    const uint8_t* blocks, const uint16_t* blockMapping, const uint8_t* lvLayout, uint8_t _lvTexture);

        void createLayeringObjs(std::list<Entity*>& entList);

        uint8_t getChunk(Vector2i pos);
        uint16_t getBlock(Vector2i pos);
        
        int getTileVerHeight(Vector2i pos);
        int getTileHorHeight(Vector2i pos);
        float getTileAngle(Vector2i pos);
        TileType getTileType(Vector2i pos);  
        Tile getTile(Vector2i pos);

        Size getSize()  { return lvSize; }
        void setLayer(uint8_t _layer) { layer = _layer; }

        void draw(Camera& cam);
        void drawChunk(Camera& cam, uint16_t chunkId, Vector2f pos);
        void drawChunkPart(Camera& cam, uint16_t chunkId, Vector2f pos, IntRect rect);
    private:
        const uint8_t* verHeights = nullptr;
        const uint8_t* horHeights = nullptr;
        const uint8_t* angles = nullptr;

        const uint8_t* blocksPtr = nullptr;
        const uint8_t* lvLayoutPtr = nullptr ;
        const uint16_t* blockMappingPtr = nullptr;

        const uint16_t chunkSide = 256;

        uint8_t lvTexture = 0;
        uint8_t layer = 0;
        Size lvSize;
};
