#include "terrain-drawer.hpp"

using namespace terrain;

terrain::TerrainDrawer::~TerrainDrawer() {
    delete m_imgTiles;
    delete m_texTiles;
}

void terrain::TerrainDrawer::initDebugCollisionView() {
    if (m_imgTiles != nullptr)
        return;

    m_imgTiles = new sf::Image();
    m_texTiles = new sf::Texture();

    m_imgTiles->create(TERRAIN_TILE_SIZE,
                       TERRAIN_TILE_SIZE * m_storeTiles.count(),
                       sf::Color::Transparent);

    for (int i = 0; i < m_storeTiles.count(); i++) {
        Tile &tile = m_storeTiles.get(i);

        int tileTop = i * TERRAIN_TILE_SIZE;
        int tileBot = tileTop + TERRAIN_TILE_SIZE - 1;

        for (int j = 0; j < TERRAIN_TILE_SIZE; j++) {
            int height = tile.heightsVertical[j];

            for (int k = 0; k < abs(height); k++) {
                int y = (height > 0) ? (tileBot - k) : (tileTop + k);

                m_imgTiles->setPixel(j, y, sf::Color(0xFFFFFFFF));
            }
        }
    }

    m_texTiles->loadFromImage(*m_imgTiles);
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

            m_camera.draw(
                m_textureId,
                irect(0, block.blockId * TERRAIN_TILE_SIZE, TERRAIN_TILE_SIZE,
                      TERRAIN_TILE_SIZE),
                v2f(x + j * TERRAIN_TILE_SIZE, y + i * TERRAIN_TILE_SIZE),
                v2i(16 * block.xFlip, 16 * block.yFlip), 0.0, block.xFlip, block.yFlip);

            if (m_debugCollisionView)
                drawChunkBlockDebug(block, x, j, y, i);
        }
    }
}

void terrain::TerrainDrawer::drawChunkBlockDebug(terrain::ChunkBlock &block,
                                                 float x, int j, float y,
                                                 int i) const {
    int sprLeft = block.xFlip ? TERRAIN_TILE_SIZE : 0;
    int sprTop = (TERRAIN_TILE_SIZE * block.tile.id) +
                 (block.yFlip ? TERRAIN_TILE_SIZE : 0);

    int sprWidth = block.xFlip ? -TERRAIN_TILE_SIZE : TERRAIN_TILE_SIZE;
    int sprHeight = block.yFlip ? -TERRAIN_TILE_SIZE : TERRAIN_TILE_SIZE;

    sf::Sprite spr(*m_texTiles,
                   sf::IntRect(sprLeft, sprTop, sprWidth, sprHeight));
    spr.setPosition(x + j * TERRAIN_TILE_SIZE - m_camera.getPos().x,
                    y + i * TERRAIN_TILE_SIZE - m_camera.getPos().y);

    sf::Color color =
        (block.solidityNormalLayer == BlockSolidity::ONLY_TOP)
            ? sf::Color(0x055CBCFF)
        : (block.solidityNormalLayer == BlockSolidity::ONLY_LEFT_BOTTOM_RIGHT)
            ? sf::Color(0x01CC86FF)
            : sf::Color(0xD1CDCCFF);

    color.a = ((i + j) % 2) ? 0xF0 : 0xD0;

    spr.setColor(color);

    if (block.solidityNormalLayer != BlockSolidity::EMPTY)
        m_camera.getScr().getSfmlWindow().draw(spr);
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