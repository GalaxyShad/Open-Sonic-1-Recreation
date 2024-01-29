#include "Terrain.h"

#include "entities/general/LayerSwitcher.h"

#include <iostream>

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






