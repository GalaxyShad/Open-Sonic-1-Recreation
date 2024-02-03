#pragma once

#include <stdint.h>

#include "entities/entity-interfaces.hpp"

struct EntityPlacement {
    uint16_t x;
    uint16_t y;
    
    bool flipVertical;
    bool flipHorizontal;

    bool shouldRespawn;

    uint8_t  objectId;
    uint8_t  additionalArgs;

    static EntityPlacement fromSonic1(uint8_t* buff) {
        // XXXX XXXX XXXX XXXX  VH00 YYYY YYYY YYYY  RIII IIII AAAA AAAA

        EntityPlacement res = { 0 };

        res.x               = ((uint16_t)buff[0] << 8) | buff[1];
		res.y               = (((uint16_t)buff[2] << 8) & 0x0FFF) | buff[3];

		res.flipVertical    = buff[2] >> 7;
		res.flipHorizontal  = (buff[2] & 0b01000000) >> 6 ;

		res.objectId 	    = buff[4] & 0b01111111;
		res.shouldRespawn   = buff[4] >> 7;

		res.additionalArgs  = buff[5];

        return res;
    }
};