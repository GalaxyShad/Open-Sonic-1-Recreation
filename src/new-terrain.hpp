#pragma once

#include <cstdint>
#include <cmath>
#include <vector> 
#include <fstream>
#include <assert.h>

#include "core/Geometry.h"
#include "hex-angle.hpp"

namespace terrain {

template <class T>
class Store {
public:
    Store() : m_size(0) {}

    Store(const T* buffer, size_t size) : m_size(size) 
    {
        for (int i = 0; i < size; i++) {
            m_store.push_back(buffer[i]);
        }
    }

    inline void add(T& el) { m_store.push_back(el); }
    inline size_t count() const { return m_store.size(); }
    
    inline T& get(int index) { return m_store[index]; }

private:
    std::vector<T> m_store;
    size_t m_size;
};

const int TERRAIN_TILE_SIZE = 16;

struct Tile {
    int id;
    HexAngle angle;
    int8_t heightsVertical[TERRAIN_TILE_SIZE];
    int8_t heightsHorizontal[TERRAIN_TILE_SIZE];

    Tile flip(bool horizontal, bool vertical) const {
        Tile flipped = { };

        for (int i = 0; i < TERRAIN_TILE_SIZE; i++) {
            int ri = (TERRAIN_TILE_SIZE - 1 - i); // reverse i

            flipped.heightsHorizontal[i] = heightsHorizontal[vertical ? ri : i];
            flipped.heightsVertical[i]   = heightsVertical[horizontal ? ri : i]; 
        }

        flipped.id = id;
        flipped.angle = angle;

        if (angle.isRotatable()) {
            flipped.angle.hex = (vertical)   ? (128 - flipped.angle.hex) : flipped.angle.hex;
            flipped.angle.hex = (horizontal) ? (256 - flipped.angle.hex) : flipped.angle.hex;
        }

        return flipped;
    }

    Tile flipVertical() const { return flip(false, true); } 

    Tile flipHorizontal() const { return flip(true, false); }
};

enum class BlockSolidity { 
    EMPTY                   = 0b00, 
    ONLY_TOP                = 0b01, 
    ONLY_LEFT_BOTTOM_RIGHT  = 0b10, 
    FULL                    = 0b11 
};

struct Block {
    int id;
    Tile& solid;

    Block(int id, Tile& solid) : id(id), solid(solid) {}
};

struct ChunkBlock {
    BlockSolidity solidityNormalLayer;
    BlockSolidity solidityAlternateLayer;    // S2+
    bool xFlip;  
    bool yFlip;  
    Tile tile;
    int blockId;

    static ChunkBlock fromSonic1(uint16_t rawBlockS1, Store<Block>& blockStore) {
        // 0SSY X0II IIII IIII
        //              0SSYX0IIIIIIIIII
        uint16_t ms = 0b0110000000000000;
        uint16_t my = 0b0001000000000000;
        uint16_t mx = 0b0000100000000000;
        uint16_t mi = 0b0000001111111111;

        ChunkBlock chunkBlock = {};

        chunkBlock.solidityNormalLayer    = (BlockSolidity)((rawBlockS1 & ms) >> 13);
        chunkBlock.solidityAlternateLayer = BlockSolidity::EMPTY;
        chunkBlock.yFlip   = (rawBlockS1 & my) >> 12;
        chunkBlock.xFlip   = (rawBlockS1 & mx) >> 11;
        chunkBlock.blockId = (rawBlockS1 & mi); 

        // [!TEMP] fix for case when there too big values 
        if (chunkBlock.blockId < blockStore.count()) {
            Block block        = blockStore.get(chunkBlock.blockId);
            chunkBlock.tile    = block.solid.flip(chunkBlock.xFlip, chunkBlock.yFlip);
        }

        return chunkBlock;
    }

