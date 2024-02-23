#include "Level.h"

#include "entity-creator.hpp"
#include <cstdio>


void Level::create() {
	if (m_gameType == GameType::SONIC_1) {
		trn.createLayeringObjs(m_entities);
	}

	lvInformer = new LevelInformer(m_zoneName.c_str(), m_act, m_screen, m_audio, LevelInformer::T_TITLE_CARD);

	if (m_gameType == GameType::SONIC_1) {
        createZoneSpecific();
    }

	// Create player
	Player* pl = new Player(m_playerStartPosition, m_terrain, m_input, m_audio, rings, score);
	m_entities.push_back(pl);	

	// Create camera
	auto screenSize = m_screen.getSize();
	auto cameraPos = v2f(
		m_playerStartPosition.x - (float)screenSize.width  / 2, 
		m_playerStartPosition.y - (float)screenSize.height / 2
	);
	auto levelSize = m_terrain.getLayout().getSizeInPixels();

    cam.create(cameraPos, Size(levelSize.x, levelSize.y), (m_gameType == GameType::SONIC_3K));

	for (it = m_entities.begin(); it != m_entities.end(); it++)
		(*it)->init();

	// Level Variables
	ringFrame = 80.0; // Using for same animation to all rings

	tick = 0;
	rings = 0;

	end = false;
	isTimeStopped = false;
	isFadeOut = false;
	isFadeDeath = false;
	fade = 0;
}

void Level::createZoneSpecific()
{
    if (m_zoneNameShort == "GHZ")
    {
        m_screen.setBgColor(0x08E0);
        bg.create(0x3C, 3);
        bg.addLayer(0, 32, 0.001, 0.5);
        bg.addLayer(32, 16, 0.002, 0.25);
        bg.addLayer(48, 16, 0.003, 0.125);
        bg.addLayer(64, 48, 0.005);
        bg.addLayer(112, 40, 0.060);
        const int waterDiv = 8;
        for (int i = 0; i < 104 / waterDiv; i++)
            bg.addLayer(152 + i * waterDiv, waterDiv, 0.060 + (float)i / 25.0);
    }
    else if (m_zoneNameShort == "LZ")
    {
        m_screen.setBgColor(0x0000);
        bg.create(0x32, 7);
        bg.addLayer(0, 256, 0.005);
    }
    else if (m_zoneNameShort == "MZ")
    {
        m_screen.setBgColor(0x00a0);
        bg.create(0x36, 2);
        bg.addLayer(0, 256, 0.005);
    }
    else if (m_zoneNameShort == "SBZ")
    {
        m_screen.setBgColor(0x6400);
        bg.create(0x34, 4);
        bg.addLayer(0, 256, 0.005);
    }
    else if (m_zoneNameShort == "SLZ")
    {
        m_screen.setBgColor(0x0000);
        bg.create(0x30, 1);
        bg.addLayer(0, 256, 0.005);
    }
    else if (m_zoneNameShort == "SYZ")
    {
        m_screen.setBgColor(0x6260);
        bg.create(0x3D, 2);
        bg.addLayer(0, 256, 0.005);
    }

    // S Tubes
    if (m_zoneNameShort == "GHZ")
    {
        m_audio.playMusic(2);

		auto& layout = m_terrain.getLayout();

        for (int i = 0; i < layout.getHeight(); i++)
            for (int j = 0; j < layout.getWidth(); j++)
            {
                uint8_t chunkId = layout.getChunkId(j, i);

                int x = j * layout.getChunksRadiusPixels();
                int y = i * layout.getChunksRadiusPixels();

                if (chunkId == 0x1F)
                    m_entities.push_back(new GimGHZ_STube(v2f(x + 8 + 8, y + 112), 0));
                else if (chunkId == 0x20)
                    m_entities.push_back(new GimGHZ_STube(v2f(x + 248 - 8, y + 112), 1));
            }
    }
}


