
#pragma once

#include "core/game_enviroment/artist/ArtistStructs.h"
#include "new-terrain.hpp"
#include "Camera.h"

#include <SFML/Graphics.hpp>
#include <memory>

namespace terrain {

class TerrainDrawer {
public:
    TerrainDrawer(Camera& camera, Store<Chunk>& storeChunk, Layout& layout, uint8_t textureId, Store<Tile>& storeTiles) 
        : m_camera(camera) 
        , m_layout(layout)
        , m_storeChunk(storeChunk)
        , m_textureId(textureId)
        , m_storeTiles(storeTiles)
    {}

    virtual ~TerrainDrawer() = default;

    void allowDrawingCollisions(bool value) {
        initDebugCollisionView();

        m_debugCollisionView = value;
    }

    bool isDrawingCollisions() { return m_debugCollisionView; }

    void draw() const;
    void drawChunk(const Chunk& chunk, float x, float y) const;
    void drawChunkById(int chunkId, float x, float y) const;
    void drawChunkPart(const Chunk &chunk, v2f pos, irect rect) const;
    void drawChunkPartById(int chunkId, v2f pos, irect rect) const;

private:
    Camera&         m_camera;
    Layout&         m_layout;
    Store<Chunk>&   m_storeChunk;
    Store<Tile>&    m_storeTiles;

    bool            m_debugCollisionView = false;

    std::unique_ptr<artist_api::Image> m_imgTiles = nullptr;
    std::unique_ptr<artist_api::Texture> m_texTiles = nullptr;

    uint8_t         m_textureId;

private:
    void initDebugCollisionView();
    void drawChunkBlockDebug(terrain::ChunkBlock &block, float x, int j, float y, int i) const;

};

}