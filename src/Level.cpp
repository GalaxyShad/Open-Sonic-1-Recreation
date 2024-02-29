#include "Level.h"

#include "LevelInformer.h"
#include "entity-creator.hpp"
#include "game-loop-ticker.h"
#include <cstdio>

void Level::create() {
    if (m_gameType == GameType::SONIC_1) {
        createSonic1LayeringObjects();
    }

    m_entityPool.create(new TitleCardSonic1(
        m_zoneName, m_act, m_screen.getSize().width, m_entityPool));

    if (m_gameType == GameType::SONIC_1) {
        createZoneSpecific();
    }

    // Create player
    m_entityPool.create(new Player(
        m_playerStartPosition, m_entityPool.legacy_rawPool(), m_entityPool, cam,
        m_terrain, m_input, m_audio, rings, score));

    // Create camera
    auto screenSize = m_screen.getSize();
    auto cameraPos =
        v2f(m_playerStartPosition.x - (float)screenSize.width / 2,
            m_playerStartPosition.y - (float)screenSize.height / 2);
    auto levelSize = m_terrain.getLayout().getSizeInPixels();

    cam.create(cameraPos, Size(levelSize.x, levelSize.y),
               (m_gameType == GameType::SONIC_3K));

    m_entityPool.init();

    // Level Variables
    ringFrame = 80.0; // Using for same animation to all rings

    tick = 0;
    rings = 0;

    end = false;
    isTimeStopped = false;
    isFadeOut = false;
    isFadeDeath = false;
    fade = 0;

    GameLoopTicker::instance().reset();
}

void Level::createZoneSpecific() {
    if (m_zoneNameShort == "GHZ") {
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
    } else if (m_zoneNameShort == "LZ") {
        m_screen.setBgColor(0x0000);
        bg.create(0x32, 7);
        bg.addLayer(0, 256, 0.005);
    } else if (m_zoneNameShort == "MZ") {
        m_screen.setBgColor(0x00a0);
        bg.create(0x36, 2);
        bg.addLayer(0, 256, 0.005);
    } else if (m_zoneNameShort == "SBZ") {
        m_screen.setBgColor(0x6400);
        bg.create(0x34, 4);
        bg.addLayer(0, 256, 0.005);
    } else if (m_zoneNameShort == "SLZ") {
        m_screen.setBgColor(0x0000);
        bg.create(0x30, 1);
        bg.addLayer(0, 256, 0.005);
    } else if (m_zoneNameShort == "SYZ") {
        m_screen.setBgColor(0x6260);
        bg.create(0x3D, 2);
        bg.addLayer(0, 256, 0.005);
    }

    // S Tubes
    if (m_zoneNameShort == "GHZ") {
        m_audio.playMusic(2);

        auto &layout = m_terrain.getLayout();

        for (int i = 0; i < layout.getHeight(); i++)
            for (int j = 0; j < layout.getWidth(); j++) {
                uint8_t chunkId = layout.getChunkId(j, i);

                int x = j * layout.getChunksRadiusPixels();
                int y = i * layout.getChunksRadiusPixels();

                if (chunkId == 0x1F)
                    m_entityPool.create(
                        new GimGHZ_STube(v2f(x + 8 + 8, y + 112), 0));
                else if (chunkId == 0x20)
                    m_entityPool.create(
                        new GimGHZ_STube(v2f(x + 248 - 8, y + 112), 1));
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

    m_entityPool.update();

    // Time count
    tick++;

    if (!isTimeStopped)
        time = tick / 60;

    GameLoopTicker::instance().tick();
}

void Level::updateLevelSpecific() {
    if (m_zoneNameShort == "GHZ") {
        switch (m_act) {
        case 1:
            cam.setBottomBorder((cam.getPos().x < 0x16D0) ? 1024 : 1280);
            break;
        case 2:
            cam.setBottomBorder(
                (cam.getPos().x > 0x0EE0 && cam.getPos().x < 0x1580) ? 0x02E0
                                                                     : 1536);
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

void Level::createSonic1LayeringObjects() {
    auto &layout = m_terrain.getLayout();

    for (int i = 0; i < layout.getHeight(); i++) {
        for (int j = 0; j < layout.getWidth(); j++) {
            if (!layout.isLayeringChunk(j, i))
                continue;

            int x = j * layout.getChunksRadiusPixels();
            int y = i * layout.getChunksRadiusPixels();

            m_entityPool.create(
                new LayerSwitcher(v2f(x + 128, y + 32), v2f(16, 64), 2));
            m_entityPool.create(
                new LayerSwitcher(v2f(x - 8, y + 128), v2f(16, 256), 0));
            m_entityPool.create(
                new LayerSwitcher(v2f(x + 264, y + 128), v2f(16, 256), 1));
        }
    }
}

void Level::draw() {
    cam.update();

    // bg.draw(cam, trn);

    // Terrain
    m_terrainDrawer.draw();

    // Entities
    m_entityPool.draw();

    drawHud();

    if (lvInformer && isTimeStopped)
        lvInformer->draw();

    if (fade != 0xFF) {
        sf::RenderWindow &wnd = m_screen.getSfmlWindow();
        sf::RectangleShape rs(
            sf::Vector2f(m_screen.getSize().width, m_screen.getSize().height));
        rs.setFillColor(sf::Color(0, 0, 0, 0xFF - fade));
        wnd.draw(rs);
    }

    if (lvInformer && !isTimeStopped)
        lvInformer->draw();
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

void Level::free() { delete lvInformer; }