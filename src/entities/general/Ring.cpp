#include "Ring.h"
#include "core/GameMath.h"
#include "entities/Player.h"
#include "entities/general/Ring.h"
#include "entity-pool.hpp"

Ring* Ring::CreateRow(EntityPool& entityPool, terrain::Terrain &_trn, 
	v2f startPosition, int count, float direction, float spaceBetween
) {
	float sina = sin(radians(direction));
	float cosa = cos(radians(direction));

    auto ringPos = startPosition;

    for (int i = 0; i < count - 1; i++) {
        entityPool.create(new Ring(ringPos, entityPool, _trn));

        ringPos.x += (16 + spaceBetween) * cosa;
        ringPos.y += (16 + spaceBetween) * sina;
	}

    return new Ring(ringPos, entityPool, _trn);
}

void Ring::init() {
    m_anim.create(TEX_OBJECTS);
    m_anim.set(80, 80, 0.0);
}

void Ring::update() {
    float frame = 80 + fmod(GameLoopTicker::instance().getTickFloat() / 10, 4);

    m_anim.set(frame, frame, (m_bouncing) ? ((256 - m_liveTimer) / 64) : 0.0);

    if (!m_bouncing)
        return;

    m_pos.x += m_spd.x;
    m_pos.y += m_spd.y;
    
    m_spd.y += 0.09375;

    m_gndSensor.setPosition(v2f(m_pos.x, m_pos.y + 16));
    if (m_gndSensor.getDistance() <= 0)
        m_spd.y *= -0.75;

    if (m_liveTimer > 0)
        m_liveTimer--;
}

void Ring::draw(Camera &cam) { 
    cam.draw(m_anim, m_pos); 
}

void Ring::onHitboxCollision(Entity &entity) {
    if (entity.type() != EntityTypeID::PLAYER)
        return;

    auto& pl = entity.castTo<Player>();

    pl.tuduring();    
}
