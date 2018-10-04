#ifndef ORBITAL_MECHANICS_ORBIT_H
#define ORBITAL_MECHANICS_ORBIT_H

#include "Vector.h"
#include "constants.h"
#include "OrbitalElementsGenerator.h"
#include "StateVectorGenerator.h"

class Orbit
{
public:
    Orbit();
    ~Orbit();

    OrbitalElements generateOrbitalElements(StateVectors &stateVectors);
    StateVectors generateStateVectors(OrbitalElements elements);

private:
    OrbitalElementsGenerator *m_orbitalElementsGenerator;
    StateVectorGenerator *m_stateVectorGenerator;
};


#endif //ORBITAL_MECHANICS_ORBIT_H
