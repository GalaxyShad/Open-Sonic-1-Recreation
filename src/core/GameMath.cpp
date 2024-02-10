#include "GameMath.h"

using namespace gmath;

int gmath::sign(int val) {
    return (val > 0) ?  1  : 
           (val < 0) ? -1 
                     :  0;
}

float gmath::fsign(float val) {
    return (val > 0.f) ?  1.f  : 
           (val < 0.f) ? -1.f 
                       :  0.f;
}
