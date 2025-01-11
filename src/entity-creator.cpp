#include "entity-creator.hpp"

#include "core/game_enviroment/artist/ArtistStructs.h"
#include "entities/_index.hpp"
#include "entities/sonic-1/enemies/EnemiesGHZ.h"
#include "entities/sonic-1/ghz/GimmicksGHZ.h"
#include "sonic/SonicResources.h"


Entity* EntityCreatorSonic1::create(EntityPlacement entPlacement) {
    Entity* res = nullptr;

    // printf(
    //     "%s x=%d y=%d fv=%d fh=%d resp=%d args=%02X\n", 
    //     ObjectID_S1ToString((ObjectID_S1)entPlacement.objectId),
    //     entPlacement.x, entPlacement.y,
    //     entPlacement.flipVertical, entPlacement.flipHorizontal,
    //     entPlacement.shouldRespawn,
    //     entPlacement.additionalArgs
    // );

    if ((res = createGeneral(entPlacement))) return res;
    if ((res = createEnemies(entPlacement))) return res;
    if ((res = createOther(entPlacement)))   return res;
    
    // Placeholder
    return new Ring(v2f(entPlacement.x, entPlacement.y), m_entityList, m_terrain);
}

Entity* EntityCreatorSonic1::createGeneral(EntityPlacement eplc) {
    v2f position = v2f(eplc.x, eplc.y);
    
    switch ((ObjectID_S1)eplc.objectId) {
        case (ObjectID_S1::S1_RING): {
            uint8_t count = (eplc.additionalArgs & 0x0F);
            uint8_t direction = ((eplc.additionalArgs & 0xF0) >> 4);

            return Ring::CreateRow(
                m_entityList, 
                m_terrain, 
                position, 
                count + 1, 
                (-direction+1) * 90.0f, 
                8
            );
        }
        
        case (ObjectID_S1::S1_MONITOR): {
            switch (eplc.additionalArgs) {
                case 0x02: return new Monitor(position, Monitor::M_LIVE);
                case 0x03: return new Monitor(position, Monitor::M_SPEED);
                case 0x04: return new Monitor(position, Monitor::M_SHIELD);
                case 0x05: return new Monitor(position, Monitor::M_INVINCIBILITY);
                case 0x06: return new Monitor(position, Monitor::M_RINGS);
                default:   return new Monitor(position, Monitor::M_RINGS);
            }
        }

        case (ObjectID_S1::S1_SPRINGS): {
            uint8_t flagHorizontal = (eplc.additionalArgs & 0xF0) >> 4;
            uint8_t flagYellow = eplc.additionalArgs & 0x0F;

            Spring::Rotation rotation = 
                (flagHorizontal) 
                    ? (eplc.flipHorizontal) ? Spring::R_LEFT : Spring::R_RIGHT
                    : (eplc.flipVertical)   ? Spring::R_DOWN : Spring::R_UP;

            return new Spring(position, !flagYellow, rotation);
        }

        case (ObjectID_S1::S1_SPIKES): {
            // if ((eplc.additionalArgs & 0b00010000) == 0)
            return new Spikes(position, eplc.additionalArgs, m_entityList.legacy_rawPool());
        }

        case (ObjectID_S1::S1_EGG_PRISON):{
            auto& animsEggman = store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.signPost.eggman);
            auto& animsSpin = store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.signPost.spin);
            auto& animsSonic = store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.signPost.sonic);
            auto& animsStick = store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.signPost.stick);
            SignPostAnimations anims = {animsEggman,animsSpin,animsSonic,animsStick};
            return new SignPost(position,anims);
        }

        case (ObjectID_S1::S1_END_OF_LEVEL_SIGNPOST):{
            auto& animsEggman = store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.signPost.eggman);
            auto& animsSpin = store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.signPost.spin);
            auto& animsSonic = store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.signPost.sonic);
            auto& animsStick = store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.signPost.stick);
            SignPostAnimations anims = {animsEggman,animsSpin,animsSonic,animsStick};
            return new SignPost(position,anims);
        }


        default: 
            return nullptr;
    }
}

Entity* EntityCreatorSonic1::createEnemies(EntityPlacement eplc) {
    v2f position = v2f(eplc.x, eplc.y);
      
    
    switch ((ObjectID_S1)eplc.objectId) {
        case (ObjectID_S1::S1_MOTOBUG_ENEMY): {
            auto& anim = store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.enemies.motobug);
            return new EnMotobug(position, anim, m_terrain);///
            // return new EnMotobug(position, m_terrain);
        }

        case (ObjectID_S1::S1_CHOPPER): {
            auto& anim = store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.enemies.chopper);
            return new EnChopper(position, anim);
        }

        case (ObjectID_S1::S1_CRABMEAT): {
            auto& anim = store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.enemies.crabmeat);
            auto& animBullet = store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.bulletRed);
            return new EnCrab(position, anim, m_entityList.legacy_rawPool(), m_terrain, animBullet);
            // return new EnCrab(position, m_entityList.legacy_rawPool(), m_terrain);
        }
        
        case (ObjectID_S1::S1_BUZZ_BOMBER):  {
            auto& animBody = store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.enemies.buzzbomber.body);
            auto& animWings = store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.enemies.buzzbomber.wings);
            auto& animTurbo = store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.enemies.buzzbomber.turbo);
            auto& animFire = store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.enemies.buzzbomber.fire);
            auto& animBullet = store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.bulletYellow);
            EnBuzzAnimations anims = {animBody,animWings,animTurbo,animFire};
            return new EnBuzz(position, anims, animBullet);
        }
        default: 
            return nullptr;
    }
}

