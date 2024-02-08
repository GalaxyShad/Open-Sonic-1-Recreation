#pragma once

#include "../Entity.h"

class LayerSwitcher : public Entity 
{
    public:
        LayerSwitcher(v2f _pos, v2f _hBox, uint8_t _mode) : 
            Entity(_pos) { mode = _mode; hitBoxSize = _hBox;}
        void create() { type = TYPE_LAYER_SWITCH; }
        void update() {}
        void draw(Camera& cam) 
        { 
            // cam.getScr().drawRectangle(
            // Vector2f(pos.x - cam.getPos().x - hitBoxSize.x/2, 
            //             pos.y - cam.getPos().y - hitBoxSize.y/2), 
            // Size(hitBoxSize.x, hitBoxSize.y), 
            // 0x4440+0x4440*mode); 
        }
        uint8_t getMode() { return mode; };
    private:
        uint8_t mode;
};