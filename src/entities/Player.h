#pragma once

#include "../core/_index.hpp"

#include "AnimMgr.h"
#include "Camera.h"

#include "Entity.h"

#include "new-terrain.hpp"
#include "player-collider.hpp"
#include "player-state-base.hpp"
#include "player-state-normal.hpp"
#include "terrain-sensor.hpp"

#include "general/_index.hpp"
#include "sonic-1/_index.hpp"

#include "../player-collider.hpp"
#include "../player-state-base.hpp"

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
            , m_collider(dv_pos, spd, gsp, _trn)
            , m_stateMachine(m_props)
        { };
        void init() override;
        void terrainCollision(Camera& cam);
		void entitiesCollision(std::list<Entity*>& entities, Camera& cam);
        void d_update() override;
        void d_draw(Camera& cam) override;
		void moveCam(Camera& cam);
        bool isEndLv() {return endLv;}
        bool isDied() { return dead; }
    private:
        PlayerCollider m_collider;
        PlayerStateMachine m_stateMachine;


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

        // controlls
        bool canHorMove = true;

        bool debug = false;

        // movement
        float gsp = 0.0;
        float angle = 0;
        float shiftX = 0;
        float shiftY = 0;
        int enemyCombo = 0;

        Entity* standOn;

        // gameplay
		bool isSpindashDirRight = false;
        bool standOnObj = false;
        bool sTube = false;
        bool endLv = false;
        bool dead = false;

        // timers
        int ringTimer = 0;
        int horizontalLockTimer = 0;
        int camLagTimer = 0;
        int invicTimer = 0;

        // animation
        bool animFlip = false;
		float anim8Angle = 0.0;
        float animAngle = 0.0;
        bool diaAnim = false;

        int layer = 0;

		// debug
		irect lSenRect, rSenRect;
        bool isDebugPressed = false;

        PlayerStateProps m_props {
            dv_pos, spd, gsp, input, audio, dv_anim, m_collider, m_stateMachine
        };

        // === functions === //
        void gameplay();
        void movement();
        void animation();
        void getHit(std::list<Entity*>& entities);

};