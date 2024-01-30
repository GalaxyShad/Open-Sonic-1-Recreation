#include "terrain-drawer.hpp"

using namespace terrain;

void TerrainDrawer::draw() const {
    auto camX = m_camera.getPos().x;
    auto camY = m_camera.getPos().y;
    auto camW = m_camera.getSize().width;
    auto camH = m_camera.getSize().height;

    auto chunkSide = m_layout.getChunksRadiusPixels();

    auto left   = (camX - chunkSide) / chunkSide;
    auto top    = (camY - chunkSide) / chunkSide;
    auto right  = (camX + chunkSide + camW) / chunkSide;
    auto bottom = (camY + chunkSide + camH) / chunkSide;

    if (bottom >= m_layout.getHeight()) bottom = m_layout.getHeight();
    if (top < 0)                        top    = 0;
    if (right >= m_layout.getWidth())   right  = m_layout.getWidth();
    if (left < 0)                       left   = 0;

    for(int i = top; i < bottom; i++) {
        for (int j = left; j < right; j++) {
            auto& chunk = m_layout.getChunk(j, i, TerrainLayer::NORMAL);

            drawChunk(chunk, j * chunkSide, i * chunkSide);
        }
    }
}

void TerrainDrawer::drawChunk(const Chunk& chunk, float x, float y) const {
    for (int i = 0; i < chunk.getRadius(); i++) {
        for (int j = 0; j < chunk.getRadius(); j++) {
            auto block = chunk.getBlock(j, i);

            m_camera.draw(
                m_textureId, 
                IntRect(
                    0,                 block.blockId * TERRAIN_TILE_SIZE, 
                    TERRAIN_TILE_SIZE, TERRAIN_TILE_SIZE
                ), 
                Vector2f(
                    x + j * TERRAIN_TILE_SIZE, 
                    y + i * TERRAIN_TILE_SIZE
                ), 
                Vector2i(0, 0), 
                0.0, 
                block.xFlip, 
                block.yFlip
            );
        }
    }
}

void terrain::TerrainDrawer::drawChunkById(int chunkId, float x, float y) const {
    auto& chunk = m_storeChunk.get(chunkId);

    drawChunk(chunk, x, y);
}

void TerrainDrawer::drawChunkPart(const Chunk& chunk, Vector2f pos, IntRect rect) const {
    int ystart = rect.top / 16;
    int yend   = (rect.top+rect.height) / 16;

    if (yend >= 16) yend = 15;

    for (int i = ystart; i <= yend; i++) {
        for (int j = 0; j < 16; j++) {
            auto block = chunk.getBlock(j, i);

            IntRect br = IntRect(0, block.blockId * 16, 16, 16);

            if (i == ystart) {
                br.top += rect.top % 16;
                br.height = 16 - rect.top % 16; 
            } 

            if (i == yend && i != ystart) {
                br.height = (rect.top+rect.height)  - yend * 16;
            }

            m_camera.draw(
                m_textureId,
                br, 
                Vector2f(pos.x + j * TERRAIN_TILE_SIZE, 
                        (pos.y + (i - ystart) * TERRAIN_TILE_SIZE) - ((i == ystart) ? 0 : rect.top % 16)),
                Vector2i(0, 0), 
                0.0, 
                block.xFlip, 
                block.yFlip
            );
        }
    }
}

void TerrainDrawer::drawChunkPartById(int chunkId, Vector2f pos, IntRect rect) const {
    auto& chunk = m_storeChunk.get(chunkId);

    drawChunkPart(chunk, pos, rect);
}