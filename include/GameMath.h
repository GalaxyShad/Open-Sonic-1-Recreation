#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#define radians(deg)  ((deg) * M_PI / 180.0)

using namespace std;

namespace gmath
{
    float fsign(float val);
    int sign(int val);
}