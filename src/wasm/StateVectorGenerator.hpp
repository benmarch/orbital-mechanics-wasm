#ifndef ORBITAL_MECHANICS_STATEVECTORGENERATOR_H
#define ORBITAL_MECHANICS_STATEVECTORGENERATOR_H

#include <functional>
#include "constants.hpp"
#include "OrbitalElementsGenerator.hpp"

class StateVectorGenerator
{
public:
    StateVectorGenerator();
    StateVectorGenerator(double mu);

    StateVectors generateFromOrbitalElements(OrbitalElements elements);
    void setMu(double mu);

private:
    double m_mu;
    StateVectors m_state_vectors;
    OrbitalElements m_elements;
    RotationMatrix m_rotation_matrix;

    void generate();

    void determineRotationMatrix();
    Vector rotateToIJK(Vector &vec);
    void calculatePosition();
    void calculateVelocity();
};


#endif //ORBITAL_MECHANICS_STATEVECTORGENERATOR_H
