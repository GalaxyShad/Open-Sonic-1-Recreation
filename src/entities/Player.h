#pragma once

#include "../core/_index.hpp"

#include "Terrain.h"
#include "AnimMgr.h"
#include "Camera.h"

#include "Entity.h"

#include "new-terrain.hpp"
#include "player-collider.hpp"
#include "terrain-sensor.hpp"

#include "general/_index.hpp"
#include "sonic-1/_index.hpp"

#include "../player-collider.hpp"


// === Constants === //

// = physics =
// normal
#define PL_ACC              0.046875 
#define PL_DEC              0.5 
#define PL_FRC              0.046875
#define PL_TOP              6
#define PL_JMP              6.5
#define PL_GRAV             0.21875
#define PL_AIR              0.09375 

#define PL_SLP              0.125 
#define PL_SLP_ROLL_UP      0.078125 
#define PL_SLP_ROLL_DOWN    0.3125 

#define PL_FRC_ROLL         0.0234375 
#define PL_DEC_ROLL         0.125 

#define PL_FOOT_LEVEL       20

using namespace gmath;

class Player : public Entity
{
    public:
		Player(v2f _pos, terrain::Terrain& _trn, IInputMgr& input, Audio& audio, int& rings, int& score) 
            : Entity(_pos)
            , input(input)
            , audio(audio)
            , rings(rings)
            , score(score) 
            , m_collider(pos, spd, gsp, _trn)
        { };
        void create();
        void terrainCollision(Camera& cam);
		void entitiesCollision(std::list<Entity*>& entities, Camera& cam);
        void update();
        void draw(Camera& cam);
		void moveCam(Camera& cam);
        bool isEndLv() {return endLv;}
        bool isDied() { return dead; }
    private:
        PlayerCollider m_collider;

        IInputMgr& input;
        Audio& audio;

        v2f spd = v2f(0.f, 0.f);

        int& rings;
        int& score;

        // enums
        enum FlrMode { 
            FLOOR, 
            RIGHT_WALL, 
            BOTTOM, 
            LEFT_WALL
        };
        enum Action { 
            ACT_NORMAL, 
            ACT_JUMP, 
            ACT_ROLL, 
            ACT_SKID, 
            ACT_SPINDASH,
			ACT_SPRING,
            ACT_HURT,
            ACT_DIE,
        };
        // controlls
        bool canHorMove = true;

        bool debug = false;

        // movement
        float gsp = 0.0;
        float angle = 0;
        float shiftX = 0;
        float shiftY = 0;
        bool ground = false;
        int gndHeight = 20; // Ground sensor height
        int gndWidth = 7;   // Ground sensors width
        int enemyCombo = 0;

        Entity* standOn;

        // gameplay
        Action action = ACT_NORMAL;
		bool isSpindashDirRight = false;
        bool standOnObj = false;
        bool sTube = false;
        bool endLv = false;
        bool dead = false;

        // timers
        int spindashTimer = 0;
        int ringTimer = 0;
        int horizontalLockTimer = 0;
        int camLagTimer = 0;
        int invicTimer = 0;

        // animation
        bool animFlip = false;
		float anim8Angle = 0.0;
        float animAngle = 0.0;
        int animIdleTimer = 288;
        bool diaAnim = false;

        int layer = 0;

		// debug
		irect lSenRect, rSenRect;
        bool isDebugPressed = false;

        // === functions === //
        void gameplay();
        void movement();
        void animation();
        void getHit(std::list<Entity*>& entities);

};