#include "Bg.h"

void Bg::create(uint8_t _from, uint8_t _len) { 
    chunkFrom = _from; 
    chunkLen = _len; 
    chunkCur = chunkFrom;
};

void Bg::draw(Camera& cam, Terrain& trn) {
    tick++;
    for (int i = 0; i < layers.size(); i++)
        drawLayer(i, cam, trn);
}

void Bg::addLayer(int start, int end, float scrollRatio, float spd) {
    if (start < 0 || end < 0)
        return;

    layers.push_back(BgLayer(start, end, scrollRatio, spd));
}

void Bg::drawLayer(int layerInd, Camera& cam, Terrain& trn) {
    if (layerInd < 0)
        return;

    BgLayer& layer = layers[layerInd];

    float cx = cam.getPos().x;
    float cy = cam.getPos().y;

    static float dx = 0;
    float x = -cx * layer.scrollRatio - layer.spd * tick;

    float BackgroundStep  = 256*chunkLen;
    float BackgroundStart = cx+fmod(x - cx,  BackgroundStep)-BackgroundStep;
    float BackgroundEnd   = cx+cam.getSize().width+BackgroundStep;
   
    int n = (int)((BackgroundEnd-BackgroundStart)/BackgroundStep);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < chunkLen; j++)
            trn.drawChunkPart(cam, chunkFrom+j, Vector2f(BackgroundStart+j*256, cy+layer.start), IntRect(0, layer.start, 256, layer.size));
        BackgroundStart += BackgroundStep;
    }
}