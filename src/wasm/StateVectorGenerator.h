#ifndef ORBITAL_MECHANICS_STATEVECTORGENERATOR_H
#define ORBITAL_MECHANICS_STATEVECTORGENERATOR_H

#include <functional>
#include "ClassicalElementsGenerator.h"

typedef struct {
    Vector position;
    Vector velocity;
} StateVectors;

typedef struct {
    Vector row1;
    Vector row2;
    Vector row3;
} RotationMatrix;

class StateVectorGenerator {
public:
    StateVectors generateFromClassicalElements(ClassicalElements elements);

private:
    StateVectors m_state_vectors;
    ClassicalElements m_elements;
    RotationMatrix m_rotation_matrix;

    void determineRotationMatrix();
    Vector rotateToIJK(Vector &vec);
    void calculatePosition();
    void calculateVelocity();
};


#endif //ORBITAL_MECHANICS_STATEVECTORGENERATOR_H
