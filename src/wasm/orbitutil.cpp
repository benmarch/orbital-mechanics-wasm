#include "orbitutil.h"
#include "mutil.h"

bool isCircularOrbit(const OrbitalElements &elements)
{
    return withinPrecision(elements.e, 0);
}

bool isOpenOrbit(const OrbitalElements &elements)
{
    return withinPrecision(elements.e.getMagnitude(), 1) || elements.e.getMagnitude() >= 1;
}

bool isEquatorialOrbit(const OrbitalElements &elements)
{
    return withinPrecision(elements.i, 0, 0.0001);
}
