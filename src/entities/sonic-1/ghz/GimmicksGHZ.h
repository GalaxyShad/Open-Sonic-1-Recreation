#pragma once

#include "../../Entity.h"

#include "core/Geometry.h"
#include "core/game_enviroment/artist/Animator.h"
#include "core/game_enviroment/artist/ArtistStructs.h"

#define TEX_OBJECTS 0
#define TEX_GHZ_GIMM 1
#define TEX_GHZ_TILES 2

class GimGHZ_Stone : public Entity
{
    public: GimGHZ_Stone(v2f _pos, artist_api::Sprite &spr) : Entity(_pos), spr_(spr) {}
        void init();
        void draw(Camera &cam) override {
            cam.getScr().artist().drawSprite(spr_, {.x = dv_pos.x - cam.getPos().x,
                                                   .y = dv_pos.y - cam.getPos().y});
        }
        EntityTypeID type() override { return EntityTypeID::DEPRECATED; }
    private:
        artist_api::Sprite spr_;
};

class GimGHZ_Bridge : public Entity
{   
    public: GimGHZ_Bridge();
        GimGHZ_Bridge(v2f _pos, artist_api::Sprite &spr) : Entity(_pos), spr_(spr) {}
        bool isActive() { return active; }
        void setActive(bool _active) { active = _active;}
        void init();
        void draw(Camera &cam) override {
            cam.getScr().artist().drawSprite(spr_, {.x = dv_pos.x - cam.getPos().x,
                                                   .y = dv_pos.y - cam.getPos().y});
        }
        float getY();

        int maxDepression = 0;
        int number = 0;
        int count = 0;

        EntityTypeID type() override { return EntityTypeID::DEPRECATED; }
    private:
        artist_api::Sprite spr_;
        bool active = false;
};

class GimGHZ_BridgeController : public Entity
{
    public: GimGHZ_BridgeController(v2f _pos, artist_api::Sprite &spr, uint8_t count, std::list<Entity*>& ent);
        void init();
        void d_update();
        void d_draw(Camera& cam) {}
        EntityTypeID type() override { return EntityTypeID::DEPRECATED; }
    private:
        artist_api::Sprite &spr_;
        GimGHZ_Bridge** segments;
        int segmentsCount;
        int curSegment = 0;
        int angle = 0;
};


class GimGhz_BridgeColumn : public Entity {
    public:
        GimGhz_BridgeColumn(v2f _pos, artist_api::Sprite &spr, bool flip) : 
            Entity(_pos), spr_(spr), flip(flip) { }
        void init() {}
        void draw(Camera &cam) override {
            cam.getScr().artist().drawSprite(spr_, {.x = dv_pos.x - cam.getPos().x,
                                                   .y = dv_pos.y - cam.getPos().y}, {.flipHorizontal=flip});
        }
        void d_draw(Camera& cam) {
            // cam.draw(dv_anim, dv_pos, 0, flip);
        }
        EntityTypeID type() override { return EntityTypeID::DEPRECATED; }
    private:
        artist_api::Sprite spr_;
        bool flip = false;
};


class GimGHZ_Platform : public Entity 
{
    public: 
        GimGHZ_Platform(v2f _pos, artist_api::Sprite &spr, int _dir = 0, bool _mooving = false, bool _canFall = false) : Entity(_pos), spr_(spr) 
            { dir = _dir; mooving = _mooving; canFall = _canFall; }
        void init();
        void d_update();
        void draw(Camera &cam) override {
            cam.getScr().artist().drawSprite(spr_, {.x = dv_pos.x - cam.getPos().x,
                                                   .y = dv_pos.y - cam.getPos().y});
        }
        int getDir() {return dir;}
        float getSpd() { return xsp; }

        void onOutOfView() override {
            init();            
        }


        bool isCanFall()  { return canFall; }
        bool isFalling()  { return falling; }
        void setFalling(bool flag) { if (falling != flag) {falling = flag; fallTimer = 10;} }