void Level::update() {
	if (lvInformer) {
		if (lvInformer->getType() == LevelInformer::T_ROUND_CLEAR &&
			lvInformer->getTick() == 0)
				m_audio.playMusic(1);

		if (lvInformer->isDone()) {
			if (isTimeStopped)
				isFadeOut = true;
			else {
				delete lvInformer;
				lvInformer = nullptr;
			}
		}
	}

	if (!isFadeDeath) {
		if (!isFadeOut) {
			if (fade < 0xFF)
				fade++;
		} else {
			if (fade > 0)
				fade--;
			else
				end = true;
		}
	}

	if (isFadeDeath) {
		if (fade > 0)
			fade--;
		else {
			score = 0;
			playerDied = true;
			return;
		}
	}

    updateLevelSpecific();

	//m_entityPool.update();

    for (it = m_entities.begin(); it != m_entities.end();) {
		Entity* ent = *it;
		if (!ent->d_isLiving()) {
			it = m_entities.erase(it);
			delete ent;
		} else {
			it++;
		}
	}

	for (it = m_entities.begin(); it != m_entities.end(); it++) {
	
		if ((*it)->d_isInCamera(cam) || (*it)->d_getType() == TYPE_PLAYER) {
			
			(*it)->d_update();
			(*it)->d_reactingToOthers(m_entities);
			if ((*it)->d_getType() == TYPE_PLAYER) {
				Player* pl = (Player*)(*it);
				pl->terrainCollision(cam);
				pl->entitiesCollision(m_entities, cam);
				pl->moveCam(cam);	

				if (pl->isEndLv() && !lvInformer && !isTimeStopped) {
					isTimeStopped = true;
					lvInformer = new LevelInformer("err", m_act, m_screen, m_audio, 
						LevelInformer::T_ROUND_CLEAR, &score, rings, time );
				}

				if (pl->isDied())
					isFadeDeath = true;
			} else if ((*it)->d_getType() == TYPE_RING) {
				// Ring animation
				Ring* ring = (Ring*)(*it);
				ring->animate(float(ringFrame));
			} 
			
		} else if ((*it)->d_isLiving()) {
			if ((*it)->d_getType() == TYPE_PARTICLE) {
				(*it)->d_destroy();
			}

			(*it)->d_goToStartPos();
			if ((*it)->d_getType() == TYPE_PLATFORM) {
				GimGHZ_Platform* plt = (GimGHZ_Platform*)(*it);
				plt->init();
			}
			if ((*it)->d_getType() == TYPE_RING) {
				Ring* ring = (Ring*)*it;
				if (ring->isBounce())
					ring->d_destroy();
			}
			if ((*it)->d_getType() == TYPE_BULLET) {
				(*it)->d_destroy();
			}
		}
	}

	

	// Ring animation
	ringFrame += 0.125;
	if (ringFrame >= 84)
		ringFrame = 80;

	// Time count
	tick++;

	if (!isTimeStopped)
		time = tick / 60;
}

void Level::updateLevelSpecific()
{
    if (m_zoneNameShort == "GHZ")
    {
        switch (m_act)
        {
        case 1:
            cam.setBottomBorder((cam.getPos().x < 0x16D0) ? 1024 : 1280);
            break;
        case 2:
            cam.setBottomBorder((cam.getPos().x > 0x0EE0 && cam.getPos().x < 0x1580) ? 0x02E0 : 1536);
        case 3:
            if (cam.getPos().x > 0x1680)
                cam.setBottomBorder(0x0400);
            else if (cam.getPos().x < 0x0400)
                cam.setBottomBorder(0x03F0);
            else
                cam.setBottomBorder(1536);
            break;
        }
    }
}

void Level::draw() {
	cam.update();

	// bg.draw(cam, trn);

	// Terrain
	m_terrainDrawer.draw();

	// Entities
	// m_entityPool.draw();

	for (it = m_entities.begin(); it != m_entities.end(); it++) {
		if ((*it)->d_isInCamera(cam)) {
			(*it)->d_draw(cam);
		}
	}

	drawHud();

	if (lvInformer && isTimeStopped) lvInformer->draw();

	if (fade != 0xFF) {
		sf::RenderWindow& wnd = m_screen.getSfmlWindow();
		sf::RectangleShape rs(sf::Vector2f(m_screen.getSize().width, m_screen.getSize().height));
		rs.setFillColor(sf::Color(0, 0, 0, 0xFF - fade));
		wnd.draw(rs);
	}

	if (lvInformer && !isTimeStopped) lvInformer->draw();
}

void Level::drawHud() {
	m_screen.drawTextureRect(2, irect(1, 60, 40, 16), v2f(16, 8));
	m_screen.drawTextureRect(2, irect(42, 60, 32, 16), v2f(16, 24));
	m_screen.drawTextureRect(2, irect(75, 60, 40, 16), v2f(16, 40));

	char sScore[7];
	char sTime[5];
	char sRings[4];

	snprintf(sScore, 7, "%6d", score);
	snprintf(sTime, 5, "%d:%02d", time / 60, time % 60);
	snprintf(sRings, 4, "%3d", rings);

	m_screen.drawText(3, sScore, v2f(64, 8));
	m_screen.drawText(3, sTime, v2f(56, 24));
	m_screen.drawText(3, sRings, v2f(64, 40));
}


void Level::free() {
	delete lvInformer;
}