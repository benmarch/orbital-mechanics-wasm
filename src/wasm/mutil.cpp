#include <math.h>

#include "mutil.h"

double fixError(double val)
{
    if (abs(val) < PRECISION) {
        return 0;
    }

    return val;
}

Vector fixError(const Vector &vec) {
    return Vector{fixError(vec.getX()), fixError(vec.getY()), fixError(vec.getZ())};
}

bool withinPrecision(double a, double b)
{
    return abs(a - b) < PRECISION;
}