        EntityTypeID type() override { return EntityTypeID::DEPRECATED; }

        enum {DIR_RIGHT, DIR_LEFT, DIR_UP, DIR_DOWN};
    private:
        artist_api::Sprite spr_;
        int dir = 0; // 0 - right 1 - left 2 - up 3 - down
        float angle;
        float xsp = 0;
        float ysp = 0;
        bool mooving = false;

        bool canFall = false;
        bool falling = false;
        int fallTimer = -1;

        const float spd = 1;
};

class GimGHZ_SlpPlatform : public Entity {
    public: 
        GimGHZ_SlpPlatform(v2f _pos, artist_api::Sprite &spr, std::list<Entity*>& ent, 
                           bool _left = false) : Entity(_pos), spr_(spr), ent(ent) { isLeft = _left; }
        void init();
        void d_update();
        void draw(Camera &cam) override {
            cam.getScr().artist().drawSprite(spr_, {.x = dv_pos.x - cam.getPos().x,
                                                   .y = dv_pos.y - cam.getPos().y}, {.flipHorizontal=isLeft});
        }
        void d_draw(Camera& cam) {
            // cam.draw(dv_anim, dv_pos, 0.0, isLeft);
            // cam.getScr().artist().drawSprite(spr_, {.x = dv_pos.x - cam.getPos().x,
            //                                        .y = dv_pos.y - cam.getPos().y}, {.flipHorizontal=isLeft});
        }
        void d_destroy() { if (deathTimer < 0) deathTimer = 32; } 
        
        EntityTypeID type() override { return EntityTypeID::DEPRECATED; }

        int getHeight(int x) 
        { 
            if (!isLeft) {
                if (x >= 0 && x < 96) 
                    return heights[x]; 
                else  if (x < 0)
                    return 13; 
                else 
                    return 29;
            } else {
                if (x >= 0 && x < 96) 
                    return heights[95 - x]; 
                else  if (x < 0)
                    return 29; 
                else 
                    return 13;    
            }
        }
    private:
        artist_api::Sprite &spr_;
        int deathTimer;
        bool isLeft;
        int heights[96] = { 
            13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
            13, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 15, 16, 16, 16, 16,
            17, 17, 17, 17, 18, 18, 18, 18, 19, 19, 19, 19, 20, 20, 20, 20,
            21, 21, 21, 21, 22, 22, 22, 22, 23, 23, 23, 23, 24, 24, 24, 24,
            25, 25, 25, 25, 26, 26, 26, 26, 27, 27, 27, 27, 28, 28, 28, 28,
            29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29  
        };
        std::list<Entity*>& ent;
};

class GimGHZ_SlpPlatformPart : public Entity {
    public: 
        GimGHZ_SlpPlatformPart(v2f _pos, artist_api::Sprite &spr, int partIndex, bool _left = false) : 
            Entity(_pos), spr_(spr), partIndex(partIndex), left(_left) 
            { timer = (35 - partIndex); dv_type = TYPE_PARTICLE; }
        void d_update() { 
            if (timer > 0) timer--; 
            else ysp += 0.2;

            dv_pos.y += ysp;
        }
        void draw(Camera &cam) override {            
            cam.getScr().artist().drawSprite(spr_, {.x = dv_pos.x - cam.getPos().x,
                                                   .y = dv_pos.y - cam.getPos().y}, {.flipHorizontal=left});
        }
        void d_draw(Camera& cam) {
            // cam.draw(
            //     TEX_GHZ_GIMM, 
            //     irect(48+(partIndex % 6)*16,
            //             (partIndex / 6)*16,
            //             16, 16), 
            //     dv_pos, v2i(0,0), 0.0, left);
        }
        EntityTypeID type() override { return EntityTypeID::DEPRECATED; }
    private:
        artist_api::Sprite spr_;
        int partIndex;
        int timer;
        float ysp = 0;
        bool left;

};

