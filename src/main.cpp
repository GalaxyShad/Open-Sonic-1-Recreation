
#include "Game.h"

#include "SFML/Audio/InputSoundFile.hpp"
#include "core/DeprecatedGameEnv.h"
#include "core/game_enviroment/GameEnvironment.h"
#include "core/game_enviroment/IStorableResource.h"
#include "core/game_enviroment/ResourceStore.h"

#include "core/game_enviroment/artist/ArtistStructs.h"
#include "sfml_game_environment/SfmlGameEnvironment.h"

#include "sonic/SonicResources.h"

#include "FramesMappings.h"

#include "AudioMappings.h"
#include <memory>
#include <utility>

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
        .s1TitleCard =
            store.load(std::make_unique<art::SpriteFont>(s1titleCard)),
        .s1HudDigits =
            store.load(std::make_unique<art::SpriteFont>(s1hudDigits)),
    };

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

SonicResources::Animation makeSpritesAndAnimations(ResourceStore& store, SonicResources::Textures& textures) {
    using namespace artist_api; 

    SonicResources::Animation anims;

    auto t = [&store](Texture& tex, Rect r, bool centered = true, Vector2D<float> offset = {}){
        auto* spr = centered ? new Sprite(Sprite::withCenterOffset(tex, r)) : new Sprite {.texture = tex,.rect = r,.offset = offset};

        return store.load(std::unique_ptr<IStorableResource>((IStorableResource*)spr));
    };

    // clang-format off


    auto an = [&store](Animation a){
        auto* ptr = new Animation(std::move(a));

        return store.load(std::unique_ptr<IStorableResource>((IStorableResource*)ptr));
    };

    auto& ghzTex = store.get<Texture>(textures.objectsGhz);
    // auto& ghzTex = store.get<Texture>(textures.objects);
    auto& objTex = store.get<Texture>(textures.objects);

    anims.sprites.greenHillZone = {
        .stone               = t(ghzTex, {0,   0,  48, 32}),
        .swingPlatform       = t(ghzTex, {48,  0,  96, 88}, false, {48, 56}),
        .bridge              = t(ghzTex, {0,   32, 16, 16}),
        .platform            = t(ghzTex, {0,   90, 64, 30}),

        .wallLeftWithShadow  = t(ghzTex, {144, 0,  16, 64}),
        .wallLeft            = t(ghzTex, {160, 0,  16, 64}),
        .wallRight           = t(ghzTex, {144, 64, 16, 64}),

        .bridgeColumn        = t(ghzTex, {64,  88, 32, 16}),
    };

    anims.sonic = {
        .idle = an(Animation {
            .frames = {
               Sprite { objTex, {0, 0, 29, 39},  {14, 19} },
            }
        }),
        .boredStart = an(Animation {
            .frames = {
               Sprite { objTex, {29, 1, 30, 38}, {12, 18} },
            }
        }),
        .bored = an(Animation {
            .frames = {
               Sprite { objTex, {59, 1, 30, 38}, {12, 18} },
               Sprite { objTex, {89, 1, 29, 38}, {12, 18} },
            }
        }),
        .walk = an(Animation {
            .frames = {
               Sprite { objTex, {147,2,24,37}, {13,17} },
               Sprite { objTex, {171,2,36,37}, {19,19} },
               Sprite { objTex, {207,1,29,38}, {18,18} },
               Sprite { objTex, {236,1,27,37}, {14,17} },
               Sprite { objTex, {263,1,37,37}, {18,19} },
               Sprite { objTex, {300,0,32,38}, {14,18} },
            }
        }),
        .walkDiagonal = an(Animation{
            .frames = {
                Sprite { objTex, {680,0,34,37}, {17,18}},
                Sprite { objTex, {718,0,36,43}, {18,19}},
                Sprite { objTex, {758,0,35,40}, {17,18}},
                Sprite { objTex, {795,2,38,38}, {18,16}},
                Sprite { objTex, {835,2,38,43}, {19,17}},
                Sprite { objTex, {875,2,41,34}, {18,16}},
            }
        }),
        .run = an(Animation{
            .frames = {
                Sprite { objTex, {332,1,31,35}, {20,15}},
                Sprite { objTex, {363,0,31,36}, {20,16}},
                Sprite { objTex, {394,1,30,36}, {19,15}},
                Sprite { objTex, {424,0,31,36}, {20,16}},
            }
        }),
        .runDiagonal = an(Animation{
            .frames = {
                Sprite { objTex, {919,1,37,39}, {19,17}},
                Sprite { objTex, {959,0,36,40}, {18,18}},
                Sprite { objTex, {2,40,36,39}, {19,17}},
                Sprite { objTex, {43,39,36,40}, {18,18}},
            }
        }),
        .dash = an(Animation{
            .frames = {
                Sprite { objTex, {287,40,39,31}, {28,11}},
                Sprite { objTex, {327,39,39,32}, {28,12}},
                Sprite { objTex, {367,37,39,31}, {28,11}},
                Sprite { objTex, {408,36,38,32}, {27,12}},
            }
        }),
        .dashDiagonal = an(Animation{
            .frames = {
                Sprite { objTex, {449,37,37,41}, {18,14}},
                Sprite { objTex, {489,32,34,40}, {18,14}},
                Sprite { objTex, {530,32,37,43}, {18,14}},
                Sprite { objTex, {569,33,35,39}, {18,14}},
            }
        }),
        .sit = an(Animation{
            .frames = {
                Sprite { objTex, {118,13,29,26}, {9,6}},
            }
        }),
        .roll = an(Animation{
            .frames = {
                Sprite { objTex, {456,1,30,30}, {15,15}},
                Sprite { objTex, {488,1,29,30}, {15,16}},
                Sprite { objTex, {456,1,30,30}, {15,15}},
                Sprite { objTex, {520,1,30,29}, {16,16}},
                Sprite { objTex, {456,1,30,30}, {15,15}},
                Sprite { objTex, {553,1,29,30}, {15,16}},
                Sprite { objTex, {456,1,30,30}, {15,15}},
                Sprite { objTex, {584,2,30,29}, {16,15}},
            }
        }),
        .skidStart = an(Animation{
            .frames = {
                Sprite { objTex, {681,37,33,36}, {15,16}},
            }
        }),
        .skid = an(Animation{
            .frames = {
                Sprite { objTex, {615,0,30,35}, {12,15}},
                Sprite { objTex, {645,0,31,36}, {13,16}},
            }
        }),
        .hurt = an(Animation{
            .frames = {
                Sprite { objTex, {80,39,40,28}, {20,14}},
            }
        }),
        .die = an(Animation{
            .frames = {
                Sprite { objTex, {120,39,34,43}, {17,21}},
            }
        }),
        .push = an(Animation{
            .frames = {
                Sprite { objTex, {154,40,29,36}, {14,17}},
                Sprite { objTex, {183,39,24,37}, {9,18}},
                Sprite { objTex, {207,40,28,36}, {13,17}},
                Sprite { objTex, {235,39,24,37}, {9,18}},
            }
        }),
        .lookUp = an(Animation{
            .frames = {
                Sprite { objTex, {259,40,28,38}, {14,18}},
            }
        }),
        .spring = an(Animation{
            .frames = {
                Sprite { objTex, {607,36,25,43}, {13,22}},
                Sprite { objTex, {632,36,20,44}, {8,22}},
                Sprite { objTex, {652,37,29,44}, {14,22}},
            }
        }),
        .fall = an(Animation{
            .frames = {
                Sprite { objTex, {714,43,36,48}, {18,24}},
                Sprite { objTex, {750,43,36,47}, {18,23}},
            }
        }),
    };

    anims.monitor = {
        .main = an(Animation{
            .frames = { Sprite { objTex, {786,40,30,30}, {15,15}}, }
            }),
        .broken = an(Animation{
            .frames = { Sprite { objTex, {816,45,32,16}, {16,1}}, }
            }),
        .noise = an(Animation{
            .frames = {
                    Sprite { objTex, {786,70,16,14}, {8,7}},
                    Sprite { objTex, {802,70,16,14}, {8,7}},
                }
            }),
        .icon = {
            .ring = an(Animation{
                .frames = { Sprite { objTex, {818,61,16,14}, {8,7}}, }
            }),
            .invinsibility = an(Animation{
                .frames = { Sprite { objTex, {834,61,16,14}, {8,7}}, }
            }),
            .sonic = an(Animation{
                .frames = { Sprite { objTex, {850,61,16,14}, {8,7}}, }
            }),
            .shield = an(Animation{
                .frames = { Sprite { objTex, {818,75,16,14}, {8,7}}, }
            }),
            .speedSneakers = an(Animation{
                .frames = { Sprite { objTex, {834,75,16,14}, {8,7}}, }
            }),            
        }
    };

    anims.spring = {
        .yellow = an(Animation{
            .frames = { Sprite { objTex, {876,37,28,16}, {14,8}}, }
            }),
        .yellowExpanded = an(Animation{
            .frames = { Sprite { objTex, {894,61,28,32}, {14,24}}, }
            }),
        .red = an(Animation{
            .frames = { Sprite { objTex, {848,45,28,16}, {14,8}}, }
            }),
        .redExpanded = an(Animation{
            .frames = { Sprite { objTex, {866,61,28,32}, {14,24}}, }
            }),
    };

    anims.killScore = {
        .n10 = an(Animation{
            .frames = { Sprite { objTex, {876,53,9,8}, {4,4}}, }
            }),
        .n100 = an(Animation{
            .frames = { Sprite { objTex, {885,53,14,8}, {7,4}}, }
            }),
        .n200 = an(Animation{
            .frames = { Sprite { objTex, {904,37,15,8}, {7,4}}, }
            }),
        .n500 = an(Animation{
            .frames = { Sprite { objTex, {918,53,15,8}, {7,4}}, }
            }),
        .n1_000 = an(Animation{
            .frames = { Sprite { objTex, {899,53,19,8}, {9,4}}, }
            }),
        .n10_000 = an(Animation{
            .frames = { Sprite { objTex, {904,45,24,8}, {12,4}}, }
            }),
    };

    anims.act = {
        .act = an(Animation{
            .frames = { Sprite { objTex, {928,40,24,8}, {12,4}}, }
        }),
        .actBack = an(Animation{
            .frames = { Sprite { objTex, {968,40,16,16}, {8,8}}, }
        }),
        .d1 = an(Animation{
            .frames = { Sprite { objTex, {933,48,7,24},  {3,12}}, }
        }),
        .d2 = an(Animation{
            .frames = { Sprite { objTex, {940,48,14,24}, {7,12}}, }
        }),
        .d3 = an(Animation{
            .frames = { Sprite { objTex, {954,48,14,24}, {7,12}}, }
        }),
        .ellipse = an(Animation{
            .frames = { Sprite { objTex, {1334,0,56,56}, {28,28}}, }
        }),
    };

    anims.ring = {
        .idle = an(Animation{
            .frames = {
                Sprite { objTex, {984,40,16,16}, {8,8}},
                Sprite { objTex, {968,56,12,16}, {6,8}},
                Sprite { objTex, {980,56,6,16},  {3,8}},
                Sprite { objTex, {986,56,12,16}, {6,8}},
            }
        }),
        .stars = an(Animation{
            .frames = {
                Sprite { objTex, {935,72,16,14}, {8,7}},
                Sprite { objTex, {951,72,16,14}, {8,7}},
                Sprite { objTex, {967,72,16,14}, {8,7}},
                Sprite { objTex, {983,72,16,14}, {8,7}},
            }
        })
    };

    anims.sfx = {
        .dust = an(Animation{
            .frames = {
                    Sprite { objTex, {1000,32,13,14}, {6,7}},
                    Sprite { objTex, {1000,46,11,12}, {5,6}},
                    Sprite { objTex, {1001,58,11,9}, {5,4}},
                    Sprite { objTex, {1002,67,8,10}, {4,5}},
                }
        }),
        .explosion = an(Animation{
            .frames = {
                    Sprite { objTex, {1166,0,20,16}, {10,8}},
                    Sprite { objTex, {1165,16,26,30}, {13,15}},
                    Sprite { objTex, {1168,46,33,32}, {16,16}},
                    Sprite { objTex, {1191,0,38,38}, {19,19}},
                    Sprite { objTex, {1202,38,36,38}, {18,19}},
                }
        }),
        .invinsible = an(Animation{
            .frames = {
                    Sprite { objTex, {1238,1,48,47}, {24,23}},
                    Sprite { objTex, {1286,0,48,47}, {24,23}},
                    Sprite { objTex, {1241,48,46,460}, {23,23}},
                    Sprite { objTex, {1288,48,46,46}, {23,23}},
                }
        }),
    };

    anims.flicky = {
        .bird = an(Animation{
            .frames = { Sprite { objTex, {850,75,11,20}, {5,10}}, }
        }),
        .birdFlying = an(Animation{
            .frames = {
                Sprite { objTex, {786,84,17,15}, {8,7}},
                Sprite { objTex, {803,85,15,14}, {7,7}},
            }
        }),
    };

    anims.signPost = {
        .eggman = an(Animation{
            .frames = { Sprite { objTex, {1390,0,48,32}, {24,16}}, }
        }),
        .spin = an(Animation{
            .frames = {
                Sprite { objTex, {1390,0,48,32}, {24,16}},

                Sprite { objTex, {1334,56,32,32}, {16,16}},
                Sprite { objTex, {1398,64,8,32}, {4,16}},
                Sprite { objTex, {1366,64,32,32}, {16,16}},
                
                Sprite { objTex, {1390,32,48,32}, {24,16}},
                
                Sprite { objTex, {1334,56,32,32}, {16,16}},
                Sprite { objTex, {1398,64,8,32}, {4,16}},
                Sprite { objTex, {1366,64,32,32}, {16,16}},
            }
        }),
        .sonic = an(Animation{
            .frames = { Sprite { objTex, {1390,32,48,32}, {24,16}}, }
        }),
        .stick = an(Animation{
            .frames = {
                Sprite { objTex, {1406,64,6,16}, {3,8}},
            }
        }),
    };

    anims.enemies = {
        .chopper = an(Animation{
            .frames = {
                    Sprite { objTex, {1090,0,30,32}, {15,16}},
                    Sprite { objTex, {1088,62,29,31}, {14,16}},
                }
            }),
        .crabmeat = an(Animation{
            .frames = {
                    Sprite { objTex, {1120,0,42,31}, {21,15}},
                    Sprite { objTex, {1120,31,44,31}, {22,15}},
                    Sprite { objTex, {1120,0,42,31}, {21,15}},
                    Sprite { objTex, {1120,62,48,30}, {24,14}},
                }
            }),
        .buzzbomber = {
            .body = an(Animation{
                .frames = {
                        Sprite { objTex, {670,81,44,19}, {22,9}},
                        Sprite { objTex, {1046,64,36,29}, {18,14}},
                    }
                }),
            .wings = an(Animation{
                .frames = {
                        Sprite { objTex, {714,91,35,8}, {17,4}},
                        Sprite { objTex, {749,91,37,6}, {18,3}},
                    }
                }),
            .turbo = an(Animation{
                .frames = {
                        Sprite { objTex, {681,76,6,5}, {3,2}},
                        Sprite { objTex, {687,76,10,5}, {5,2}},
                    }
                }),
            .fire = an(Animation{
                .frames = {
                        Sprite { objTex, {640,82,16,16}, {8,8}},
                        Sprite { objTex, {656,82,14,14}, {7,7}},
                    }
                }),
        },
        .motobug = an(Animation{
            .frames = {
                    Sprite { objTex, {1012,0,39,29}, {19,14}},
                    Sprite { objTex, {1051,1,39,28}, {19,13}},
                    Sprite { objTex, {1013,29,40,29}, {20,14}},
                    Sprite { objTex, {1053,32,40,28}, {20,13}},
                }
            }),
    };

    anims.bulletYellow = {
        an(Animation{
            .frames = {
                    Sprite { objTex, {1010,65,12,12}, {6,6}},
                    Sprite { objTex, {1022,65,12,12}, {6,6}},
                }
            })
    };
    anims.bulletRed = {
        an(Animation{
            .frames = {
                    Sprite { objTex, {1022,65,12,12}, {6,6}},
                    Sprite { objTex, {1034,65,12,12}, {6,6}},
                }
            })
    };

    anims.spikes ={
        an(Animation{
            .frames = {
                    Sprite::withCenterOffset( objTex, {1001, 0, 8, 32})
                }
        })
    };
        
    // .debug = an(Animation{
    //     .frames = {
    //             Sprite { objTex, {0,84,16,16}, {8,8}},
    //             Sprite { objTex, {16,84,16,16}, {8,8}},
    //             Sprite { objTex, {32,84,16,16}, {8,8}},
    //             Sprite { objTex, {48,84,16,16}, {8,8}},
    //             Sprite { objTex, {64,84,16,16}, {8,8}},
    //             Sprite { objTex, {80,84,16,16}, {8,8}},
    //         }
    //     }),

    return anims;
    // clang-format on

    return anims;
}

std::unique_ptr<IStorableResource> loadResources(ResourceStore& store, GameEnvironment& env, DeprecatedGameEnvironment& deprEnv) {
    auto res = new SonicResources;

    res->textures = loadTextures(store, env, deprEnv);
    res->fonts = loadFonts(store, res->textures.hud, deprEnv);
    res->sounds = loadSounds(store, env, deprEnv);
    res->music = loadMusic(store, env, deprEnv);
    res->animations = makeSpritesAndAnimations(store, res->textures);

    return std::unique_ptr<IStorableResource>(res);
}

int main() {
    return Game::RunGameWithSfmlBackend(loadResources);
}