    static ChunkBlock fromSonic23K(uint16_t rawblockS23K, Store<Block>& blockStore) {
        // SSTT YXII IIII IIII
        //              SSTTYXIIIIIIIIII
        uint16_t ms = 0b1100000000000000;
        uint16_t mt = 0b0011000000000000;
        uint16_t my = 0b0000100000000000;
        uint16_t mx = 0b0000010000000000;
        uint16_t mi = 0b0000001111111111;

        ChunkBlock chunkBlock = {};

        chunkBlock.solidityNormalLayer    = (BlockSolidity)((rawblockS23K & ms) >> 14);
        chunkBlock.solidityAlternateLayer = (BlockSolidity)((rawblockS23K & mt) >> 12);
        chunkBlock.yFlip   = (rawblockS23K & my) >> 11;
        chunkBlock.xFlip   = (rawblockS23K & mx) >> 10;
        chunkBlock.blockId = (rawblockS23K & mi);

        Block block        = blockStore.get(chunkBlock.blockId);
        chunkBlock.tile    = block.solid.flip(chunkBlock.xFlip, chunkBlock.yFlip);

        return chunkBlock;
    }
};

enum class ChunkFormat { 
    SONIC_1_CD,
    SONIC_2_3_K
};


struct Chunk {
public:
    Chunk(const uint8_t* chunkBuffer, ChunkFormat chunkFormat, Store<Block>& blockStore) {
        m_radius = (chunkFormat == ChunkFormat::SONIC_1_CD) ? 16 : 8;

        for (int i = 0; i < m_radius*m_radius; i++) {
            uint16_t rawBlock = ((uint16_t)chunkBuffer[i*2] << 8) | chunkBuffer[i*2+1];

            m_blockList.push_back(
                (chunkFormat == ChunkFormat::SONIC_1_CD)
                    ? ChunkBlock::fromSonic1(rawBlock, blockStore)
                    : ChunkBlock::fromSonic23K(rawBlock, blockStore)
            );
        }
    }

    int getRadius() const {
        return m_radius;
    }

    int getRadiusPixels() const {
        return m_radius * TERRAIN_TILE_SIZE;
    }

    ChunkBlock getBlock(int index) const {
        assert(index < m_radius*m_radius);

        return m_blockList[index];
    }

    ChunkBlock getBlock(int x, int y) const {
        assert(x >= 0 && y >= 0 && x < m_radius && y < m_radius);

        return m_blockList[y * m_radius + x];
    }
private:
    std::vector<ChunkBlock> m_blockList;
    int m_radius;
};

enum class TerrainLayer {
    NORMAL,
    ALTERNATE
};

class Layout {
private:
    const int FLAGGED_MASK = 0x80;

public:
    Layout(const uint8_t* layoutBuffer, size_t width, size_t height, size_t chunkRadius, Store<Chunk>& chunkStore, bool useFlaggedTiles = false) 
        : m_chunkStore(chunkStore)
        , m_width(width)
        , m_height(height)
        , m_usingFlaggedLayering(useFlaggedTiles)
        , m_chunkRadius(chunkRadius)
    {
        int size = m_width * m_height;

        m_layoutIndexes = new uint8_t[size];

        memcpy(m_layoutIndexes, layoutBuffer, size);
    }

    ~Layout() {
        delete [] m_layoutIndexes;
    }

    static Layout* createFromSonic1(const uint8_t* buffer, Store<Chunk>& chunkStore) {
        int width  = buffer[0] + 1;
        int height = buffer[1] + 1;

        return new Layout(buffer+2, width, height, 16, chunkStore, true);
    }

    static Layout* createFromSonic3(const uint8_t* buffer, Store<Chunk>& chunkStore) {
        int foregroundWidth  = (buffer[0] << 8) | buffer[1];
        int backgroundWidth  = (buffer[2] << 8) | buffer[3];

        int foregroundHeight = (buffer[4] << 8) | buffer[5];
        int backgroundHeight = (buffer[6] << 8) | buffer[7];

        Layout* result = nullptr;

        auto     foregroundSize = foregroundWidth * foregroundHeight;
        uint8_t* foreground     = new uint8_t[foregroundSize];

        for (int i = 0; i < foregroundSize; i++) {
            foreground[i] = buffer[0x88 + i];
        }

        result = new Layout(foreground, foregroundWidth, foregroundHeight, 8, chunkStore, false);

        delete [] foreground;

        return result;
    }

