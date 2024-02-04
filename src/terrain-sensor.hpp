#pragma once

#include "new-terrain.hpp"

namespace terrain {

enum class SensorDirection {
    DOWN,
    UP,
    RIGHT,
    LEFT,
};

//https://info.sonicretro.org/SPG:Solid_Tiles#Sensors
class Sensor {
public:
    Sensor(Vector2f position, SensorDirection direction, Terrain& terrain)
        : m_terrain(terrain) 
        , m_position(position)
        , m_direction(direction)
    {}

    void         setLayer(TerrainLayer layer) { m_layer = layer; }
    TerrainLayer getLayer()                   { return m_layer;  }

    void     setPosition(Vector2f position) { m_position = position; }
    Vector2f getPosition()                  { return m_position;     }

private: 
    SensorDirection m_direction;
    Vector2f        m_position;
    Terrain&        m_terrain;
    TerrainLayer    m_layer = TerrainLayer::NORMAL;

private:
    ChunkBlock getBlock(int gridX, int gridY) {
        ChunkBlock baseBlock;
        int        baseHeight;

        initBlockAndHeight(baseBlock, baseHeight, gridX, gridY);

        if (baseHeight >= 1 && baseHeight <= 15)
            return baseBlock;

        // Regression
        else if (baseHeight >= 16) {
            ChunkBlock regressionBlock;
            int        regressionHeight;

            initBlockAndHeight(regressionBlock, regressionHeight, gridX, gridY, -1);

            if (isBlockEmpty(regressionHeight, regressionBlock))
                return baseBlock;

            return regressionBlock;
        }

        // Extension
        else if (isBlockEmpty(baseHeight, baseBlock)) {
            ChunkBlock extensionBlock;
            int        extensionHeight;

            initBlockAndHeight(extensionBlock, extensionHeight, gridX, gridY, +1);

            return extensionBlock;
        }
    }    

    void initBlockAndHeight(ChunkBlock& block, int& height, int gridX, int gridY, int ext = 0) {
        switch (m_direction) {
            case SensorDirection::DOWN:
                block  = m_terrain.getBlockFromGrid(gridX, gridY+ext, m_layer);
                height = m_terrain.getVerticalHeightInTile(m_position.x, block.tile);
                break;
            case SensorDirection::UP:
                block  = m_terrain.getBlockFromGrid(gridX, gridY-ext, m_layer);
                height = m_terrain.getVerticalHeightInTile(m_position.x, block.tile);
                break;
            case SensorDirection::LEFT:
                block  = m_terrain.getBlockFromGrid(gridX-ext, gridY, m_layer);
                height = m_terrain.getHorizontalHeightInTile(m_position.y, block.tile);
                break;
            case SensorDirection::RIGHT:
                block  = m_terrain.getBlockFromGrid(gridX+ext, gridY, m_layer);
                height = m_terrain.getHorizontalHeightInTile(m_position.y, block.tile);
                break;
        }

        height = abs(height);
    }


    bool isBlockEmpty(int detectedHeight, ChunkBlock& block) {
        return (detectedHeight == 0 || checkBlockSolidity(block) == BlockSolidity::EMPTY);
    }

    BlockSolidity checkBlockSolidity(ChunkBlock& block) {
        return (m_layer == TerrainLayer::NORMAL) ? block.solidityNormalLayer : block.solidityAlternateLayer;
    }

    Vector2i getCurrentBlockPositionInGrid(const Vector2f& position) {
        return Vector2i(
            (int)(position.x / TERRAIN_TILE_SIZE),
            (int)(position.y / TERRAIN_TILE_SIZE)
        );
    }
};

}
