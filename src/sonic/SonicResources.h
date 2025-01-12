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
        ResourceID s1TitleCard;
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

    

    struct Animation {
        struct Sprite {
            struct {
                ResourceID stone;
                ResourceID swingPlatform;
                ResourceID bridge;
                ResourceID platform;

                ResourceID wallLeftWithShadow;
                ResourceID wallLeft;
                ResourceID wallRight;

                ResourceID bridgeColumn;
            } greenHillZone;
        } sprites;

        struct {
            ResourceID idle;

            ResourceID walk;
            ResourceID walkDiagonal;

            ResourceID run;
            ResourceID runDiagonal;

            ResourceID dash;
            ResourceID dashDiagonal;

            ResourceID sit;

            ResourceID roll;
            ResourceID skid;
            ResourceID hurt;
            ResourceID die;
            ResourceID push;
            ResourceID lookUp;

            ResourceID spring;
            ResourceID fall;
        } sonic;

        struct {
            ResourceID main;
            ResourceID broken;
            ResourceID noise;

            struct {
                ResourceID ring;
                ResourceID invinsibility;
                ResourceID sonic;
                ResourceID shield;
                ResourceID speedSneakers;
            } icon;
        } monitor;

        struct {
            ResourceID yellow;
            ResourceID yellowExpanded;

            ResourceID red;
            ResourceID redExpanded;
        } spring;

        struct {
            ResourceID n10;
            ResourceID n100;
            ResourceID n200;
            ResourceID n500;
            ResourceID n1_000;
            ResourceID n10_000;
        } killScore;

        struct {
            ResourceID act;
            ResourceID actBack;
            ResourceID d1, d2, d3;
            ResourceID ellipse;
        } act;

        struct {
            ResourceID idle;
            ResourceID stars;
        } ring;

        struct {
            ResourceID dust, explosion, invinsible;
        } sfx;

        struct {
            ResourceID bird;
            ResourceID birdFlying;
        } flicky;

        struct {
            ResourceID eggman;
            ResourceID spin;
            ResourceID sonic;
            ResourceID stick;
        } signPost;


        struct {
            ResourceID chopper;
            ResourceID crabmeat;
            struct {
                ResourceID body;
                ResourceID wings;
                ResourceID turbo;
                ResourceID fire;
            } buzzbomber;
            ResourceID motobug;
        } enemies;

        ResourceID bulletYellow;
        ResourceID bulletRed;

        ResourceID spikes;
    } animations;
};


#endif // OS1R_SONICRESOURCES_H
