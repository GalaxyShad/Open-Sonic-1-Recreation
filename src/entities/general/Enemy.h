#pragma once

#include "../Entity.h"
#include "core/game_enviroment/artist/Animator.h"
#include "core/game_enviroment/artist/ArtistStructs.h"

class Enemy : public Entity
{
    public:
        Enemy(v2f _pos, artist_api::Animation &animExplosion) : Entity(_pos), animExplosion_(animExplosion) {}
        void setAbilityToDying(bool _canDie) { canDie = _canDie; };
        void onOutOfView() override {
            dv_pos = dv_startPos;
        }
        artist_api::Animation &getAnimationExplosion(){ return animExplosion_; }
    private:    
        artist_api::Animation &animExplosion_;
        bool canDie = true;
};

class EnemyScore : public Entity {
    public:
        enum Points { P_100, P_200, P_500, P_1000 };
        EnemyScore(v2f _pos, artist_api::Animation &anim, Points points) : //
            Entity(_pos), animator_(anim), points(points) { init(); }//

        void init() {
            dv_pos.y -= 3;
            tick = 0;
            // dv_anim.create(TEX_OBJECTS);
            // dv_anim.set(70+points, 70+points, 0);
            animator_.setSpeed(0.0f);
        }

        void draw(Camera &cam){
            auto &spr = animator_.getCurrentFrame();
            cam.getScr().artist().drawSprite(spr, {.x = dv_pos.x - cam.getPos().x,
                                                   .y = dv_pos.y - cam.getPos().y});
        }

        void d_update() {
            if (tick < 30)  
                dv_pos.y -= 2;
            if (tick > 40)
                dv_living = false;

            tick++;
        }

        EntityTypeID type() override { return EntityTypeID::DEPRECATED; }

    private:
        artist_api::Animator animator_;
        Points points;
        int tick;
};