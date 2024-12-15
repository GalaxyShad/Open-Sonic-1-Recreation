#include "level-loader.hpp"

#include "terrain-loader.hpp"
#include "entity-creator.hpp"

#define LEVEL_TEXTURE_ID 255

Level* LevelLoader::loadFromSonic1(ZoneSonic1 zone, int act) {
    using namespace terrain;
    
    reset();

    auto sZone      = std::string(ZoneSonic1ToString(zone));
    auto sZoneShort = std::string(ZoneSonic1ToStringShort(zone));

    auto sAct = sZoneShort + std::to_string(act);
    std::transform(
        sAct.begin(), sAct.end(), sAct.begin(), 
        [](unsigned char c){ return std::tolower(c); }
    );

    sonic1LoadTerrain(sZoneShort, sAct);
    sonic1LoadStartPosition(sAct);
    sonic1LoadObjects(sAct);

    return new Level(
        *m_terrain, m_entityPlacementList, GameType::SONIC_1, 
        m_screen, m_input, m_audio, 
        sZone, sZoneShort, act, 
        m_playerStartPosition, *m_storeTiles.get());
}


Level* LevelLoader::loadFromSonic2(ZoneSonic2 zone, int act) {
    reset();
    
    assert(false && "Not implemented");

    return nullptr;
}


Level* LevelLoader::loadFromSonic3K(ZoneSonic3K zone, int act) {
    using namespace terrain;
    
    reset();
    
    TerrainLoaderSonic1FilePaths filepaths = {};

    auto sZone      = std::string(ZoneSonic3KToString(zone));
    auto sZoneShort = std::string("ICZ");

    filepaths.angles            = "content/levels/sonic3/Angles S3.bin";
    filepaths.verticalHeights   = "content/levels/sonic3/Height Maps S3.bin";
    filepaths.horizontalHeights = "content/levels/sonic3/Height Maps Rotated S3.bin";

    if (zone != ZoneSonic3K::ICE_CAP && act != 1) {
        assert(false && "Not implemented");
    }

    filepaths.blocks = "content/levels/sonic3/ICZ/Collision/1.bin";
    filepaths.chunks = "content/levels/sonic3/ICZ/Chunks/Primary.bin";
    filepaths.layout = "content/levels/sonic3/ICZ/Layout/1.bin";

    auto terrainLoader = TerrainLoaderSonic3(filepaths);

    m_storeTiles  = terrainLoader.loadTiles();
    m_storeBlocks = terrainLoader.loadBlocks(*m_storeTiles.get());
    m_storeChunks = terrainLoader.loadChunks(*m_storeBlocks.get());
    m_layout      = terrainLoader.loadLayout(*m_storeChunks.get());

    m_terrain = new terrain::Terrain(*m_layout.get());
    m_screen.loadTextureFromFile("content/levels/sonic3/ICZ/texture.png", LEVEL_TEXTURE_ID);

    return new Level(
        *m_terrain, m_entityPlacementList, GameType::SONIC_3K, 
        m_screen, m_input, m_audio, 
        sZone, sZoneShort, act, 
        m_playerStartPosition, *m_storeTiles.get());
}

void LevelLoader::reset() {
    m_entityPlacementList.clear();

	m_storeTiles.release();
	m_storeBlocks.release();
	m_storeChunks.release();
	m_layout.release();
        

	delete m_terrain;
}

///////////////////////////////////////////////////////////////////////////////////
//// Sonic 1
///////////////////////////////////////////////////////////////////////////////////

void LevelLoader::sonic1LoadTerrain(std::string& sZone, std::string& sZoneAct) {
    using namespace terrain;

	auto sCollide   = "content/levels/collide/"  + sZone    + ".bin";
	auto sMap256    = "content/levels/map256/"   + sZone    + ".bin";
    auto sLayout    = "content/levels/layout/"   + sZoneAct + ".bin";
    auto sTex       = "content/levels/textures/" + sZone    + ".png";
	
    TerrainLoaderSonic1FilePaths filepaths = {};

    filepaths.angles            = "content/levels/collide/Angle Map.bin";
    filepaths.verticalHeights   = "content/levels/collide/Collision Array (Normal).bin";
    filepaths.horizontalHeights = "content/levels/collide/Collision Array (Rotated).bin";
    filepaths.blocks            = sCollide.c_str();
    filepaths.chunks            = sMap256.c_str();
    filepaths.layout            = sLayout.c_str();

    auto terrainLoader = TerrainLoaderSonic1(filepaths);

    m_storeTiles  = terrainLoader.loadTiles();
    m_storeBlocks = terrainLoader.loadBlocks(*m_storeTiles.get());
    m_storeChunks = terrainLoader.loadChunks(*m_storeBlocks.get());
    m_layout      = terrainLoader.loadLayout(*m_storeChunks.get());

    m_terrain = new terrain::Terrain(*m_layout.get());
    m_screen.loadTextureFromFile(sTex.c_str(), LEVEL_TEXTURE_ID);
}

void LevelLoader::sonic1LoadStartPosition(std::string& sZoneAct) {
    auto sStartPos = "content/levels/startpos/" + sZoneAct + ".bin";
    
    std::ifstream file(sStartPos, std::ios::binary);

    if (!file) {
        std::cout << "Error loading Act data" << std::endl;
        std::cout << "Start position: " << sStartPos << " FAIL" << std::endl;
        assert(false && "fail data read");
    }

    uint16_t x, y;

    if (!file.read(reinterpret_cast<char*>(&x), sizeof(x))) {
        assert(false && "fail data read");
    }
    
    x = (x << 8) | (x >> 8); // Swap the byte order for x

    if (!file.read(reinterpret_cast<char*>(&y), sizeof(y))) {
        assert(false && "fail data read");
    }

    y = (y << 8) | (y >> 8); // Swap the byte order for y

    m_playerStartPosition.x = static_cast<float>(x);
    m_playerStartPosition.y = static_cast<float>(y);
}

void LevelLoader::sonic1LoadObjects(std::string &sZoneAct) {
    auto sObjPos = "content/levels/objpos/" 	 + sZoneAct + ".bin";

    std::ifstream file(sObjPos, std::ios::binary);
	if (!file) {
        std::cout << "File loadFromFile error" << std::endl;
        return;
    }

    uint8_t buff[6];

    while (file.read(reinterpret_cast<char*>(buff), sizeof(buff))) {
        m_entityPlacementList.push_back(EntityPlacement::fromSonic1(buff));
    }

    file.close();
}
