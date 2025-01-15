#pragma once

#include "../Entity.h"
#include "core/game_enviroment/artist/Animator.h"
#include "core/game_enviroment/artist/ArtistStructs.h"

#include <iostream>
using namespace std;

class Spikes : public Entity
{
    public:
        // enum Dir {DIR_RIGHT, DIR_UP, DIR_LEFT, DIR_DOWN}; 
        Spikes(v2f _pos, artist_api::Animation &anim) : Entity(_pos), animator_(anim), anim_(anim) {}
        Spikes(v2f _pos, artist_api::Animation &anim, uint8_t type, std::list<Entity*>& ent) : Entity(_pos), animator_(anim), anim_(anim)  {

            int spacing = 16;
            int count = 3;
            switch(type & 0xF0) {
                case 0x20: count = 1; break; // 1 hor
                case 0x30: spacing = 24; break; // 3 wide hor
                case 0x40: spacing = 24; count = 6; break; // 6 wide hor
                case 0x50: count = 1; break; // 1 ver
            }
            for(int i = 1; i < count / 3+1; i++) {
                ent.push_back(new Spikes(v2f(_pos.x + spacing * i, _pos.y), anim_));
                ent.push_back(new Spikes(v2f(_pos.x - spacing * i, _pos.y), anim_));
            }

            // living = false;
        }

        EntityTypeID type() override { return EntityTypeID::SPIKES; }

        void init() { 
            
            dv_solid = true; 
            dv_platform = true;
            dv_type = TYPE_SPIKES; 
            dv_hitBoxSize = v2f(8, 32); 
            animator_.setSpeed(0.f);
        }
        void draw(Camera &cam) {
            auto &spr = animator_.getCurrentFrame();
            cam.getScr().artist().drawSprite(spr, {.x = dv_pos.x - cam.getPos().x,
                                                        .y = dv_pos.y - cam.getPos().y});
        }
        //void loop() { return; }
    private:
        artist_api::Animator animator_;
        artist_api::Animation &anim_;
};