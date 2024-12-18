#include "Game.h"

#include "core/Geometry.h"

#include <iostream>



#define TEX_OBJECTS 0
#define TEX_GHZ_GIMM 1
#define TEX_HUD 2

void Game::init() {
    std::cout << "Loading textures..." << std::endl;

    depEnv_.scr.loadTextureFromFile("content/textures/texGhzObj.png", TEX_GHZ_GIMM, framesGhzGim, 8);
    depEnv_.scr.loadTextureFromFile("content/textures/texObj.png", TEX_OBJECTS, framesObjects, 139);
    depEnv_.scr.loadTextureFromFile("content/textures/texHud.png", TEX_HUD);

    depEnv_.scr.addFont(0, "abcdefghijklmnopqrstuvwxyz1234567890*.:-=!?<>", 1, TEX_HUD, irect(2, 150, 8, 7));
    depEnv_.scr.addFont(1, "abcdefghijklmnopqrstuvwxyz1234567890$.-=>", 1, TEX_HUD, irect(1, 35, 8, 7), 1);
    uint16_t f2widths[] = {15, 15, 15, 15, 14, 14, 16, 15, 6, 16, 15, 16, 15, 16, 15, 15, 14, 14, 15, 15, 15, 3};
    depEnv_.scr.addFont(2, "ABCDEFGHIKLMNOPRSTUYZ.", 1, TEX_HUD, irect(1, 1, 15, 16), 1, f2widths);
    depEnv_.scr.addFont(3, "0123456789:", 0, TEX_HUD, irect(1, 43, 8, 16), 1);

    std::cout << "Textures loaded succesuffuly!" << std::endl;

    depEnv_.audio.loadSound(SND_PATH SND_JUMP_FILE, SND_JUMP);
    depEnv_.audio.loadSound(SND_PATH SND_HURT_FILE, SND_HURT);
    depEnv_.audio.loadSound(SND_PATH SND_SKID_FILE, SND_SKID);
    depEnv_.audio.loadSound(SND_PATH SND_SPIKES_FILE, SND_SPIKES);
    depEnv_.audio.loadSound(SND_PATH SND_SHIELD_FILE, SND_SHIELD);
    depEnv_.audio.loadSound(SND_PATH SND_RING_FILE, SND_RING);
    depEnv_.audio.loadSound(SND_PATH SND_DASH_FILE, SND_DASH);
    depEnv_.audio.loadSound(SND_PATH SND_ROLL_FILE, SND_ROLL);
    depEnv_.audio.loadSound(SND_PATH SND_DESTROY_FILE, SND_DESTROY);
    depEnv_.audio.loadSound(SND_PATH SND_RING_LOSS_FILE, SND_RING_LOSS);
    depEnv_.audio.loadSound(SND_PATH SND_SPRING_FILE, SND_SPRING);

    depEnv_.audio.loadSound(SND_PATH SND_BEEP_FILE, SND_BEEP);
    depEnv_.audio.loadSound(SND_PATH SND_COUNT_END_FILE, SND_COUNT_END);
    depEnv_.audio.loadSound(SND_PATH SND_END_TABLE_FILE, SND_END_TABLE);
    depEnv_.audio.loadSound(SND_PATH SND_PLT_CRUSH_FILE, SND_PLT_CRUSH);

    depEnv_.audio.addMusic(0, "content/music/Title.ogg");
    depEnv_.audio.addMusic(1, "content/music/StageClear.ogg");
    depEnv_.audio.addMusic(2, "content/music/GHZ.ogg");

    depEnv_.audio.playMusic(0);


}

void Game::update() {
    entityPoolEventLoop_.update();
    sceneDirector_.update();
}

void Game::draw(artist_api::Artist &artist) {
    sceneDirector_.draw();
    entityPoolEventLoop_.draw();
}

bool Game::isRunning() {
    return true;
//    return env_.isRunning();
}
