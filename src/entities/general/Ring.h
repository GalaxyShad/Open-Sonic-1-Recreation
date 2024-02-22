#pragma once
#include "../Entity.h"
#include "Terrain.h"

class Ring : public Entity
{
    public:
        enum Dir {DIR_RIGHT, DIR_UP, DIR_LEFT, DIR_DOWN}; 
        Ring(v2f _pos) : Entity(_pos) {}
        Ring(v2f _pos, Terrain& _trn, float _xsp, float _ysp) : Entity(_pos) 
        {
            xsp = _xsp;
            ysp = _ysp;
            trn = &_trn;
            bouncing = true;
            liveTimer = 256;
        }
        Ring(v2f _pos, uint8_t count, uint8_t dir, std::list<Entity*>& ent);
        ~Ring();
        void init();
        void update();
        bool isBounce() { return bouncing; }
        void draw(Camera& cam);
        void animate(int frame);

    private:
        bool bouncing = false;
        float xsp = 0;
        float ysp = 0;
        int liveTimer = 0;
        Terrain* trn;
};