    int getChunkId(int index) const {
        assert(index >= 0 && index < m_width * m_height);
        
        return m_layoutIndexes[index];
    }

    int getChunkId(int x, int y) const {
        return getChunkId(y * m_width + x);
    }

    Chunk& getChunk(int index, TerrainLayer layer = TerrainLayer::NORMAL) const {
        assert(index < m_width * m_height);
        
        int chunkId = m_layoutIndexes[index];

        if (m_usingFlaggedLayering) {
            bool isFlagged = chunkId & FLAGGED_MASK;
            
            chunkId = (chunkId & (~FLAGGED_MASK)) + (isFlagged && (int)layer);
        }

        return m_chunkStore.get(chunkId);
    }

    Chunk& getChunk(int x, int y, TerrainLayer layer = TerrainLayer::NORMAL) const {
        assert((x < m_width) && (y < m_height));

        return getChunk(byXY(x, y), layer);
    }

    int getChunksRadius() const { return m_chunkRadius; }

    int getChunksRadiusPixels() const { return m_chunkRadius * TERRAIN_TILE_SIZE; }

    int getWidth() const { return m_width; }

    int getHeight() const { return m_height; }

    bool isLayeringChunk(int x, int y) const {
        assert((x < m_width) && (y < m_height));

        return (m_layoutIndexes[y * m_width + x] & FLAGGED_MASK) && m_usingFlaggedLayering;
    }

    Store<Chunk>& getChunkStore() const { return m_chunkStore; }

private:
    Store<Chunk>& m_chunkStore; 
    uint8_t* m_layoutIndexes;
    int m_width;
    int m_height;
    int m_chunkRadius;
    bool m_usingFlaggedLayering = false;

private:
    int byXY(int x, int y) const {
        return y * m_width + x;
    }
};

class Terrain {
public:
    Terrain(Layout& layout) : m_layout(layout) {}

    Chunk& getChunk(float x, float y, TerrainLayer layer = TerrainLayer::NORMAL) const {
        int ix = (int)(x / m_layout.getChunksRadiusPixels());
        int iy = (int)(y / m_layout.getChunksRadiusPixels());

        if (ix < 0 || iy < 0 || ix >= m_layout.getWidth() || iy >= m_layout.getHeight()) {
            return getChunkStore().get(0);
        }
        
        return m_layout.getChunk(ix, iy, layer);
    }

    ChunkBlock getBlock(float x, float y, TerrainLayer layer = TerrainLayer::NORMAL) const {
        auto chunk = getChunk(x, y, layer);

        int ix = (int)(x / TERRAIN_TILE_SIZE) % (m_layout.getChunksRadius());
        int iy = (int)(y / TERRAIN_TILE_SIZE) % (m_layout.getChunksRadius());

        if (ix < 0 || iy < 0)
            return getChunkStore().get(0).getBlock(0, 0);

        ChunkBlock block = chunk.getBlock(ix, iy);

        return block;
    }

    static int getVerticalHeightInTile(float x, const Tile& tile) {
        int ix = ((int)x) % TERRAIN_TILE_SIZE;

        return tile.heightsVertical[ix];
    }

    static int getHorizontalHeightInTile(float y, const Tile& tile) {
        int iy = ((int)y) % TERRAIN_TILE_SIZE;

        return tile.heightsHorizontal[iy];
    }

    Store<Chunk>& getChunkStore() const { return m_layout.getChunkStore(); } 
    Layout& getLayout() const { return m_layout; }


private:
    Layout& m_layout;

};



}
