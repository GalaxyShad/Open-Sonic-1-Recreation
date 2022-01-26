#include "GameMath.h"

using namespace gmath;

int gmath::sign(int val) {
    if (val > 0)
        return 1;
    else if (val < 0)
        return -1;
    else
        return 0; 
}

float gmath::fsign(float val) {
    if (val > 0.0)
        return 1.0;
    else if (val < 0.0)
        return -1.0;
    else
        return 0.0; 
}
