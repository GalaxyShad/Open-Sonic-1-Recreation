#pragma once
#include "../Entity.h"
#include "core/game_enviroment/artist/Animator.h"
#include "core/game_enviroment/artist/ArtistStructs.h"

#define SPRING_ANIM_TIMER 15


struct SpringAnimations {
    artist_api::Animation &anim;
    artist_api::Animation &animExpand;
};

class Spring : public Entity 
{
    public:
		enum Rotation {R_UP, R_RIGHT, R_DOWN, R_LEFT};
		Spring(v2f _pos, SpringAnimations anims, bool _red = false, Rotation rotation = R_UP) : 
			Entity(_pos), animator_(anims.anim), anim_(anims.anim), animExpand_(anims.animExpand), red(_red), rot(rotation) {};
        void init();
		void d_update();
		void d_draw(Camera& cam);
		void doAnim();
		bool isRed() { return red; }
		Rotation getRotation() { return rot; }
		EntityTypeID type() override { return EntityTypeID::SPRING; }
	private:
		artist_api::Animator animator_;
		artist_api::Animation anim_;
		artist_api::Animation animExpand_;
		int animTimer = SPRING_ANIM_TIMER;
		bool red = false;
		Rotation rot;
        
};