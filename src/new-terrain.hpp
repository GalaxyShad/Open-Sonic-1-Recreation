#pragma once

#include <cstdint>
#include <cmath>
#include <vector> 

#include "core/Geometry.h"
#include "hex-angle.hpp"

namespace terrain {

template <class T>
class Store {
public:
    Store() : m_size(0) {}

    Store(const T** buffer, size_t size) : m_size(size) 
    {
        for (int i = 0; i < size; i++) {
            m_store.push_back(buffer[i]);
        }
    }

    inline void add(T* el) { m_store.push_back(el); }
    inline size_t count() { return m_store.size(); }
    
    inline T& get(int index) { return *m_store[index]; }

private:
    std::vector<T*> m_store;
    size_t m_size;
};

const int TERRAIN_TILE_SIZE = 16;

struct Tile {
    int id;
    HexAngle angle;
    uint8_t heightsVertical[TERRAIN_TILE_SIZE];
    uint8_t heightsHorizontal[TERRAIN_TILE_SIZE];

    Tile flip(bool horizontal, bool vertical) {
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

    Tile flipVertical() { return flip(false, true); } 

    Tile flipHorizontal() { return flip(true, false); }
};

enum class BlockSolidity { 
    EMPTY                   = 0b00, 
    ONLY_TOP                = 0b01, 
    ONLY_LEFT_BOTTOM_RIGHT  = 0b10, 
    FULL                    = 0b11 
};

// L before struct name means Legacy
struct LBlockSonic1 {
    // 0SSY X0II IIII IIII
    uint16_t                        : 1;  // 0
    uint16_t solidity               : 2;  // SS
    uint16_t yFlip                  : 1;  // Y
    uint16_t xFlip                  : 1;  // X
    uint16_t                        : 1;  // 0
    uint16_t blockIndex             : 10; // II IIII IIII
};

struct LBlockSonic23K {
    // SSTT YXII IIII IIII
    uint16_t solidityAlternateLayer : 2;  // SS
    uint16_t solidityNormalLayer    : 2;  // TT
    uint16_t yFlip                  : 1;  // Y
    uint16_t xFlip                  : 1;  // X
    uint16_t blockIndex             : 10; // II IIII IIII
};

struct Block {
    int id;
    Tile& solid;
};

struct ChunkBlock {
    BlockSolidity solidityNormalLayer;
    BlockSolidity solidityAlternateLayer;    // S2+
    bool xFlip;  
    bool yFlip;  
    Tile tile;
    int blockId;

    static ChunkBlock* fromSonic1(uint16_t rawBlockS1, Store<Block>& blockStore) {
        // 0SSY X0II IIII IIII
        //              0SSYX0IIIIIIIIII
        uint16_t ms = 0b0110000000000000;
        uint16_t my = 0b0001000000000000;
        uint16_t mx = 0b0000100000000000;
        uint16_t mi = 0b0000001111111111;

        auto chunkBlock = new ChunkBlock();

        chunkBlock->solidityNormalLayer    = (BlockSolidity)((rawBlockS1 & ms) >> 13);
        chunkBlock->solidityAlternateLayer = BlockSolidity::EMPTY;
        chunkBlock->yFlip   = (rawBlockS1 & my) >> 12;
        chunkBlock->xFlip   = (rawBlockS1 & mx) >> 11;
        chunkBlock->blockId = rawBlockS1 & mi;

        if (chunkBlock->blockId < blockStore.count()) { 
            Block block = blockStore.get(chunkBlock->blockId);

            chunkBlock->tile = Tile(block.solid.flip(chunkBlock->xFlip, chunkBlock->yFlip));
        }   else {
            std::printf("%04X %X/%X x=%d y=%d\n", rawBlockS1, chunkBlock->blockId, blockStore.count(),
            chunkBlock->xFlip, chunkBlock->yFlip);
        }

        return chunkBlock;
    }

    static ChunkBlock* fromSonic23K(LBlockSonic23K blockS23K, Store<Block>& blockStore) {
        Block block = blockStore.get(blockS23K.blockIndex);

        return new ChunkBlock {
            .solidityNormalLayer = (BlockSolidity)blockS23K.solidityNormalLayer,
            .solidityAlternateLayer = (BlockSolidity)blockS23K.solidityAlternateLayer,
            .xFlip = (bool)blockS23K.xFlip,
            .yFlip = (bool)blockS23K.yFlip,
            .tile = block.solid.flip(blockS23K.xFlip, blockS23K.yFlip),
            .blockId = block.id
        };
    }
};

// used in Sonic 1/CD
struct LChunkSonic256 {
    uint16_t blocks[16*16];
};

// used in Sonic 2/3/3&K
struct LChunkSonic128 {
    uint16_t blocks[8*8];
};

class Chunk {
public:
    Chunk(uint8_t* blocks, int blockRadius, Store<Block>& blockStore) {
        m_radius = blockRadius;

        for (int i = 0; i < m_radius*m_radius; i++) {
            uint16_t rawBlockS1 = ((uint16_t)blocks[i*2] << 8) | blocks[i*2+1];

            m_blockList.push_back(ChunkBlock::fromSonic1(rawBlockS1, blockStore));
        }
    }

