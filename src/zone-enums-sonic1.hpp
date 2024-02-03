#pragma once

enum class ZoneSonic1 {
    GREEN_HILL,
    MARBLE,
    SPRING_YARD,
    LABYRINTH,
    STAR_LIGHT,
    SCRAP_BRAIN,
    FINAL,
};

static const char* ZoneSonic1ToString(ZoneSonic1 zone) {
    static const char* str[] = {
        "GREEN HILL",
        "MARBLE",
        "SPRING YARD",
        "LABYRINTH",
        "STAR LIGHT",
        "SCRAP BRAIN",
        "FINAL",
    };

    return str[(int)zone];
};

static const char* ZoneSonic1ToStringShort(ZoneSonic1 zone) {
    static const char* str[] = {
        "GHZ",
        "MZ",
        "SYZ",
        "LZ",
        "SLZ",
        "SBZ",
        "FZ",
    };

    return str[(int)zone];
};