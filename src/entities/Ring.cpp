#include "Ring.h"

Ring::Ring(Vector2f _pos, uint8_t count, uint8_t dir, std::list<Entity*>& ent) : Entity(_pos)
{
	for(int i = 1; i < count; i++) {
		switch(dir) {
			case DIR_RIGHT: 
				ent.push_back(new Ring(Vector2f(_pos.x + i * 24, _pos.y)));
				break;
			case DIR_LEFT:
				ent.push_back(new Ring(Vector2f(_pos.x - i * 24, _pos.y)));
				break;
			case DIR_DOWN:
				ent.push_back(new Ring(Vector2f(_pos.x, _pos.y + i * 24)));
				break;
			case DIR_UP:
				ent.push_back(new Ring(Vector2f(_pos.x, _pos.y - i * 24)));
				break;
		}
	}
}

void Ring::create()
{
	hitBoxSize = Vector2f(16, 16);
	type = TYPE_RING;
	anim.create(TEX_OBJECTS);

}

void Ring::update()
{
	if (!bouncing) return;
	pos.y += ysp;
	pos.x += xsp;
	ysp += 0.09375;

	Tile tile = (*trn).getTile(Vector2i(pos.x, pos.y+8));
	if (tile.type != TILE_EMPTY && ysp >= 0) {
		if (pos.y+8 >= tile.pos.y + 16 - tile.verHeight[int(pos.x) - tile.pos.x])
			ysp *= -0.75;
	}

	if (liveTimer > 0)
		liveTimer--;
	else 
		destroy();

}

void Ring::animate(int frame)
{
	if (!bouncing)
	 	anim.set(frame, frame, 0.0);
	else 
		anim.set(frame, frame, (256 - liveTimer) / 64);
}

void Ring::draw(Camera& cam)
{
	cam.draw(anim, pos);
}

Ring::~Ring()
{

}
