#include <cstdint>

struct HexAngle {
    uint8_t hex;

    inline bool isRotatable() {
        return (hex != 0xFF);
    }

    inline float degrees() { 
        return ((256.0f - hex) / 256.0f) * 360.0f; 
    }

    static HexAngle fromDegrees(float degAng) { 
        return HexAngle { (uint8_t)(((360 - degAng) / 360) * 256) };
    }
};