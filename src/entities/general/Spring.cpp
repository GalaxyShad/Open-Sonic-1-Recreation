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
	animator_.setSpeed(0);
}

void Spring::doAnim()
{
	animTimer = SPRING_ANIM_TIMER;
}

void Spring::d_update()
{
	if (animTimer==0) animator_.changeTo(anim_);
	if (animTimer==15) animator_.changeTo(animExpand_);
	animTimer--;

}

void Spring::d_draw(Camera& cam) {
	auto &spr = animator_.getCurrentFrame();
	float angle = 0;
	switch (rot) {
		case R_UP: angle = 0; break;
		case R_RIGHT: angle = 90; break;
		case R_DOWN: angle = 180; break;
		case R_LEFT: angle = 270; break;
	}
	cam.getScr().artist().drawSprite(spr, {.x = dv_pos.x - cam.getPos().x,
										   .y = dv_pos.y - cam.getPos().y}, {.angle = angle});
}

