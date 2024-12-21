#include "terrain-drawer.hpp"
#include "core/game_enviroment/artist/ArtistStructs.h"
#include "new-terrain.hpp"
#include <memory>

using namespace terrain;


void terrain::TerrainDrawer::initDebugCollisionView() {
    if (m_imgTiles != nullptr)
        return;

    m_imgTiles = std::unique_ptr<artist_api::Image>(
        new artist_api::Image({.x = TERRAIN_TILE_SIZE,
                               .y = TERRAIN_TILE_SIZE * m_storeTiles.count()}));

    for (int i = 0; i < m_storeTiles.count(); i++) {
        Tile &tile = m_storeTiles.get(i);

        int tileTop = i * TERRAIN_TILE_SIZE;
        int tileBot = tileTop + TERRAIN_TILE_SIZE - 1;

        for (int j = 0; j < TERRAIN_TILE_SIZE; j++) {
            int height = tile.heightsVertical[j];

            for (int k = 0; k < abs(height); k++) {
                int y = (height > 0) ? (tileBot - k) : (tileTop + k);

                m_imgTiles->setPixel(j, y, artist_api::Color::WHITE());
            }
        }
    }

    m_texTiles = m_camera.getScr().textureLoader().loadFromPixelBuffer(
        m_imgTiles->bitmap().buffer_, m_imgTiles->bitmap().size_.x);
}

void TerrainDrawer::draw() const {
    auto camX = m_camera.getPos().x;
    auto camY = m_camera.getPos().y;
    auto camW = m_camera.getSize().width;
    auto camH = m_camera.getSize().height;

    auto chunkSide = m_layout.getChunksRadiusPixels();

    auto left = (camX - chunkSide) / chunkSide;
    auto top = (camY - chunkSide) / chunkSide;
    auto right = (camX + chunkSide + camW) / chunkSide;
    auto bottom = (camY + chunkSide + camH) / chunkSide;

    if (!m_layout.isWarpEnabled()) {
        if (bottom >= m_layout.getHeight())
            bottom = m_layout.getHeight();
        if (top < 0)
            top = 0;
        if (right >= m_layout.getWidth())
            right = m_layout.getWidth();
        if (left < 0)
            left = 0;
    }

    for (int i = top; i < bottom; i++) {
        for (int j = left; j < right; j++) {
            auto &chunk = m_layout.getChunk(j, i, TerrainLayer::NORMAL);

            drawChunk(chunk, j * chunkSide, i * chunkSide);
        }
    }
}

void TerrainDrawer::drawChunk(const Chunk &chunk, float x, float y) const {
    for (int i = 0; i < chunk.getRadius(); i++) {
        for (int j = 0; j < chunk.getRadius(); j++) {
            auto block = chunk.getBlock(j, i);

            auto& artist = m_camera.getScr().artist();

            artist_api::Rect rect = {
                .x = 0,
                .y = static_cast<float>(block.blockId * TERRAIN_TILE_SIZE),
                .width = TERRAIN_TILE_SIZE,
                .height = TERRAIN_TILE_SIZE
            };

            artist_api::Vector2D<float> pos = {
                .x = x + j * TERRAIN_TILE_SIZE - m_camera.getPos().x,
                .y = y + i * TERRAIN_TILE_SIZE - m_camera.getPos().y
            };

            artist_api::Vector2D<float> offset = {
                static_cast<float>(16 * block.xFlip), 
                static_cast<float>(16 * block.yFlip)
            };

            artist.drawSprite(artist_api::Sprite {
                .texture = *m_tex,
                .rect = rect,
                .offset = offset
            }, pos, {
                .flipHorizontal = block.xFlip,
                .flipVertical = block.yFlip,
                .scale = {1.f, 1.f}
            });

            if (m_debugCollisionView)
                drawChunkBlockDebug(block, x, j, y, i);
        }
    }
}

void terrain::TerrainDrawer::drawChunkBlockDebug(terrain::ChunkBlock &block,
                                                 float x, int j, float y,
                                                 int i) const {
    float sprLeft = block.xFlip ? TERRAIN_TILE_SIZE : 0;
    float sprTop = (TERRAIN_TILE_SIZE * block.tile.id) +
                   (block.yFlip ? TERRAIN_TILE_SIZE : 0);

    float sprWidth = block.xFlip ? -TERRAIN_TILE_SIZE : TERRAIN_TILE_SIZE;
    float sprHeight = block.yFlip ? -TERRAIN_TILE_SIZE : TERRAIN_TILE_SIZE;

    artist_api::Sprite spr = {
        .texture = *m_texTiles,
        .rect = {
            .x = sprLeft, .y = sprTop, .width = sprWidth, .height = sprHeight}};

    artist_api::Vector2D<float> pos = {
        .x = x + j * TERRAIN_TILE_SIZE - m_camera.getPos().x,
        .y = y + i * TERRAIN_TILE_SIZE - m_camera.getPos().y};

    artist_api::Color color =
        (block.solidityNormalLayer == BlockSolidity::ONLY_TOP)
            ? artist_api::Color::fromRGB(0x055CBC)
        : (block.solidityNormalLayer == BlockSolidity::ONLY_LEFT_BOTTOM_RIGHT)
            ? artist_api::Color::fromRGB(0x01CC86)
            : artist_api::Color::fromRGB(0xD1CDCC);

    color.alpha = ((i + j) % 2) ? 0xF0 : 0xD0;

    if (block.solidityNormalLayer != BlockSolidity::EMPTY)
        m_camera.getScr().artist().drawSprite(spr, pos, {.blending = color});
}

void terrain::TerrainDrawer::drawChunkById(int chunkId, float x,
                                           float y) const {
    auto &chunk = m_storeChunk.get(chunkId);

    drawChunk(chunk, x, y);
}

void TerrainDrawer::drawChunkPart(const Chunk &chunk, v2f pos,
                                  irect rect) const {
    int ystart = rect.top / 16;
    int yend = (rect.top + rect.height) / 16;

    if (yend >= 16)
        yend = 15;

    for (int i = ystart; i <= yend; i++) {
        for (int j = 0; j < 16; j++) {
            auto block = chunk.getBlock(j, i);

            irect br = irect(0, block.blockId * 16, 16, 16);

            if (i == ystart) {
                br.top += rect.top % 16;
                br.height = 16 - rect.top % 16;
            }

            if (i == yend && i != ystart) {
                br.height = (rect.top + rect.height) - yend * 16;
            }

            m_camera.draw(m_textureId, br,
                          v2f(pos.x + j * TERRAIN_TILE_SIZE,
                              (pos.y + (i - ystart) * TERRAIN_TILE_SIZE) -
                                  ((i == ystart) ? 0 : rect.top % 16)),
                          v2i(0, 0), 0.0, block.xFlip, block.yFlip);
        }
    }
}

void TerrainDrawer::drawChunkPartById(int chunkId, v2f pos, irect rect) const {
    auto &chunk = m_storeChunk.get(chunkId);

    drawChunkPart(chunk, pos, rect);
}