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
    for (int i = 0; i < lvSize.width*lvSize.height; i++) {
        uint8_t chunk = lvLayoutPtr[i];

        if (chunk & 0x80) {
            int x = (i % lvSize.width) * 256;
            int y = (i / lvSize.width) * 256;
            
            entList.push_back(new LayerSwitcher(Vector2f(x+128, y+32), Vector2f(16, 64), 2));
            entList.push_back(new LayerSwitcher(Vector2f(x-8, y+128), Vector2f(16, 256), 0));
            entList.push_back(new LayerSwitcher(Vector2f(x+264, y+128), Vector2f(16, 256), 1));
        }
    }
}

uint8_t Terrain::getChunk(Vector2i pos) {
    if (pos.x  < 0 || pos.x  > lvSize.width  * 256 || 
        pos.y  < 0 || pos.y  > lvSize.height * 256)
            return 0;
    
    uint8_t chunk = lvLayoutPtr[(pos.y / 256) * lvSize.width + pos.x / 256];

    // Loop
    if (chunk & 0x80) {
        return (chunk & 0x7F) + layer;    
    } else {
        return (chunk & 0x7F);
    }
}

uint16_t Terrain::getBlock(Vector2i pos) {
    int chunk  = int(getChunk(pos));
    int chunkX = (pos.x / 256) * 256;
    int chunkY = (pos.y / 256) * 256;
    
    if (chunk != 0)
        return blockMappingPtr[((chunk - 1) * 16 + (pos.y - chunkY) / 16) * 16 + (pos.x - chunkX) / 16];
    else
        return 0;
    
}

Tile Terrain::getTile(Vector2i pos) {
    uint16_t block = getBlock(pos);

    uint8_t  xFlip    = ((block & 0x800)  >> 11);
    uint8_t  yFlip    = ((block & 0x1000) >> 12);
    uint8_t  type     = block >> 13;
	uint16_t blockID  = block & 0b011111111111;

    Tile tile;

    tile.type = TileType(type);
    tile.pos  = Vector2i((pos.x / 16) * 16, (pos.y / 16) * 16);

    TerrainTile trTile = m_tileStore.getTile(blocksPtr[int(blockID)]);

    if (xFlip) trTile = trTile.flipHorizontal();
    if (yFlip) trTile = trTile.flipVertical();

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

    return (height <= 16) ? height : 256 - height;
}

int Terrain::getTileHorHeight(Vector2i pos) {
    int yy = pos.y - ((pos.y / 16) * 16);

    int height = (int)getTile(pos).horHeight[yy];

    return (height <= 16) ? height : 256 - height;
}

TileType Terrain::getTileType(Vector2i pos) {
    uint16_t block = getBlock(pos);
    uint8_t solidity = block >> 13;
    return (TileType)solidity;
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
            uint16_t block = blockMappingPtr[(chunkId*16+i)*16 + j];

            uint16_t yFlip    = ((block << 3) >> 15) & 1;
            uint16_t xFlip    = ((block << 4) >> 15) & 1;
            uint16_t blockID  = (block << 5);

            blockID = blockID >> 5;
            cam.draw(
                lvTexture, 
                IntRect(0, blockID * 16, 16, 16), 
                Vector2f(pos.x + j * 16.0, pos.y + i * 16.0), 
                Vector2i(0, 0), 
                0.0, 
                xFlip, 
                yFlip
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

