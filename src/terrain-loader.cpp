#include "terrain-loader.hpp"

#include "makeunique.hpp"

using namespace terrain;

std::unique_ptr<Store<Tile>> TerrainLoaderSonic1::loadTiles() {
    std::ifstream fVerHeights(m_filepaths.verticalHeights, std::ios::binary);
    std::ifstream fHorHeights(m_filepaths.horizontalHeights, std::ios::binary);
    std::ifstream fAngles(m_filepaths.angles, std::ios::binary);

    if (!fVerHeights || !fHorHeights || !fAngles) {
        throw std::runtime_error("Error loading terrain data\n"
            "Vertical heights: " + std::string(m_filepaths.verticalHeights) + " " + (fVerHeights ? "OK" : "FAIL") + "\n"
            "Horizontal heights: " + std::string(m_filepaths.horizontalHeights) + " " + (fHorHeights ? "OK" : "FAIL") + "\n"
            "Angles: " + std::string(m_filepaths.angles) + " " + (fAngles ? "OK" : "FAIL"));
    }

    std::vector<int8_t> verHeights(TILE_HEIGHTS_BUFF_SIZE);
    if (!fVerHeights.read(reinterpret_cast<char*>(verHeights.data()), TILE_HEIGHTS_BUFF_SIZE)) {
        throw std::runtime_error("Error. Vertical heights file corrupted");
    }

    std::vector<int8_t> horHeights(TILE_HEIGHTS_BUFF_SIZE);
    if (!fHorHeights.read(reinterpret_cast<char*>(horHeights.data()), TILE_HEIGHTS_BUFF_SIZE)) {
        throw std::runtime_error("Error. Horizontal heights file corrupted");
    }

    std::vector<uint8_t> angles(TILE_ANGLES_BUFF_SIZE);
    if (!fAngles.read(reinterpret_cast<char*>(angles.data()), TILE_ANGLES_BUFF_SIZE)) {
        throw std::runtime_error("Error. Angles file corrupted");
    }

    auto res = make_unique<Store<Tile>>();

    for (int i = 0; i < 256; i++) {
        Tile tile;

        tile.id = i;
        tile.angle = { angles[i] };

        for (int j = 0; j < 16; j++) {
            tile.heightsHorizontal[j] = horHeights[i * 16 + j];
            tile.heightsVertical[j] = verHeights[i * 16 + j];
        }

        res->add(tile);
    }

    return res;
}

std::unique_ptr<Store<Block>> TerrainLoaderSonic1::loadBlocks(Store<Tile>& tileStore) {
    std::ifstream fstreamBlocks(m_filepaths.blocks, std::ios::binary);

    if (!fstreamBlocks) {
        throw std::runtime_error("Error loading blocks data: " + std::string(m_filepaths.blocks));
    }

    fstreamBlocks.seekg(0, std::ios::end);
    uint32_t numbytes = fstreamBlocks.tellg();
    fstreamBlocks.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(numbytes);
    fstreamBlocks.read(reinterpret_cast<char*>(buffer.data()), numbytes);
    fstreamBlocks.close();

    std::unique_ptr<Store<Block>> storeBlocks = make_unique<Store<Block>>();

    for (int i = 0; i < numbytes; i++) {
        Block block(i, tileStore.get(buffer[i]));
        storeBlocks->add(block);
    }

    return storeBlocks;
}

std::unique_ptr<Store<Chunk>> TerrainLoaderSonic1::loadChunks(Store<Block>& blockStore) {
    std::ifstream fstreamChunks(m_filepaths.chunks, std::ios::binary);

    if (!fstreamChunks) {
        throw std::runtime_error("Error loading chunks: " + std::string(m_filepaths.chunks));
    }

    fstreamChunks.seekg(0, std::ios::end);
    uint32_t numbytes = fstreamChunks.tellg();
    fstreamChunks.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(numbytes);
    fstreamChunks.read(reinterpret_cast<char*>(buffer.data()), numbytes);
    fstreamChunks.close();

    std::unique_ptr<Store<Chunk>> storeChunks = make_unique<Store<Chunk>>();

    uint8_t empty[512] = {0};
    Chunk chEmpty(empty, ChunkFormat::SONIC_1_CD, blockStore);
    storeChunks->add(chEmpty);

    for (int i = 0; i < numbytes; i += 512) {
        Chunk ch(buffer.data() + i, ChunkFormat::SONIC_1_CD, blockStore);
        storeChunks->add(ch);
    }

    return storeChunks;
}

std::unique_ptr<Layout> TerrainLoaderSonic1::loadLayout(Store<Chunk>& chunkStore) {
    std::ifstream fstreamLayout(m_filepaths.layout, std::ios::binary);

    if (!fstreamLayout) {
        throw std::runtime_error("Error loading layout: " + std::string(m_filepaths.layout));
    }

    fstreamLayout.seekg(0, std::ios::end);
    uint32_t numbytes = fstreamLayout.tellg();
    fstreamLayout.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(numbytes);
    fstreamLayout.read(reinterpret_cast<char*>(buffer.data()), numbytes);
    fstreamLayout.close();

    return std::unique_ptr<Layout>(Layout::createFromSonic1(buffer.data(), chunkStore));
}