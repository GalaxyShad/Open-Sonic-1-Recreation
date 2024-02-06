#pragma once

enum class ZoneSonic3K {
    ANGEL_ISLAND,
    HYDROCITY,
    MARBLE_GARDEN,
    CARNIVAL_NIGHT,
    ICE_CAP,
    LAUNCH_BASE,

    MUSHROOM_HILL,
    FLYING_BATTERY,
    SANDOPOLIS,
    LAVA_REEF,
    HIDDEN_PALACE,
    SKY_SANCTUARY,
    DEATH_EGG,
    DOOMSDAY,
};

static const char* ZoneSonic3KToString(ZoneSonic3K zone) {
    static const char* str[] = {
        "ANGEL ISLAND",
        "HYDROCITY",
        "MARBLE GARDEN",
        "CARNIVAL NIGHT",
        "ICE CAP",
        "LAUNCH BASE",

        "MUSHROOM HILL",
        "FLYING BATTERY",
        "SANDOPOLIS",
        "LAVA REEF",
        "HIDDEN PALACE",
        "SKY SANCTUARY",
        "DEATH EGG",
        "DOOMSDAY",
    };

    return str[(int)zone];
};