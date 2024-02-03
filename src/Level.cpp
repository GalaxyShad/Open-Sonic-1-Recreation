#include "Level.h"

#include "entity-creator.hpp"
#include <cstdio>


void Level::create(std::string fZone, std::string fAct, int act, GameType gameType) {
	sZone 		= fZone;
	sAct  		= fAct;
	this->act 	= act;

	if (sAct == "icz1") {
		gameType = GameType::SONIC_3K;
	}

	std::string sTex       = "content/levels/textures/"  + sZone + ".png";
	std::string sCollide   = "content/levels/collide/"   + sZone + ".bin";
	std::string sMap256    = "content/levels/map256/"    + sZone + ".bin";
	std::string sLayout    = "content/levels/layout/"    + sAct  + ".bin";
	std::string sStartPos  = "content/levels/startpos/"  + sAct  + ".bin";
	std::string sObjPos    = "content/levels/objpos/" 	 + sAct  + ".bin";

	terrain::ITerrainLoader* terrainLoader = nullptr;

	terrain::TerrainLoaderSonic1FilePaths filepaths;

	switch (gameType) {
		case GameType::SONIC_1:
			filepaths.angles            = "content/levels/collide/Angle Map.bin";
			filepaths.verticalHeights   = "content/levels/collide/Collision Array (Normal).bin";
			filepaths.horizontalHeights = "content/levels/collide/Collision Array (Rotated).bin";

			filepaths.blocks = sCollide.c_str();
			filepaths.chunks = sMap256.c_str();
			filepaths.layout = sLayout.c_str();

			scr.loadTextureFromFile(sTex.c_str(), 255);

			terrainLoader = new terrain::TerrainLoaderSonic1(filepaths);

			loadTerrainAct(sStartPos.c_str());
			break;
		case GameType::SONIC_3K: 
			filepaths.angles            = "content/levels/sonic3/Angles S3.bin";
			filepaths.verticalHeights   = "content/levels/sonic3/Height Maps S3.bin";
			filepaths.horizontalHeights = "content/levels/sonic3/Height Maps Rotated S3.bin";

			filepaths.blocks = "content/levels/sonic3/ICZ/Collision/1.bin";
			filepaths.chunks = "content/levels/sonic3/ICZ/Chunks/Primary.bin";
			filepaths.layout = "content/levels/sonic3/ICZ/Layout/1.bin";

			scr.loadTextureFromFile("content/levels/sonic3/ICZ/texture.png", 255);

			terrainLoader = new terrain::TerrainLoaderSonic3(filepaths);
			break;
		default:
			throw std::exception();
	}

	m_terrain = loadTerrain(filepaths, *terrainLoader);
	trn = new Terrain(*m_terrain);

	delete terrainLoader;

	if (gameType == GameType::SONIC_1) {
		loadObjects(sObjPos.c_str());
		trn->createLayeringObjs(entities);
	}

	m_terrainDrawer = new terrain::TerrainDrawer(cam, m_terrain->getChunkStore(), m_terrain->getLayout(), 255);

	// Bg
	bg = new Bg(*m_terrainDrawer);

	const char* zoneName = sZone.c_str();
	if      (sZone == "GHZ") zoneName = "GREEN HILL";
	else if (sZone == "LZ")  zoneName = "LABYRINTH";
	else if (sZone == "MZ")  zoneName = "MARBLE";
	else if (sZone == "SBZ") zoneName = "SCRAP BRAIN";
	else if (sZone == "SYZ") zoneName = "SPRING YARD";
	else if (sZone == "SLZ") zoneName = "STAR LIGHT";

	lvInformer = new  LevelInformer(zoneName, act, scr, audio, LevelInformer::T_TITLE_CARD);

	if (gameType == GameType::SONIC_1) {
        createLevelSpecific();
    }

	// Create player
	Player* pl = new Player(plStartPos, *trn, input, audio, rings, score);
	entities.push_back(pl);	

    cam.create(Vector2f(plStartPos.x - scr.getSize().width / 2, 
						plStartPos.y - scr.getSize().height / 2), 
			   trn->getSize(), true);


	
	for (it = entities.begin(); it != entities.end(); it++)
		(*it)->create();

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

void Level::createLevelSpecific()
{
    if (sZone == "GHZ")
    {
        scr.setBgColor(0x08E0);
        bg->create(0x3C, 3);
        bg->addLayer(0, 32, 0.001, 0.5);
        bg->addLayer(32, 16, 0.002, 0.25);
        bg->addLayer(48, 16, 0.003, 0.125);
        bg->addLayer(64, 48, 0.005);
        bg->addLayer(112, 40, 0.060);
        const int waterDiv = 8;
        for (int i = 0; i < 104 / waterDiv; i++)
            bg->addLayer(152 + i * waterDiv, waterDiv, 0.060 + (float)i / 25.0);
    }
    else if (sZone == "LZ")
    {
        scr.setBgColor(0x0000);
        bg->create(0x32, 7);
        bg->addLayer(0, 256, 0.005);
    }
    else if (sZone == "MZ")
    {
        scr.setBgColor(0x00a0);
        bg->create(0x36, 2);
        bg->addLayer(0, 256, 0.005);
    }
    else if (sZone == "SBZ")
    {
        scr.setBgColor(0x6400);
        bg->create(0x34, 4);
        bg->addLayer(0, 256, 0.005);
    }
    else if (sZone == "SLZ")
    {
        scr.setBgColor(0x0000);
        bg->create(0x30, 1);
        bg->addLayer(0, 256, 0.005);
    }
    else if (sZone == "SYZ")
    {
        scr.setBgColor(0x6260);
        bg->create(0x3D, 2);
        bg->addLayer(0, 256, 0.005);
    }

    // S Tubes
    if (sZone == "GHZ")
    {
        audio.playMusic(2);
        for (int i = 0; i < m_terrain->getLayout().getHeight(); i++)
            for (int j = 0; j < m_terrain->getLayout().getWidth(); j++)
            {
                uint8_t chunkId = m_terrain->getLayout().getChunkId(j, i);

                int x = j * m_terrain->getLayout().getChunksRadiusPixels();
                int y = i * m_terrain->getLayout().getChunksRadiusPixels();

                if (chunkId == 0x1F)
                    entities.push_back(new GimGHZ_STube(Vector2f(x + 8 + 8, y + 112), 0));
                else if (chunkId == 0x20)
                    entities.push_back(new GimGHZ_STube(Vector2f(x + 248 - 8, y + 112), 1));
            }
    }
}

void Level::update() {
	if (lvInformer) {
		if (lvInformer->getType() == LevelInformer::T_ROUND_CLEAR &&
			lvInformer->getTick() == 0)
				audio.playMusic(1);

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
			restart();
		}
	}

	if (sAct == "ghz1") {
		if (cam.getPos().x < 0x16D0) 
			cam.setBottomBorder(1024);
		else 
			cam.setBottomBorder(1280);
	} else if (sAct == "ghz2") {
		if (cam.getPos().x > 0x0EE0 && cam.getPos().x < 0x1580) 
			cam.setBottomBorder(0x02E0);
		else 
			cam.setBottomBorder(1536);
	} else if (sAct == "ghz3") {
		if (cam.getPos().x > 0x1680)
			cam.setBottomBorder(0x0400);
		else if (cam.getPos().x < 0x0400)
			cam.setBottomBorder(0x03F0);
		else
			cam.setBottomBorder(1536);
	}

	for (it = entities.begin(); it != entities.end();) {
		Entity* ent = *it;
		if (!ent->isLiving()) {
			it = entities.erase(it);
			delete ent;
		} else {
			it++;
		}
	}

	for (it = entities.begin(); it != entities.end(); it++) {
		if ((*it)->isInCamera(cam) || (*it)->getType() == TYPE_PLAYER) {
			(*it)->update();
			(*it)->reactingToOthers(entities);
			if ((*it)->getType() == TYPE_PLAYER) {
				Player* pl = (Player*)(*it);
				pl->terrainCollision(cam);
				pl->entitiesCollision(entities, cam);
				pl->moveCam(cam);	

				if (pl->isEndLv() && !lvInformer && !isTimeStopped) {
					isTimeStopped = true;
					lvInformer = new LevelInformer("err", act, scr, audio, 
						LevelInformer::T_ROUND_CLEAR, &score, rings, time );
				}

				if (pl->isDied())
					isFadeDeath = true;
			} else if ((*it)->getType() == TYPE_ENEMY) {
				Enemy* en = (Enemy*)(*it);
				en->trnCollision(*trn);
			} else if ((*it)->getType() == TYPE_RING) {
				// Ring animation
				Ring* ring = (Ring*)(*it);
				ring->animate(float(ringFrame));
			} 
			
		} else if ((*it)->isLiving()) {
			if ((*it)->getType() == TYPE_PARTICLE) {
				(*it)->destroy();
			}

			(*it)->goToStartPos();
			if ((*it)->getType() == TYPE_PLATFORM) {
				GimGHZ_Platform* plt = (GimGHZ_Platform*)(*it);
				plt->create();
			}
			if ((*it)->getType() == TYPE_RING) {
				Ring* ring = (Ring*)*it;
				if (ring->isBounce())
					ring->destroy();
			}
			if ((*it)->getType() == TYPE_BULLET) {
				(*it)->destroy();
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

void Level::draw() {
	cam.update();

	bg->draw(cam, *trn);

	// Terrain
	m_terrainDrawer->draw();

	// Entities
	for (it = entities.begin(); it != entities.end(); it++) {
		if ((*it)->isInCamera(cam))
			(*it)->draw(cam);
	}

	drawHud();

	if (lvInformer && isTimeStopped) lvInformer->draw();

	if (fade != 0xFF) {
		sf::RenderWindow& wnd = scr.getSfmlWindow();
		sf::RectangleShape rs(sf::Vector2f(scr.getSize().width, scr.getSize().height));
		rs.setFillColor(sf::Color(0, 0, 0, 0xFF - fade));
		wnd.draw(rs);
	}

	if (lvInformer && !isTimeStopped) lvInformer->draw();
}

void Level::drawHud() {
	scr.drawTextureRect(2, IntRect(1, 60, 40, 16), Vector2f(16, 8));
	scr.drawTextureRect(2, IntRect(42, 60, 32, 16), Vector2f(16, 24));
	scr.drawTextureRect(2, IntRect(75, 60, 40, 16), Vector2f(16, 40));

	char sScore[7];
	char sTime[5];
	char sRings[4];

	snprintf(sScore, 7, "%6d", score);
	snprintf(sTime, 5, "%d:%02d", time / 60, time % 60);
	snprintf(sRings, 4, "%3d", rings);

	scr.drawText(3, sScore, Vector2f(64, 8));
	scr.drawText(3, sTime, Vector2f(56, 24));
	scr.drawText(3, sRings, Vector2f(64, 40));
}

#include <algorithm>
void Level::loadObjects(const char* filename) {
	std::ifstream file(filename, std::ios::binary);
	if (!file) {
        std::cout << "File load error" << std::endl;
        return;
    }

	EntityCreator entCreator(entities);
    uint8_t buff[6];

    while (file.read(reinterpret_cast<char*>(buff), sizeof(buff))) {
        entities.push_back(entCreator.create(EntityPlacement::from6ByteBuffer(buff)));
    }

    file.close();
}


bool Level::loadTerrainAct(const char* fnStartPos) {
    std::ifstream file(fnStartPos, std::ios::binary);

    if (!file) {
        std::cout << "Error loading Act data" << std::endl;
        std::cout << "Start position: " << fnStartPos << " FAIL" << std::endl;
        return false;
    }

    uint16_t x, y;

    if (!file.read(reinterpret_cast<char*>(&x), sizeof(x))) {
        return false;
    }
    
    x = (x << 8) | (x >> 8); // Swap the byte order for x

    if (!file.read(reinterpret_cast<char*>(&y), sizeof(y))) {
        return false;
    }

    y = (y << 8) | (y >> 8); // Swap the byte order for y

    plStartPos.x = static_cast<float>(x);
    plStartPos.y = static_cast<float>(y);

    return true;
}

void Level::free() {
	for (it = entities.begin(); it != entities.end();) {
		delete *it;
		it = entities.erase(it);
	}

	m_storeTiles.release();
	m_storeBlocks.release();
	m_storeChunks.release();
	m_layout.release();

	delete m_terrain;
	delete m_terrainDrawer;

	delete trn;

	delete bg;
	delete lvInformer;

	scr.freeTexture(255);
}