#ifndef OS1R_SONICRESOURCES_H
#define OS1R_SONICRESOURCES_H

#include "core/game_enviroment/IStorableResource.h"
#include "core/game_enviroment/ResourceStore.h"

struct SonicResources : public IStorableResource {
    struct Textures {
        ResourceID objects;
        ResourceID objectsGhz;
        ResourceID hud;
    } textures;

    struct Fonts {
        ResourceID general;
        ResourceID s1Credits;
        ResourceID s1TitleScreen;
        ResourceID s1HudDigits;
    } fonts;

    struct Sounds {
        struct Player {
            ResourceID jump;
            ResourceID hurt;
            ResourceID skid;
            ResourceID spikes;
            ResourceID shield;
            ResourceID dash;
            ResourceID roll;

        } player;

        ResourceID ring;
        ResourceID ringLoss;
        ResourceID spring;

        ResourceID endSignSpin;
        ResourceID ding;

        ResourceID beep;
        ResourceID breaking;
    } sounds;

    struct Music {
        ResourceID title;
        ResourceID stageClear;
        ResourceID greenHillZone;
    } music;
};


#endif // OS1R_SONICRESOURCES_H
