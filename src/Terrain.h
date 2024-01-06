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

struct HexAngle {
    uint8_t hex;

    inline float degrees() { 
        return ((256.0f - hex) / 256.0f) * 360.0f; 
    }

    static HexAngle fromDegrees(float degAng) { 
        return HexAngle { (uint8_t)(((360 - degAng) / 360) * 256) };
    }
};

struct TerrainTile {
    HexAngle angle;
    uint8_t heightsVertical[16];
    uint8_t heightsHorizontal[16];

    TerrainTile flipVertical() {
        TerrainTile flipped = { 0 };

        for (int i = 0; i < 16; i++) {
            flipped.heightsHorizontal[i] = heightsHorizontal[15 - i];
            flipped.heightsVertical[i] = heightsVertical[i]; 
        }
        flipped.angle.hex = 0x7F - angle.hex;

        return flipped;
    } 

    TerrainTile flipHorizontal() {
        TerrainTile flipped = { 0 };

        for (int i = 0; i < 16; i++) {
            flipped.heightsHorizontal[i] = heightsHorizontal[i];
            flipped.heightsVertical[i] = heightsVertical[15 - i]; 
        }
        flipped.angle.hex = 0xFF - angle.hex;

        return flipped;
    }
};

class TileStore {
public:
    TileStore(const TerrainTile* buffer, size_t size) 
        : m_store(buffer), m_size(size) {}

    inline size_t count() { return m_size; }
    
    inline TerrainTile getTile(int index) { return m_store[index]; }

private:
    const TerrainTile* m_store;
    size_t m_size;
};

class Terrain {
    public:
        Terrain(TileStore& tileStore) : m_tileStore(tileStore) {}

        void create(
            const uint8_t* blocks, const uint16_t* blockMapping, 
            const uint8_t* lvLayout, uint8_t _lvTexture);

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
        TileStore& m_tileStore;

        const uint8_t* blocksPtr = nullptr;
        const uint8_t* lvLayoutPtr = nullptr ;
        const uint16_t* blockMappingPtr = nullptr;

        const uint16_t chunkSide = 256;

        uint8_t lvTexture = 0;
        uint8_t layer = 0;
        Size lvSize;
};