class GimGHZ_Wall : public Entity
{
    public: 
        GimGHZ_Wall(v2f _pos, artist_api::Sprite &spr, int _ty, bool _ghost=false) : Entity(_pos), spr_(spr) {ty = _ty; dv_solid = !_ghost; }
        void init() {
            dv_hitBoxSize = v2f(16, 64);
            // dv_anim.create(TEX_GHZ_GIMM);
            // dv_anim.set(4 + ty, 4 + ty, 0);    
            //solid = true;
        };
        void draw(Camera &cam) override {
            cam.getScr().artist().drawSprite(spr_, {.x = dv_pos.x - cam.getPos().x,
                                                    .y = dv_pos.y - cam.getPos().y});
        }
        EntityTypeID type() override { return EntityTypeID::DEPRECATED; }

    private:
        artist_api::Sprite spr_;
        int ty = 0;
};

class GimGHZ_STube : public Entity
{
   public:
        GimGHZ_STube(v2f _pos, uint8_t _mode) : Entity(_pos) { mode = _mode;}
        void init() {
            dv_type = TYPE_STUBE_CNTRL;
            dv_hitBoxSize = v2f(16, 64);
        }
        void d_update() {}
        void draw(Camera &cam) override {
            // cam.getScr().artist().drawSprite(spr_, {.x = dv_pos.x - cam.getPos().x,
            //                                        .y = dv_pos.y - cam.getPos().y});
        }
        void d_draw(Camera& cam) {
            // cam.getScr().drawRectangle(
            //     Vector2f(dv_pos.x - cam.getPos().x - hitBoxSize.x/2, dv_pos.y - cam.getPos().y - hitBoxSize.y/2), 
            //     Size(hitBoxSize.x, hitBoxSize.y), 
            // 0x4440+0x4440*mode); 
         }
        uint8_t getMode() {return mode; };
        EntityTypeID type() override { return EntityTypeID::DEPRECATED; }

    private:
        // artist_api::Sprite spr_;
        uint8_t mode;
};

struct SignPostAnimations {
    artist_api::Animation &animEggman;
    artist_api::Animation &animSpin;
    artist_api::Animation &animSonic;
    artist_api::Animation &animStick;
};

class SignPost : public Entity
{
   public:
        SignPost(v2f _pos, SignPostAnimations &anims) : Entity(_pos), animator_(anims.animEggman), animEggman_(anims.animEggman), animSpin_(anims.animSpin), animSonic_(anims.animSonic), animatorStick_(anims.animStick) {}
        void init() {
            animCount = 0;
            dv_type = TYPE_SIGN_POST; 
            dv_hitBoxSize = v2f(48, 48);
            animator_.setSpeed(0.0f);
        }
        void draw(Camera& cam) {
            auto &spr = animator_.getCurrentFrame();
            cam.getScr().artist().drawSprite(spr, {.x = dv_pos.x - cam.getPos().x,
                                                   .y = dv_pos.y - cam.getPos().y});
            auto &sprStick_ = animatorStick_.getCurrentFrame();
            cam.getScr().artist().drawSprite(sprStick_, {.x = dv_pos.x - cam.getPos().x,
                                                   .y = dv_pos.y + 24 - cam.getPos().y}); 
        }
        void d_update() {
            if(spin_){
                if(animCount==0){
                    animator_.changeTo(animSpin_);
                    animator_.setSpeed(0.5f);
                }
                if (animCount < 108) {
                    // if (dv_anim.getCurFrame() >= 108)
                    animCount++;
                }
                else {
                    animator_.changeTo(animSonic_);
                    animator_.setSpeed(0.0f);
                    // animCount = 50;
                }
            }
            animator_.tick();
        }
        EntityTypeID type() override { return EntityTypeID::DEPRECATED; }

        void setAnim(bool spin) { spin_=spin; }


    private:
        artist_api::Animator animator_;
        artist_api::Animation &animEggman_;
        artist_api::Animation &animSpin_;
        artist_api::Animation &animSonic_;
        artist_api::Animator animatorStick_;
        int spin_ = false;
        int animCount = 0;
};
