#pragma once

#include "new-terrain.hpp"

#include "core/Screen.h"
#include "Camera.h"

namespace terrain {

enum class SensorDirection {
    DOWN,
    RIGHT,
    UP,
    LEFT,
};

struct SensorResult {
    HexAngle      angle;
    int           distance;
    BlockSolidity solidity;
    
    bool isFlagged() { return !angle.isRotatable(); }

    static SensorResult Empty() {
        return terrain::SensorResult {
            HexAngle { 0 },
            31,
            terrain::BlockSolidity::EMPTY
        };
    }
};

//https://info.sonicretro.org/SPG:Solid_Tiles#Sensors
class Sensor {
public:
    Sensor(v2f position, SensorDirection direction, Terrain& terrain)
        : m_terrain(terrain) 
        , m_position(position)
        , m_direction(direction)
    {}

    void         setLayer(TerrainLayer layer) { m_layer = layer; }
    TerrainLayer getLayer()                   { return m_layer;  }

    void     setPosition(v2f position) { m_position = position; }
    v2f getPosition()                  { return m_position;     }

    void     move(v2f delta)           { m_position.x += delta.x; m_position.y += delta.y; }

    void            setDirection(SensorDirection direction) { m_direction = direction; }
    SensorDirection getDirection()                          { return m_direction;      }

    SensorResult scan() {
        auto gridPos     = getCurrentBlockPositionInGrid(m_position);
        auto blockResult = getBlock(gridPos.x, gridPos.y);

        return SensorResult {
            blockResult.block.tile.angle,
            blockResult.distance,
            (m_layer == TerrainLayer::NORMAL) 
                ? blockResult.block.solidityNormalLayer 
                : blockResult.block.solidityAlternateLayer
        };
    }

    int getDistance() {
        return scan().distance;
    }

    void draw(Camera& cam, sf::Color color = sf::Color::White) {
        float poscamx = m_position.x - cam.getPos().x;
        float poscamy = m_position.y - cam.getPos().y;

        drawDot(poscamx, poscamy, color, cam);

        auto gridPos = getCurrentBlockPositionInGrid(m_position);
        auto blockResult = getBlock(gridPos.x, gridPos.y);

        int distance = blockResult.distance;

        switch (m_direction) {
            case SensorDirection::DOWN:
                drawDot(poscamx, poscamy + distance, sf::Color::White, cam);
                break;
            case SensorDirection::UP:
                drawDot(poscamx, poscamy - distance, sf::Color::White, cam);
                break;
            case SensorDirection::LEFT:
                drawDot(poscamx - distance, poscamy, sf::Color::White, cam);
                break;
            case SensorDirection::RIGHT:
                drawDot(poscamx + distance, poscamy, sf::Color::White, cam);
                break;
        }
    }

private: 
    SensorDirection m_direction;
    v2f        m_position;
    Terrain&        m_terrain;
    TerrainLayer    m_layer = TerrainLayer::NORMAL;

private:
    struct BlockResult {
        ChunkBlock  block;
        v2i    gridPosition;
        int         distance;
        int         height;
    };

    BlockResult getBlock(int gridX, int gridY) {
        BlockResult baseBlock = getBlockExtended(gridX, gridY);

        if (baseBlock.height >= 1 && baseBlock.height <= 15 && checkBlockSolidity(baseBlock.block) != BlockSolidity::EMPTY)
            return baseBlock;

        // Regression
        else if (baseBlock.height >= 16 && checkBlockSolidity(baseBlock.block) != BlockSolidity::EMPTY) {
            BlockResult regressionBlock = getBlockExtended(gridX, gridY, -1);

            if (isBlockEmpty(regressionBlock))
                return baseBlock;

            return regressionBlock;
        }

        // Extension
        else if (isBlockEmpty(baseBlock)) {
            BlockResult extensionBlock = getBlockExtended(gridX, gridY, +1);

            return extensionBlock;
        }

        printf("Something strange happened :(. -> %d\n", baseBlock.height);
        return baseBlock;
    }    

    BlockResult getBlockExtended(int gridX, int gridY, int ext = 0) {
        BlockResult result = {};
        
        switch (m_direction) {
            case (SensorDirection::DOWN): {
                result.gridPosition = v2i(gridX, gridY + ext);
                result.block        = m_terrain.getBlockFromGrid(result.gridPosition.x, result.gridPosition.y, m_layer);
                result.height       = abs(m_terrain.getVerticalHeightInTile(m_position.x, result.block.tile));
                
                auto blockBottom    = (result.gridPosition.y * TERRAIN_TILE_SIZE) + TERRAIN_TILE_SIZE;

                result.distance     = (blockBottom - result.height) - m_position.y; 
                break;
            }
            case (SensorDirection::UP): {
                result.gridPosition = v2i(gridX, gridY - ext);
                result.block        = m_terrain.getBlockFromGrid(result.gridPosition.x, result.gridPosition.y, m_layer);
                result.height       = abs(m_terrain.getVerticalHeightInTile(m_position.x, result.block.tile));

                auto blockTop       = (result.gridPosition.y * TERRAIN_TILE_SIZE);

                result.distance     = m_position.y - (blockTop + result.height); 
                break;
            }
            case (SensorDirection::LEFT): {
                result.gridPosition = v2i(gridX - ext, gridY);
                result.block        = m_terrain.getBlockFromGrid(result.gridPosition.x, result.gridPosition.y, m_layer);
                result.height       = abs(m_terrain.getHorizontalHeightInTile(m_position.y, result.block.tile));

                auto blockLeft      = (result.gridPosition.x * TERRAIN_TILE_SIZE);

                result.distance     = m_position.x - (blockLeft + result.height); 
                break;
            }
            case (SensorDirection::RIGHT): {
                result.gridPosition = v2i(gridX + ext, gridY);
                result.block        = m_terrain.getBlockFromGrid(result.gridPosition.x, result.gridPosition.y, m_layer);
                result.height       = abs(m_terrain.getHorizontalHeightInTile(m_position.y, result.block.tile));

                auto blockRight     = (result.gridPosition.x * TERRAIN_TILE_SIZE) + TERRAIN_TILE_SIZE;

                result.distance     = (blockRight - result.height) - m_position.x;
                break;
            }
        }

        return result;
    }


    bool isBlockEmpty(BlockResult& blockResult) {
        return (blockResult.height == 0 || checkBlockSolidity(blockResult.block) == BlockSolidity::EMPTY);
    }

    BlockSolidity checkBlockSolidity(ChunkBlock& block) {
        return (m_layer == TerrainLayer::NORMAL) ? block.solidityNormalLayer : block.solidityAlternateLayer;
    }

    v2i getCurrentBlockPositionInGrid(const v2f& position) {
        return v2i(
            (int)(position.x / TERRAIN_TILE_SIZE),
            (int)(position.y / TERRAIN_TILE_SIZE)
        );
    }

    void drawDot(float x, float y, sf::Color color, Camera& cam) {
        sf::RectangleShape rect(sf::Vector2f(1, 1));
        rect.setFillColor(color);
        rect.setPosition(x, y);
        cam.getScr().getSfmlWindow().draw(rect);

        color.a = 100;

        ////////
        rect.setFillColor(color);

        rect.setPosition(x-1, y);
        cam.getScr().getSfmlWindow().draw(rect);

        rect.setPosition(x+1, y);
        cam.getScr().getSfmlWindow().draw(rect);

        rect.setPosition(x, y-1);
        cam.getScr().getSfmlWindow().draw(rect);

        rect.setPosition(x, y+1);
        cam.getScr().getSfmlWindow().draw(rect);
    }
};

}
