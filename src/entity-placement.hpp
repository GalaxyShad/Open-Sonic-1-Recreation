#pragma once

#include <stdint.h>

struct EntityPlacement {
    uint16_t x             : 16;
    
    uint8_t flipVertical   : 1;
    uint8_t flipHorizontal : 1;
    uint8_t                : 2;
    uint16_t y             : 12;

    uint8_t shouldRespawn  : 1;
    uint8_t objectId       : 7;

    uint8_t additionalArgs : 8;

    static EntityPlacement from6ByteBuffer(uint8_t* buff) {
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