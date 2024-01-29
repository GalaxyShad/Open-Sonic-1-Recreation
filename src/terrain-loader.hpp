#pragma once

#include <vector> 
#include <fstream>

#include "new-terrain.hpp"

namespace terrain {

class ITerrainLoader {
public:
    virtual std::unique_ptr<Store<Tile>>  loadTiles() = 0;
    virtual std::unique_ptr<Store<Block>> loadBlocks(Store<Tile>& tileStore) = 0;
    virtual std::unique_ptr<Store<Chunk>> loadChunks(Store<Block>& blockStore) = 0;
    virtual std::unique_ptr<Layout>       loadLayout(Store<Chunk>& chunkStore) = 0;

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

    std::unique_ptr<Store<Tile>>  loadTiles() override;
    std::unique_ptr<Store<Block>> loadBlocks(Store<Tile>& tileStore)   override;
    std::unique_ptr<Store<Chunk>> loadChunks(Store<Block>& blockStore) override;
    std::unique_ptr<Layout>       loadLayout(Store<Chunk>& chunkStore) override;

private:
    TerrainLoaderSonic1FilePaths m_filepaths;
};

}