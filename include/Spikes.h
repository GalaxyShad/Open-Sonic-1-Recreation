#pragma once

#include "Entity.h"
#include "Terrain.h"

#include <iostream>
using namespace std;

class Spikes : public Entity
{
    public:
        // enum Dir {DIR_RIGHT, DIR_UP, DIR_LEFT, DIR_DOWN}; 
        Spikes(Vector2f _pos) : Entity(_pos) {}
        Spikes(Vector2f _pos, uint8_t type, std::list<Entity*>& ent) : Entity(_pos)  {

            int spacing = 16;
            int count = 3;
            switch(type & 0xF0) {
                case 0x20: count = 1; break; // 1 hor
                case 0x30: spacing = 24; break; // 3 wide hor
                case 0x40: spacing = 24; count = 6; break; // 6 wide hor
                case 0x50: count = 1; break; // 1 ver
            }
            for(int i = 1; i < count / 3+1; i++) {
                ent.push_back(new Spikes(Vector2f(_pos.x + spacing * i, _pos.y)));
                ent.push_back(new Spikes(Vector2f(_pos.x - spacing * i, _pos.y)));
            }

            // living = false;
        }
        void create() { 
            solid = true; 
            platform = true;
            type = TYPE_SPIKES; 
            hitBoxSize = Vector2f(8, 32); 
            anim.create(TEX_OBJECTS); 
            anim.set(138, 138, 0);
        }
        //void update() { return; }
};