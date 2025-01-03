#include "entity-creator.hpp"

#include "core/game_enviroment/artist/ArtistStructs.h"
#include "entities/_index.hpp"
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

        case (ObjectID_S1::S1_SPIKES): 
            // if ((eplc.additionalArgs & 0b00010000) == 0)
            return new Spikes(position, eplc.additionalArgs, m_entityList.legacy_rawPool());

        case (ObjectID_S1::S1_EGG_PRISON): 
            return new SignPost(position);


        case (ObjectID_S1::S1_END_OF_LEVEL_SIGNPOST): 
            return new SignPost(position);


        default: 
            return nullptr;
    }
}

Entity* EntityCreatorSonic1::createEnemies(EntityPlacement eplc) {
    v2f position = v2f(eplc.x, eplc.y);

    
    
    switch ((ObjectID_S1)eplc.objectId) {
        case (ObjectID_S1::S1_MOTOBUG_ENEMY): 
            return new EnMotobug(position, m_terrain);

        case (ObjectID_S1::S1_CHOPPER): {

            auto& anim = store_.get<artist_api::Animation>(store_.map<SonicResources>().animations.enemies.chopper);

            return new EnChopper(position, anim);
        }

        case (ObjectID_S1::S1_CRABMEAT): 
            return new EnCrab(position, m_entityList.legacy_rawPool(), m_terrain);
        
        case (ObjectID_S1::S1_BUZZ_BOMBER): 
            return new EnBuzz(position);

        default: 
            return nullptr;
    }
}

Entity* EntityCreatorSonic1::createOther(EntityPlacement eplc) {
    v2f position = v2f(eplc.x, eplc.y);
    
    switch ((ObjectID_S1)eplc.objectId) {
        case (ObjectID_S1::S1_GHZ_BRIDGE):  
            return new GimGHZ_BridgeController(position, eplc.additionalArgs, m_entityList.legacy_rawPool());
        
        case (ObjectID_S1::S1_PLATFORMS_GHZ_SLZ_SYZ): 
            switch (eplc.additionalArgs & 0x0F) {
                case 0x00: return new GimGHZ_Platform(position, false, 0); break;
                case 0x01: return new GimGHZ_Platform(position, GimGHZ_Platform::DIR_LEFT, true); break;
                case 0x02: return new GimGHZ_Platform(position, GimGHZ_Platform::DIR_UP, true); break;
                case 0x03: return new GimGHZ_Platform(position, false, 0, true); break; // Falls when stood on
                //case 0x04: break; // Falls immediately
                case 0x05: return new GimGHZ_Platform(position, GimGHZ_Platform::DIR_RIGHT, true); break;
                case 0x06: return new GimGHZ_Platform(position, GimGHZ_Platform::DIR_DOWN, true); break;
                //case 0x07: break; // x7: Rises when switch in high nybble is pressed
                //case 0x08: break; // x8: Rises immediately
                case 0x09: return new GimGHZ_Platform(position, false, false); break; // x9: Doesn't move
                //case 0x0A: break; // xA: Large - moves up and down
                //case 0x0B: break; // xB: Moves up and down slowly
                //case 0x0C: break; // xC: Moves up and down slowly
                default: return new GimGHZ_Platform(position, false, false); break;
            }
            
        case (ObjectID_S1::S1_COLLAPSING_LEDGE_FROM_GHZ): 
            return new GimGHZ_SlpPlatform(position, m_entityList.legacy_rawPool(), (bool)eplc.additionalArgs);
        
        case (ObjectID_S1::S1_ZONE_SCENERY_OBJECT): 
            return new GimGhz_BridgeColumn(position, (bool)eplc.flipHorizontal);
        
        case (ObjectID_S1::S1_GHZ_PURPLE_ROCK):
            return new GimGHZ_Stone(position);

        case (ObjectID_S1::S1_WALL_BARRIER_FROM_GHZ): 
            return new GimGHZ_Wall(position, eplc.additionalArgs & 0x0F, eplc.additionalArgs & 0xF0);

        default:
            return nullptr;
    }
}
