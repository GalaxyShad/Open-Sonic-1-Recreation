#include "Ring.h"
#include "core/GameMath.h"
#include "entities/general/Ring.h"

Ring* Ring::CreateRow(std::list<Entity *> &ent, terrain::Terrain &_trn, 
	v2f startPosition, int count, float direction, float spaceBetween
) {
	float sina = sin(radians(direction));
	float cosa = cos(radians(direction));

    auto ringPos = startPosition;

    for (int i = 0; i < count - 1; i++) {
		ent.push_back(new Ring(ringPos, _trn));

        ringPos.x += (16 + spaceBetween) * cosa;
        ringPos.y += (16 + spaceBetween) * sina;
	}

    return new Ring(ringPos, _trn);
}

void Ring::init() {
    dv_hitBoxSize = v2f(16, 16);
    dv_type = TYPE_RING;
    dv_anim.create(TEX_OBJECTS);
}

void Ring::d_update() {
    if (!bouncing)
        return;

    m_gndSensor.setPosition(v2f(dv_pos.x, dv_pos.y + 16));

    dv_pos.y += ysp;
    dv_pos.x += xsp;
    ysp += 0.09375;

    if (m_gndSensor.getDistance() <= 0)
        ysp *= -0.75;

    if (liveTimer > 0)
        liveTimer--;
    else
        d_destroy();
}

void Ring::animate(int frame) {
    if (!bouncing)
        dv_anim.set(frame, frame, 0.0);
    else
        dv_anim.set(frame, frame, (256 - liveTimer) / 64);
}

void Ring::d_draw(Camera &cam) { cam.draw(dv_anim, dv_pos); }
