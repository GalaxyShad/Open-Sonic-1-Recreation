#include "Spring.h"

void Spring::create()
{
	type = TYPE_SPRING;
    solid = true;

	if (rot == R_UP || rot == R_DOWN)
		hitBoxSize = Vector2f(28, 16);
	else {
		platform = true;
		hitBoxSize = Vector2f(16, 28);
	}

	anim.create(TEX_OBJECTS);
	if (red)
		anim.set(65, 65, 0.0);
	else
		anim.set(67, 67, 0.0);
}

void Spring::doAnim()
{
	if (red)
		anim.set(66, 66, 0);
	else
		anim.set(68, 68, 0);
	animTimer = SPRING_ANIM_TIMER;
}

void Spring::update()
{
	if (animTimer > 0) {
		animTimer--;
	} else {
		if (red)
			anim.set(65, 65, 0);
		else
			anim.set(67, 67, 0);
	}
}

void Spring::draw(Camera& cam) {
	switch (rot) {
		case R_UP: cam.draw(anim, pos, 0); break;
		case R_RIGHT: cam.draw(anim, pos, 90); break;
		case R_DOWN: cam.draw(anim, pos, 180); break;
		case R_LEFT: cam.draw(anim, pos, 270); break;
	}
}