    const ChunkBlock& getBlock(int index) const {
        return *m_blockList[index];
    }

    const ChunkBlock& getBlock(int x, int y) const {
        return *m_blockList[y * m_radius + x];
    }
private:
    std::vector<ChunkBlock*> m_blockList;
    size_t m_radius;
};

enum class TerrainLayer {
    NORMAL,
    ALTERNATE
};

class Layout {
public:
    Layout(
        const uint8_t* layoutBuffer,
        uint8_t width, uint8_t height,  
        Store<Chunk>& chunkStore,
        int chunkRadius = 8
    ) 
    : 
        m_width(width),
        m_height(height),
        m_chunkStore(chunkStore),
        m_chunkRadius(chunkRadius)
    {
        m_layoutIndexes = new uint8_t[width * height];
        memcpy(m_layoutIndexes, layoutBuffer, width * height);
    }

    ~Layout() {
        delete [] m_layoutIndexes;
    }

    static std::unique_ptr<Layout> fromSonic1(
        const uint8_t* s1LayoutBuffer, 
        Store<Chunk>& chunkStore
    ) {
        uint8_t width = s1LayoutBuffer[0]+1;
        uint8_t height = s1LayoutBuffer[1]+1;

        auto res = new Layout(s1LayoutBuffer+2, width, height, chunkStore, 16);
        res->useFlaggedLayering();

        return std::unique_ptr<Layout>(res); 
    }

    Layout& useFlaggedLayering() {
        m_usingFlaggedLayering = true;
        return *this;
    }

    

    Chunk& getChunk(int index, TerrainLayer layer = TerrainLayer::NORMAL) const {
        int chunkId = m_layoutIndexes[index];

        if (m_usingFlaggedLayering) {
            bool isFlagged = chunkId & 0x80;
            
            chunkId = chunkId & (~0x80) + (isFlagged && (int)layer);
        }

        return m_chunkStore.get(chunkId);
    }

    Chunk& getChunk(int x, int y, TerrainLayer layer = TerrainLayer::NORMAL) const {
        int index = y * m_width + x;

        if (index >= m_chunkStore.count()){
            printf("rerere %d\n!!!", index);
        }

        return getChunk(index, layer);
    }

    int getChunkRadius() const { return m_chunkRadius; }
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

    bool isLayeringChunk(int x, int y) const {
        return (m_layoutIndexes[y * m_width + x] & 0x80) && m_usingFlaggedLayering;
    }

    Store<Chunk>& getChunkStore() const { return m_chunkStore; }
private:
    Store<Chunk>& m_chunkStore; 
    uint8_t* m_layoutIndexes;
    int m_width;
    int m_height;
    int m_chunkRadius;
    bool m_usingFlaggedLayering = false;
};

class Terrain {
public:
    Terrain(Layout& layout) : m_layout(layout) {}

    Chunk& getChunk(float x, float y) const {
        return m_layout.getChunk(
            (int)(x / (m_layout.getChunkRadius()*16)),
            (int)(y / (m_layout.getChunkRadius()*16))
        );
    }

    const ChunkBlock& getBlock(float x, float y) const {
        int ix = (int)(x / TERRAIN_TILE_SIZE) % (m_layout.getChunkRadius());
        int iy = (int)(y / TERRAIN_TILE_SIZE) % (m_layout.getChunkRadius());

        auto chunk = getChunk(x, y);

        return chunk.getBlock(ix, iy);
    }

    Store<Chunk>& getChunkStore() const { return m_layout.getChunkStore(); } 
    Layout& getLayout() const { return m_layout; }


private:
    Layout& m_layout;

};


class ITerrainLoader {
public:
    virtual std::unique_ptr<Store<Tile>>  loadTiles() = 0;
    virtual std::unique_ptr<Store<Block>> loadBlocks(Store<Tile>& tileStore) = 0;
    virtual std::unique_ptr<Store<Chunk>> loadChunks(Store<Block>& blockStore) = 0;
    virtual std::unique_ptr<Layout>  loadLayout(Store<Chunk>& chunkStore) = 0;

    Terrain load();
};

struct TerrainLoaderSonic1FilePaths {
    const char* angles;
    const char* horizontalHeights;
    const char* verticalHeights;

    const char* blocks;
    const char* chunks;

    const char* layout;
};

class TerrainLoaderSonic1 : public ITerrainLoader {
private:
    const int TILE_HEIGHTS_BUFF_SIZE = 4096;
    const int TILE_ANGLES_BUFF_SIZE  = 256;

public:
    TerrainLoaderSonic1(TerrainLoaderSonic1FilePaths filepaths)
        : m_filepaths(filepaths) {}

