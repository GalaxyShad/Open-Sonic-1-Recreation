#pragma once

#include <vector>

#include "Camera.h"
#include "terrain-drawer.hpp"

struct BgLayer {
    int start;
    int size;
    float scrollRatio;
    float spd;

    BgLayer(int start, int size, float scrollRatio, float spd) : 
        start(start), size(size), scrollRatio(scrollRatio), spd(spd) { }
        
};

class Bg {
    public:
        Bg(terrain::TerrainDrawer& terrainDrawer) : m_terrainDrawer(terrainDrawer) {}

        void create(uint8_t _from, uint8_t _len);
        void draw(Camera& cam);
        void addLayer(int start, int end, float scrollRatio, float spd = 0.0);
    private:
        terrain::TerrainDrawer& m_terrainDrawer;

        uint8_t chunkFrom;
        uint8_t chunkCur;
        uint8_t chunkLen;
        int tick = 0;
        std::vector<BgLayer> layers;

        void drawLayer(int layer, Camera& cam);

};
