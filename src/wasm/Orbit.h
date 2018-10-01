#ifndef ORBITAL_MECHANICS_ORBIT_H
#define ORBITAL_MECHANICS_ORBIT_H

#include "Vector.h"
#include "ClassicalElementsGenerator.h"
#include "StateVectorGenerator.h"

class Orbit {
public:
    Orbit();
    ~Orbit();

    ClassicalElements generateClassicalElements(StateVectors &stateVectors);
    StateVectors generateStateVectors(ClassicalElements elements);

private:
    ClassicalElementsGenerator *m_elements_generator;
    StateVectorGenerator *m_state_vector_generator;
};


#endif //ORBITAL_MECHANICS_ORBIT_H