    std::unique_ptr<Store<Tile>> loadTiles() override {
        FILE* fVerHeights = fopen(m_filepaths.verticalHeights, "rb");
	    FILE* fHorHeights = fopen(m_filepaths.horizontalHeights, "rb");
	    FILE* fAngles	  = fopen(m_filepaths.angles, "rb");
	
        if (!fVerHeights || !fHorHeights || !fAngles) {
            printf("Error loading terrain data\n"
                "Vertical heights: %s %s\n"
                "Horizontal heights: %s %s\n"
                "Angles: %s %s\n",
                m_filepaths.verticalHeights,    fVerHeights ? "OK" : "FAIL",
                m_filepaths.horizontalHeights,  fHorHeights ? "OK" : "FAIL",
                m_filepaths.angles,             fAngles     ? "OK" : "FAIL");
        }

        uint8_t* verHeights = new uint8_t[TILE_HEIGHTS_BUFF_SIZE];
        if (!fread(verHeights, 1, TILE_HEIGHTS_BUFF_SIZE, fVerHeights), fclose(fVerHeights)) {
            printf("Error. Ver heights file corrupted\n");
            throw;
        }

        uint8_t* horHeights = new uint8_t[TILE_HEIGHTS_BUFF_SIZE];
        if (!fread(horHeights, 1, TILE_HEIGHTS_BUFF_SIZE, fHorHeights), fclose(fHorHeights)) {
            printf("Error. Hor heights file corrupted\n");
            throw;
        }

        uint8_t* angles = new uint8_t[TILE_ANGLES_BUFF_SIZE];
        if (!fread(angles, 1, TILE_ANGLES_BUFF_SIZE, fAngles), fclose(fAngles)) {
            printf("Error. Angles file corrupted\n");
            throw;
        }

        auto res = new Store<Tile>();

        for (int i = 0; i < 256; i++) {
            auto tile = new Tile();

            tile->id = i;
            tile->angle = { angles[i] };	
            
            for (int j = 0; j < 16; j++) {
                tile->heightsHorizontal[j] = horHeights[i * 16 + j]; 
                tile->heightsVertical[j]   = verHeights[i * 16 + j];
            }

            res->add(tile);
        }

        return std::unique_ptr<Store<Tile>>(res);
    }

    std::unique_ptr<Store<Block>> loadBlocks(Store<Tile>& tileStore) override {
        FILE* fstreamBlocks  = fopen(m_filepaths.blocks, "rb");

        if (!fstreamBlocks) {
            printf("Error loading blocks data: %s %s\n",
                m_filepaths.blocks,  fstreamBlocks  ? "OK" : "FAIL");
            
            throw;
        }

        fseek(fstreamBlocks, 0L, SEEK_END);
        uint32_t numbytes = ftell(fstreamBlocks);
        fseek(fstreamBlocks, 0L, SEEK_SET);	
        
        uint8_t* buffer = new uint8_t[numbytes];	
        fread(buffer, 1, numbytes, fstreamBlocks);
        fclose(fstreamBlocks);

        auto storeBlocks = new Store<Block>();

        for (int i = 0; i < numbytes; i++) {
            storeBlocks->add(new Block { 
                .id = i,
                .solid = tileStore.get(buffer[i])
            });

            std::printf("ma [%x] %x\n", storeBlocks->get(i).id, storeBlocks->get(i).solid.id);
        }

        delete [] buffer;

	    return std::unique_ptr<Store<Block>>(storeBlocks);
    }

    std::unique_ptr<Store<Chunk>> loadChunks(Store<Block>& blockStore) override {
        FILE* fstreamChunks  = fopen(m_filepaths.chunks, "rb");

        if (!fstreamChunks) {
            printf("Error loading chunks: %s %s\n",
                m_filepaths.chunks,  fstreamChunks  ? "OK" : "FAIL");
            
            throw;
        }

        fseek(fstreamChunks, 0L, SEEK_END);
        uint32_t numbytes = ftell(fstreamChunks);
        fseek(fstreamChunks, 0L, SEEK_SET);	
        
        uint8_t* buffer = new uint8_t[numbytes];	
        fread(buffer, 1, numbytes, fstreamChunks);
        fclose(fstreamChunks);

        auto storeChunks = new Store<Chunk>();

        uint8_t empty[512] = {0};

        storeChunks->add(new Chunk(empty, 16, blockStore));

     
        for (int i = 0; i < numbytes; i += 512) {
            storeChunks->add(new Chunk(buffer+i, 16, blockStore));
        }

        delete [] buffer;

	    return std::unique_ptr<Store<Chunk>>(storeChunks);
    }

    std::unique_ptr<Layout> loadLayout(Store<Chunk>& chunkStore) override {
        FILE* fstreamLayout  = fopen(m_filepaths.layout, "rb");

        if (!fstreamLayout) {
            printf("Error loading chunks: %s %s\n",
                m_filepaths.layout,  fstreamLayout  ? "OK" : "FAIL");
            
            throw;
        }

        fseek(fstreamLayout, 0L, SEEK_END);
        uint32_t numbytes = ftell(fstreamLayout);
        fseek(fstreamLayout, 0L, SEEK_SET);	
        
        uint8_t* buffer = new uint8_t[numbytes];	
        fread(buffer, 1, numbytes, fstreamLayout);
        fclose(fstreamLayout);

	    return Layout::fromSonic1(buffer, chunkStore);
    }

private:
    TerrainLoaderSonic1FilePaths m_filepaths;
};


}
