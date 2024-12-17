#include "Game.h"

#include "core/Geometry.h"

#include <iostream>

#define TEX_OBJECTS 0
#define TEX_GHZ_GIMM 1
#define TEX_HUD 2

void Game::init() {
    std::cout << "Loading textures..." << std::endl;

    scr.loadTextureFromFile("content/textures/texGhzObj.png", TEX_GHZ_GIMM, framesGhzGim, 8);
    scr.loadTextureFromFile("content/textures/texObj.png", TEX_OBJECTS, framesObjects, 139);
    scr.loadTextureFromFile("content/textures/texHud.png", TEX_HUD);

    scr.addFont(0, "abcdefghijklmnopqrstuvwxyz1234567890*.:-=!?<>", 1, TEX_HUD, irect(2, 150, 8, 7));
    scr.addFont(1, "abcdefghijklmnopqrstuvwxyz1234567890$.-=>", 1, TEX_HUD, irect(1, 35, 8, 7), 1);
    uint16_t f2widths[] = {15, 15, 15, 15, 14, 14, 16, 15, 6, 16, 15, 16, 15, 16, 15, 15, 14, 14, 15, 15, 15, 3};
    scr.addFont(2, "ABCDEFGHIKLMNOPRSTUYZ.", 1, TEX_HUD, irect(1, 1, 15, 16), 1, f2widths);
    scr.addFont(3, "0123456789:", 0, TEX_HUD, irect(1, 43, 8, 16), 1);

    std::cout << "Textures loaded succesuffuly!" << std::endl;

    audio.loadSound(SND_PATH SND_JUMP_FILE, SND_JUMP);
    audio.loadSound(SND_PATH SND_HURT_FILE, SND_HURT);
    audio.loadSound(SND_PATH SND_SKID_FILE, SND_SKID);
    audio.loadSound(SND_PATH SND_SPIKES_FILE, SND_SPIKES);
    audio.loadSound(SND_PATH SND_SHIELD_FILE, SND_SHIELD);
    audio.loadSound(SND_PATH SND_RING_FILE, SND_RING);
    audio.loadSound(SND_PATH SND_DASH_FILE, SND_DASH);
    audio.loadSound(SND_PATH SND_ROLL_FILE, SND_ROLL);
    audio.loadSound(SND_PATH SND_DESTROY_FILE, SND_DESTROY);
    audio.loadSound(SND_PATH SND_RING_LOSS_FILE, SND_RING_LOSS);
    audio.loadSound(SND_PATH SND_SPRING_FILE, SND_SPRING);

    audio.loadSound(SND_PATH SND_BEEP_FILE, SND_BEEP);
    audio.loadSound(SND_PATH SND_COUNT_END_FILE, SND_COUNT_END);
    audio.loadSound(SND_PATH SND_END_TABLE_FILE, SND_END_TABLE);
    audio.loadSound(SND_PATH SND_PLT_CRUSH_FILE, SND_PLT_CRUSH);

    audio.addMusic(0, "content/music/Title.ogg");
    audio.addMusic(1, "content/music/StageClear.ogg");
    audio.addMusic(2, "content/music/GHZ.ogg");

    audio.playMusic(0);
}

void Game::update() {
    titleMenuEntity_.onUpdate({
        .input = env_.input()
    });
}

void Game::draw(artist_api::Artist &artist) {
    titleMenuEntity_.onDraw({
        .artist = artist,
        .deprecatedScreen = scr
    });
}

bool Game::isRunning() {
    return (bool)(scr.getSfmlWindow().isOpen());
}
