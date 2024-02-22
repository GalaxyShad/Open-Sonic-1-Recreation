#include "Spring.h"

void Spring::init()
{
	dv_type = TYPE_SPRING;
    dv_solid = true;

	if (rot == R_UP || rot == R_DOWN)
		dv_hitBoxSize = v2f(28, 16);
	else {
		dv_platform = true;
		dv_hitBoxSize = v2f(16, 28);
	}

	dv_anim.create(TEX_OBJECTS);
	if (red)
		dv_anim.set(65, 65, 0.0);
	else
		dv_anim.set(67, 67, 0.0);
}

void Spring::doAnim()
{
	if (red)
		dv_anim.set(66, 66, 0);
	else
		dv_anim.set(68, 68, 0);
	animTimer = SPRING_ANIM_TIMER;
}

void Spring::update()
{
	if (animTimer > 0) {
		animTimer--;
	} else {
		if (red)
			dv_anim.set(65, 65, 0);
		else
			dv_anim.set(67, 67, 0);
	}
}

void Spring::draw(Camera& cam) {
	switch (rot) {
		case R_UP: cam.draw(dv_anim, dv_pos, 0); break;
		case R_RIGHT: cam.draw(dv_anim, dv_pos, 90); break;
		case R_DOWN: cam.draw(dv_anim, dv_pos, 180); break;
		case R_LEFT: cam.draw(dv_anim, dv_pos, 270); break;
	}
}

