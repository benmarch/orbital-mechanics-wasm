#ifndef ORBITAL_MECHANICS_ORBITUTILS_H
#define ORBITAL_MECHANICS_ORBITUTILS_H

#include "constants.hpp"

bool isCircularOrbit(const OrbitalElements &elements);
bool isOpenOrbit(const OrbitalElements &elements);
bool isEquatorialOrbit(const OrbitalElements &elements);

#endif //ORBITAL_MECHANICS_ORBITUTILS_H
