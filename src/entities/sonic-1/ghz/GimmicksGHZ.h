#pragma once

#include "../../Entity.h"
#include "Terrain.h"

#include "core/Geometry.h"

#define TEX_OBJECTS 0
#define TEX_GHZ_GIMM 1
#define TEX_GHZ_TILES 2

class GimGHZ_Stone : public Entity
{
    public: GimGHZ_Stone(Vector2f _pos) : Entity(_pos) {}
            void create();
};

class GimGHZ_Bridge : public Entity
{   
    public: GimGHZ_Bridge();
            GimGHZ_Bridge(Vector2f _pos) : Entity(_pos) {}
            bool isActive() { return active; }
            void setActive(bool _active) { active = _active;}
            void create();
            float getY();

            int maxDepression = 0;
            int number = 0;
            int count = 0;
    private:
            bool active = false;
};

class GimGHZ_BridgeController : public Entity
{
    public: GimGHZ_BridgeController(Vector2f _pos, uint8_t count, std::list<Entity*>& ent);
            void create();
            void update();
            void draw(Camera& cam) {}
    private:
            GimGHZ_Bridge** segments;
            int segmentsCount;
            int curSegment = 0;
            int angle = 0;
};

class GimGhz_BridgeColumn : public Entity {
    public:
        GimGhz_BridgeColumn(Vector2f _pos, bool flip) : 
            Entity(_pos), flip(flip) { }
        void create() {
            anim.create(TEX_GHZ_GIMM); 
            anim.set(7, 7, 0);
        }
        void draw(Camera& cam) { cam.draw(anim, pos, 0, flip); }
    private:
        bool flip = false;
};

class GimGHZ_Platform : public Entity 
{
    public: 
        GimGHZ_Platform(Vector2f _pos, int _dir = 0, bool _mooving = false, bool _canFall = false) : Entity(_pos) 
            { dir = _dir; mooving = _mooving; canFall = _canFall; }
        void create();
        void update();
        int getDir() {return dir;}
        float getSpd() { return xsp; }

        bool isCanFall()  { return canFall; }
        bool isFalling()  { return falling; }
        void setFalling(bool flag) { if (falling != flag) {falling = flag; fallTimer = 10;} }

        enum {DIR_RIGHT, DIR_LEFT, DIR_UP, DIR_DOWN};
    private:
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
        GimGHZ_SlpPlatform(Vector2f _pos, std::list<Entity*>& ent, 
                           bool _left = false) : Entity(_pos), ent(ent) { isLeft = _left; }
        void create();
        void update();
        void draw(Camera& cam) { cam.draw(anim, pos, 0.0, isLeft); }
        void destroy() { if (deathTimer < 0) deathTimer = 32; } 
        
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
        GimGHZ_SlpPlatformPart(Vector2f _pos, int partIndex, bool _left = false) : 
            Entity(_pos), partIndex(partIndex), left(_left) 
            { timer = (35 - partIndex); type = TYPE_PARTICLE; }
        void update() { 
            if (timer > 0) timer--; 
            else ysp += 0.2;

            pos.y += ysp;
        }
        void draw(Camera& cam) {
            cam.draw(
                TEX_GHZ_GIMM, 
                IntRect(48+(partIndex % 6)*16,
                        (partIndex / 6)*16,
                        16, 16), 
                pos, Vector2i(0,0), 0.0, left);
        }
    private:
        int partIndex;
        int timer;
        float ysp = 0;
        bool left;

};

class GimGHZ_Wall : public Entity
{
    public: 
        GimGHZ_Wall(Vector2f _pos, int _ty, bool _ghost=false) : Entity(_pos) {ty = _ty; solid = !_ghost; }
        void create() {
            hitBoxSize = Vector2f(16, 64);
            anim.create(TEX_GHZ_GIMM);
            anim.set(4 + ty, 4 + ty, 0);
            //solid = true;
        };
    private: 
        int ty = 0;
};

class GimGHZ_STube : public Entity
{
   public:
        GimGHZ_STube(Vector2f _pos, uint8_t _mode) : Entity(_pos) { mode = _mode;}
        void create() {type = TYPE_STUBE_CNTRL; hitBoxSize = Vector2f(16, 64);}
        void update() {}
        void draw(Camera& cam) { 
            // cam.getScr().drawRectangle(
            // Vector2f(pos.x - cam.getPos().x - hitBoxSize.x/2, 
            //             pos.y - cam.getPos().y - hitBoxSize.y/2), 
            // Size(hitBoxSize.x, hitBoxSize.y), 
            // 0x4440+0x4440*mode); 
         }
        uint8_t getMode() {return mode; };
    private:
        uint8_t mode;
};

class SignPost : public Entity
{
   public:
        SignPost(Vector2f _pos) : Entity(_pos) {}
        void create() {
            animCount = 0;
            type = TYPE_SIGN_POST; 
            hitBoxSize = Vector2f(48, 48); 
            anim.create(TEX_OBJECTS);
            animPost.create(TEX_OBJECTS);
            anim.set(105, 105, 0); 
            animPost.set(104, 104, 0); 
            }
        void draw(Camera& cam) {
            cam.draw(animPost, Vector2f(pos.x, pos.y+24));
            cam.draw(anim, Vector2f(pos.x, pos.y));
        }
        void setAnim(bool spin) {
            if (animCount < 10) {
                if (anim.getCurFrame() >= 108)
                    animCount++;
                anim.set(105, 108, 0.5); 
            }
            else 
            {
                animCount = 50;
                anim.set(109, 109, 0); 
            }
        }
    private:
        AnimMgr animPost;
        int animCount = 0;
};
