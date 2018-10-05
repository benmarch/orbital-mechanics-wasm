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

    void updateFromOrbitalElements(OrbitalElements elements);
    void updateFromStateVectors(StateVectors &stateVectors);
    OrbitalElements getElements() const;
    StateVectors getStateVectors() const;

    bool isCircular() const;
    bool isOpen() const;
    bool isEquatorial() const;

private:
    OrbitalElementsGenerator *m_orbitalElementsGenerator;
    StateVectorGenerator *m_stateVectorGenerator;
    OrbitalElements m_elements;
    StateVectors m_stateVectors;
};


#endif //ORBITAL_MECHANICS_ORBIT_H
