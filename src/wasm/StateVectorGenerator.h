#ifndef ORBITAL_MECHANICS_STATEVECTORGENERATOR_H
#define ORBITAL_MECHANICS_STATEVECTORGENERATOR_H

#include <functional>
#include "constants.h"
#include "OrbitalElementsGenerator.h"

class StateVectorGenerator
{
public:
    StateVectors generateFromOrbitalElements(OrbitalElements elements);

private:
    StateVectors m_state_vectors;
    OrbitalElements m_elements;
    RotationMatrix m_rotation_matrix;

    void determineRotationMatrix();
    Vector rotateToIJK(Vector &vec);
    void calculatePosition();
    void calculateVelocity();
};


#endif //ORBITAL_MECHANICS_STATEVECTORGENERATOR_H
