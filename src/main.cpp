
#include "Game.h"

#include "core/DeprecatedGameEnv.h"
#include "core/game_enviroment/ResourceStore.h"

#include "sfml_game_environment/SfmlGameEnvironment.h"

#include "sonic/SonicResources.h"

#include "FramesMappings.h"

#include "AudioMappings.h"

SonicResources::Textures loadTextures(ResourceStore &store,
                                      GameEnvironment &env,
                                      DeprecatedGameEnvironment &deprEnv) {
    auto &loader = env.textureLoader();

    auto load = [&](const std::string &path) {
        return store.load(loader.loadFromFile("content/textures/" + path));
    };

    SonicResources::Textures textures = {
        .objects = load("texObj.png"),
        .objectsGhz = load("texGhzObj.png"),
        .hud = load("texHud.png"),
    };

    constexpr int OBJECTS = 0;
    constexpr int GHZ_GIMM = 1;
    constexpr int HUD = 2;

    deprEnv.scr.bindTexture(OBJECTS, textures.objects);
    deprEnv.scr.bindTexture(GHZ_GIMM, textures.objectsGhz);
    deprEnv.scr.bindTexture(HUD, textures.hud);

    deprEnv.scr.bindTextureFrames(OBJECTS, framesObjects, 139);
    deprEnv.scr.bindTextureFrames(GHZ_GIMM, framesGhzGim, 8);

    return textures;
}

SonicResources::Fonts loadFonts(ResourceStore &store,
                                ResourceID hudTextureResource,
                                DeprecatedGameEnvironment &deprEnv) {

    namespace art = artist_api;

    auto &hudTexture = store.get<art::Texture>(hudTextureResource);

    auto general = art::SpriteFont::fromSpriteRow(
        art::Sprite{.texture = hudTexture,
                    .rect = {.x = 2, .y = 150, .width = 8, .height = 7}},
        "abcdefghijklmnopqrstuvwxyz1234567890*.:-=!?<>");

    auto s1font = art::SpriteFont::fromSpriteRow(
        art::Sprite{.texture = hudTexture,
                    .rect = {.x = 1, .y = 32, .width = 8, .height = 7}},
        "abcdefghijklmnopqrstuvwxyz1234567890$.-=>", {.pixelsBetweenRects = 1});

    auto s1titleCard = art::SpriteFont::fromSpriteRowWidths(
        art::Sprite{.texture = hudTexture,
                    .rect = {.x = 1, .y = 1, .width = 15, .height = 16}},
        {{'A', 15}, {'B', 15}, {'C', 15}, {'D', 15}, {'E', 14}, {'F', 14},
         {'G', 16}, {'H', 15}, {'I', 6},  {'K', 16}, {'L', 15}, {'M', 16},
         {'N', 15}, {'O', 16}, {'P', 15}, {'R', 15}, {'S', 14}, {'T', 14},
         {'U', 15}, {'Y', 15}, {'Z', 15}, {'.', 3}},
        {.pixelsBetweenRects = 1});

    auto s1hudDigits = art::SpriteFont::fromSpriteRow(
        art::Sprite{.texture = hudTexture,
                    .rect = {.x = 1, .y = 43, .width = 8, .height = 16}},
        "0123456789:", {.pixelsBetweenRects = 1});

    SonicResources::Fonts fonts = {
        .general = store.load(std::make_unique<art::SpriteFont>(general)),
        .s1Credits = store.load(std::make_unique<art::SpriteFont>(s1font)),
        .s1TitleScreen =
            store.load(std::make_unique<art::SpriteFont>(s1titleCard)),
        .s1HudDigits =
            store.load(std::make_unique<art::SpriteFont>(s1hudDigits)),
    };

    deprEnv.scr.bindFont(0, fonts.general);
    deprEnv.scr.bindFont(1, fonts.s1Credits);
    deprEnv.scr.bindFont(2, fonts.s1TitleScreen);
    deprEnv.scr.bindFont(3, fonts.s1HudDigits);

    return fonts;
}

SonicResources::Sounds loadSounds(ResourceStore &store,
                                  GameEnvironment &env,
                                  DeprecatedGameEnvironment &deprEnv) {

    auto& loader = env.audioLoader();

    auto loadSound = [&](const std::string &path) {
        return store.load(loader.loadSound("content/sounds/" + path));
    };

    SonicResources::Sounds sounds = {
        .player = {
            .jump = loadSound(SND_JUMP_FILE),
            .hurt = loadSound(SND_HURT_FILE),
            .skid = loadSound(SND_SKID_FILE),
            .spikes = loadSound(SND_SPIKES_FILE),
            .shield = loadSound(SND_SHIELD_FILE),
            .dash = loadSound(SND_DASH_FILE),
            .roll = loadSound(SND_ROLL_FILE),
        },

        .ring = loadSound(SND_RING_FILE),
        .ringLoss = loadSound(SND_RING_LOSS_FILE),
        .spring = loadSound(SND_SPRING_FILE),
        .ding = loadSound(SND_COUNT_END_FILE),
        .beep = loadSound(SND_BEEP_FILE),
        .breaking = loadSound(SND_PLT_CRUSH_FILE),
        .endSignSpin = loadSound(SND_END_TABLE_FILE),
    };

    return sounds;
}

SonicResources::Music loadMusic(ResourceStore &store,
                                GameEnvironment &env,
                                DeprecatedGameEnvironment &deprEnv) {
    auto& loader = env.audioLoader();

    auto load = [&](const std::string &path) {
        return store.load(loader.loadMusic("content/music/" + path));
    };

    SonicResources::Music mus = {
        .title = load("Title.ogg"),
        .stageClear = load("StageClear.ogg"),
        .greenHillZone = load("GHZ.ogg")
    };

    return mus;
}

std::unique_ptr<IStorableResource> loadResources(ResourceStore& store, GameEnvironment& env, DeprecatedGameEnvironment& deprEnv) {
    auto res = new SonicResources;

    res->textures = loadTextures(store, env, deprEnv);
    res->fonts = loadFonts(store, res->textures.hud, deprEnv);
    res->sounds = loadSounds(store, env, deprEnv);
    res->music = loadMusic(store, env, deprEnv);

    return std::unique_ptr<IStorableResource>(res);
}

int main() {
    return Game::RunGameWithSfmlBackend(loadResources);
}