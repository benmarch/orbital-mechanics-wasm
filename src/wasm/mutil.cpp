#include <math.h>

#include "mutil.hpp"

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

bool withinPrecision(const Vector &a, const Vector &b)
{
    return abs(a.getX() - b.getX()) < PRECISION && abs(a.getY() - b.getY()) < PRECISION && abs(a.getY() - b.getY()) < PRECISION;
}

bool withinPrecision(double a, double b, double relEpsilon)
{
    // Check if the numbers are really close -- needed when comparing numbers near zero.
    double diff = fabs(a - b);
    if (diff <= PRECISION)
        return true;

    // Otherwise fall back to Knuth's algorithm
    return diff <= ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * relEpsilon);
}

double reduceToWithin(double value, double multiple)
{
    if (withinPrecision(value, multiple)) {
        return 0;
    }

    if (value < multiple) {
        return value;
    }

    return reduceToWithin(value - multiple * floor(value / multiple), multiple);
}