Entity* EntityCreatorSonic1::createOther(EntityPlacement eplc) {
    v2f position = v2f(eplc.x, eplc.y);
    
    switch ((ObjectID_S1)eplc.objectId) {
        case (ObjectID_S1::S1_GHZ_BRIDGE):{
            auto& spr = store_.get<artist_api::Sprite>(store_.map<SonicResources>().animations.sprites.greenHillZone.bridge);
            return new GimGHZ_BridgeController(position, spr, eplc.additionalArgs, m_entityList.legacy_rawPool());
        }
            
        case (ObjectID_S1::S1_PLATFORMS_GHZ_SLZ_SYZ): {
            switch (eplc.additionalArgs & 0x0F) {
                case 0x00:{
                    auto& spr = store_.get<artist_api::Sprite>(store_.map<SonicResources>().animations.sprites.greenHillZone.platform);
                    return new GimGHZ_Platform(position, spr, false, 0);
                }
                case 0x01:{
                    auto& spr = store_.get<artist_api::Sprite>(store_.map<SonicResources>().animations.sprites.greenHillZone.platform);
                    return new GimGHZ_Platform(position, spr, GimGHZ_Platform::DIR_LEFT, true);
                }
                case 0x02:{
                    auto& spr = store_.get<artist_api::Sprite>(store_.map<SonicResources>().animations.sprites.greenHillZone.platform);
                    return new GimGHZ_Platform(position, spr, GimGHZ_Platform::DIR_UP, true);
                }
                case 0x03:{
                    auto& spr = store_.get<artist_api::Sprite>(store_.map<SonicResources>().animations.sprites.greenHillZone.platform);
                    return new GimGHZ_Platform(position, spr, false, 0, true);
                } // Falls when stood on
                //case 0x04: break; // Falls immediately
                case 0x05:{
                    auto& spr = store_.get<artist_api::Sprite>(store_.map<SonicResources>().animations.sprites.greenHillZone.platform);
                    return new GimGHZ_Platform(position, spr, GimGHZ_Platform::DIR_RIGHT, true);
                }
                case 0x06:{
                    auto& spr = store_.get<artist_api::Sprite>(store_.map<SonicResources>().animations.sprites.greenHillZone.platform);
                    return new GimGHZ_Platform(position, spr, GimGHZ_Platform::DIR_DOWN, true);
                }
                //case 0x07: break; // x7: Rises when switch in high nybble is pressed
                //case 0x08: break; // x8: Rises immediately
                case 0x09:{
                    auto& spr = store_.get<artist_api::Sprite>(store_.map<SonicResources>().animations.sprites.greenHillZone.platform);
                    return new GimGHZ_Platform(position, spr, false, false);
                } // x9: Doesn't move
                //case 0x0A: break; // xA: Large - moves up and down
                //case 0x0B: break; // xB: Moves up and down slowly
                //case 0x0C: break; // xC: Moves up and down slowly
                default:{
                    auto& spr = store_.get<artist_api::Sprite>(store_.map<SonicResources>().animations.sprites.greenHillZone.platform);
                    return new GimGHZ_Platform(position, spr, false, false);
                }
            }
        }
        
            
        case (ObjectID_S1::S1_COLLAPSING_LEDGE_FROM_GHZ):{
            auto& spr = store_.get<artist_api::Sprite>(store_.map<SonicResources>().animations.sprites.greenHillZone.swingPlatform);
            return new GimGHZ_SlpPlatform(position, spr, m_entityList.legacy_rawPool(), (bool)eplc.additionalArgs);
        }

        case (ObjectID_S1::S1_ZONE_SCENERY_OBJECT):{
            auto& spr = store_.get<artist_api::Sprite>(store_.map<SonicResources>().animations.sprites.greenHillZone.bridgeColumn);
            return new GimGhz_BridgeColumn(position, spr, (bool)eplc.flipHorizontal);
            // return new GimGhz_BridgeColumn(position, (bool)eplc.flipHorizontal);
        }

        case (ObjectID_S1::S1_GHZ_PURPLE_ROCK):{
            auto& spr = store_.get<artist_api::Sprite>(store_.map<SonicResources>().animations.sprites.greenHillZone.stone);
            return new GimGHZ_Stone(position,spr);
        }

        case (ObjectID_S1::S1_WALL_BARRIER_FROM_GHZ):{
            auto& spr = store_.get<artist_api::Sprite>(store_.map<SonicResources>().animations.sprites.greenHillZone.wallLeft);
            return new GimGHZ_Wall(position, spr, eplc.additionalArgs & 0x0F, eplc.additionalArgs & 0xF0);
        }

        default:
            return nullptr;
    }
}
