#pragma once

#include "../Entity.h"

class LayerSwitcher : public Entity 
{
    public:
        LayerSwitcher(v2f _pos, v2f _hBox, uint8_t _mode) : 
            Entity(_pos) { mode = _mode; dv_hitBoxSize = _hBox;}
        void init() { dv_type = TYPE_LAYER_SWITCH; }
        void d_update() {}
        void d_draw(Camera& cam) 
        { 
        }
        uint8_t getMode() { return mode; };

        EntityTypeID type() override { return EntityTypeID::LAYER_SWITCH; }
    private:
        uint8_t mode;
};