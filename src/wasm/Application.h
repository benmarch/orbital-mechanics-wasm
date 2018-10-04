#ifndef ORBITAL_MECHANICS_APPLICATION_H
#define ORBITAL_MECHANICS_APPLICATION_H

#include <iostream>
#include "Orbit.h"

class Application
{
public:
    Orbit createOrbit(StateVectors stateVectors);
    Orbit createOrbit(OrbitalElements elements);
};

#endif //ORBITAL_MECHANICS_APPLICATION_H
