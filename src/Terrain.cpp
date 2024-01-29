#include "Terrain.h"

#include "entities/general/LayerSwitcher.h"

#include <iostream>

void Terrain::create(
                     const uint8_t* blocks, const uint16_t* blockMapping, 
                     const uint8_t* lvLayout, uint8_t _lvTexture) {
    
    blocksPtr       = blocks;
    blockMappingPtr = blockMapping;
    lvLayoutPtr     = lvLayout + 2;

    uint8_t width  = *(lvLayout) + 1;
    uint8_t height = *(lvLayout+1) + 1;
    lvSize = Size(width, height);

    lvTexture = _lvTexture;
}

void Terrain::createLayeringObjs(std::list<Entity*>& entList) {
    auto& layout = m_terrain.getLayout();
    
    for (int i = 0; i < layout.getHeight(); i++) {
        for (int j = 0; j < layout.getWidth(); j++) {
            if (!layout.isLayeringChunk(j, i)) continue;

            int x = j * layout.getChunksRadiusPixels();
            int y = i * layout.getChunksRadiusPixels();

            entList.push_back(new LayerSwitcher(Vector2f(x+128, y+32), Vector2f(16, 64), 2));
            entList.push_back(new LayerSwitcher(Vector2f(x-8, y+128), Vector2f(16, 256), 0));
            entList.push_back(new LayerSwitcher(Vector2f(x+264, y+128), Vector2f(16, 256), 1));
        }
    }
}

uint16_t Terrain::getBlock(Vector2i pos) {
    uint8_t chunk = m_terrain.getLayout().getChunkId(pos.x / 256, pos.y / 256);

    int chunkX = (pos.x / 256) * 256;
    int chunkY = (pos.y / 256) * 256;
    
    if (chunk != 0)
        return blockMappingPtr[((chunk - 1) * 16 + (pos.y - chunkY) / 16) * 16 + (pos.x - chunkX) / 16];
    else
        return 0;
    
}

Tile Terrain::getTile(Vector2i pos) {
    const terrain::ChunkBlock& trBlock = m_terrain.getBlock(pos.x, pos.y, (terrain::TerrainLayer)layer);
    const terrain::Tile&  trTile = trBlock.tile;


    Tile tile = {};

    tile.type = (TileType)trBlock.solidityNormalLayer;
    tile.pos  = Vector2i((pos.x / 16) * 16, (pos.y / 16) * 16);


    // Set Heights
    for (int i = 0; i < 16; i++) {
        tile.verHeight[i] = trTile.heightsVertical[i];
        tile.horHeight[i] = trTile.heightsHorizontal[i];
    }

    tile.angle = trTile.angle.isRotatable() 
        ? trTile.angle.degrees() 
        : 0;

    return tile;
}

int Terrain::getTileVerHeight(Vector2i pos) {
    int xx = pos.x - ((pos.x / 16) * 16); 

    int height = (int)getTile(pos).verHeight[xx];

    return height;
}

int Terrain::getTileHorHeight(Vector2i pos) {
    int yy = pos.y - ((pos.y / 16) * 16);

    int height = (int)getTile(pos).horHeight[yy];

    return height;
}

TileType Terrain::getTileType(Vector2i pos) {
    return getTile(pos).type;
}

float Terrain::getTileAngle(Vector2i pos) {
    return getTile(pos).angle;
}

void Terrain::draw(Camera& cam) {

    uint16_t camX = (uint16_t)cam.getPos().x;
    uint16_t camY = (uint16_t)cam.getPos().y;

    int16_t left   = (camX - chunkSide) / chunkSide;
    int16_t top    = (camY - chunkSide) / chunkSide;
    int16_t right  = (camX + chunkSide + cam.getSize().width)  / chunkSide;
    int16_t bottom = (camY + chunkSide + cam.getSize().height) / chunkSide;

    if (bottom > lvSize.height)  bottom = lvSize.height;
    if (top < 0)                 top = 0;
    if (right > lvSize.width)    right = lvSize.width;
    if (left < 0)                left = 0;

    for(int i = top; i < bottom; i++) {
        for (int j = left; j < right; j++) {
            uint16_t chunk = lvLayoutPtr[i * lvSize.width + j] & 0x7F;

            if (chunk) {
                drawChunk(cam, chunk -1, Vector2f(j * chunkSide, i * chunkSide));
            }
        }
    }
}

void Terrain::drawChunk(Camera& cam, uint16_t chunkId, Vector2f pos) {
    if (!lvTexture)
        return;

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            auto block = m_terrain.getChunkStore().get(chunkId+1).getBlock(j, i);

            cam.draw(
                lvTexture, 
                IntRect(0, block.blockId * 16, 16, 16), 
                Vector2f(pos.x + j * 16.0, pos.y + i * 16.0), 
                Vector2i(0, 0), 
                0.0, 
                block.xFlip, 
                block.yFlip
            );
        }
    }
}

void Terrain::drawChunkPart(Camera& cam, uint16_t chunkId, Vector2f pos, IntRect rect) {
    if (!lvTexture)
        return;

    int ystart = rect.top / 16;
    int yend   = (rect.top+rect.height) / 16;

    for (int i = ystart; i <= yend; i++) {
        for (int j = 0; j < 16; j++) {

            uint16_t block = blockMappingPtr[(chunkId*16+i)*16 + j];

            uint16_t yFlip    = ((block << 3) >> 15) & 1;
            uint16_t xFlip    = ((block << 4) >> 15) & 1;
            uint16_t blockID  =  (block << 5);

            blockID = blockID >> 5;

            IntRect br = IntRect(0, blockID * 16, 16, 16);

            if (i == ystart) {
                br.top += rect.top % 16;
                br.height = 16 - rect.top % 16; 
            } 

            if (i == yend && i != ystart) {
                br.height = (rect.top+rect.height)  - yend * 16;
            }

            cam.draw(
                lvTexture,
                br, 
                Vector2f(pos.x + j * 16.0, 
                         (pos.y + (i - ystart) * 16.0) - ((i == ystart) ? 0 : rect.top % 16)),
                Vector2i(0, 0), 
                0.0, 
                xFlip, 
                yFlip
            );
        }
    }
}

