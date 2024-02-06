#pragma once

enum class ZoneSonic2 {
    EMERALD_HILL,
    CHEMICAL_PLANT,
    AQUATIC_RUIN,
    CASINO_NIGHT,
    HILL_TOP,
    MYSTIC_CAVE,
    OIL_OCEAN,
    METROPOLIS,
    SKY_CHASE,
    WING_FORTRESS,
    DEATH_EGG,
};

static const char* ZoneSonic2ToString(ZoneSonic2 zone) {
    static const char* str[] = {
        "EMERALD HILL",
        "CHEMICAL PLANT",
        "AQUATIC RUIN",
        "CASINO NIGHT",
        "HILL TOP",
        "MYSTIC CAVE",
        "OIL OCEAN",
        "METROPOLIS",
        "SKY CHASE",
        "WING FORTRESS",
        "DEATH EGG",
    };

    return str[(int)zone];
};