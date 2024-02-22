#include "Ring.h"

Ring::Ring(v2f _pos, uint8_t count, uint8_t dir, std::list<Entity*>& ent) : Entity(_pos)
{
	for(int i = 1; i < count; i++) {
		switch(dir) {
			case DIR_RIGHT: 
				ent.push_back(new Ring(v2f(_pos.x + i * 24, _pos.y)));
				break;
			case DIR_LEFT:
				ent.push_back(new Ring(v2f(_pos.x - i * 24, _pos.y)));
				break;
			case DIR_DOWN:
				ent.push_back(new Ring(v2f(_pos.x, _pos.y + i * 24)));
				break;
			case DIR_UP:
				ent.push_back(new Ring(v2f(_pos.x, _pos.y - i * 24)));
				break;
		}
	}
}

void Ring::init()
{
	dv_hitBoxSize = v2f(16, 16);
	dv_type = TYPE_RING;
	dv_anim.create(TEX_OBJECTS);

}

void Ring::d_update()
{
	if (!bouncing) return;
	dv_pos.y += ysp;
	dv_pos.x += xsp;
	ysp += 0.09375;

	Tile tile = (*trn).getTile(v2i(dv_pos.x, dv_pos.y+8));
	if (tile.type != TILE_EMPTY && ysp >= 0) {
		if (dv_pos.y+8 >= tile.pos.y + 16 - tile.verHeight[int(dv_pos.x) - tile.pos.x])
			ysp *= -0.75;
	}

	if (liveTimer > 0)
		liveTimer--;
	else 
		d_destroy();

}

void Ring::animate(int frame)
{
	if (!bouncing)
	 	dv_anim.set(frame, frame, 0.0);
	else 
		dv_anim.set(frame, frame, (256 - liveTimer) / 64);
}

void Ring::d_draw(Camera& cam)
{
	cam.draw(dv_anim, dv_pos);
}

Ring::~Ring()
{

}
