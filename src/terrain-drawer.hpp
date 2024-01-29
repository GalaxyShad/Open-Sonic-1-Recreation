
#pragma once

#include "new-terrain.hpp"
#include "Camera.h"

namespace terrain {

class TerrainDrawer {
public:
    TerrainDrawer(Camera& camera, Store<Chunk>& storeChunk, Layout& layout, uint8_t textureId) 
        : m_camera(camera) 
        , m_layout(layout)
        , m_storeChunk(storeChunk)
        , m_textureId(textureId)
    {}

    void draw() const;
    void drawChunk(const Chunk& chunk, float x, float y) const;
    void drawChunkPart(const Chunk& chunk, Vector2f pos, IntRect rect) const;
    void drawChunkPartById(int chunkId, Vector2f pos, IntRect rect) const;

private:
    Camera& m_camera;
    Layout& m_layout;
    Store<Chunk>& m_storeChunk;

    uint8_t m_textureId;

};

}