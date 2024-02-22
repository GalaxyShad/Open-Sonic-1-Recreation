#pragma once
#include "../Entity.h"

#define SPRING_ANIM_TIMER 15

class Spring : public Entity 
{
    public:
		enum Rotation {R_UP, R_RIGHT, R_DOWN, R_LEFT};
		Spring(v2f _pos, bool _red = false, Rotation rotation = R_UP) : 
			Entity(_pos), red(_red), rot(rotation) {};
        void init();
		void d_update();
		void d_draw(Camera& cam);
		void doAnim();
		bool isRed() { return red; }
		Rotation getRotation() { return rot; }
	private:
		int animTimer = SPRING_ANIM_TIMER;
		bool red = false;
		Rotation rot;
        
};