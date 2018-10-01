#include <emscripten/bind.h>
#include "Orbit.h"

using namespace emscripten;

Orbit::Orbit()
{
    m_elements_generator = new ClassicalElementsGenerator();
    m_state_vector_generator = new StateVectorGenerator();
}

Orbit::~Orbit()
{
    delete m_elements_generator;
    delete m_state_vector_generator;
}

ClassicalElements Orbit::generateClassicalElements(StateVectors &stateVectors)
{
    return m_elements_generator->generateFromStateVectors(stateVectors.position, stateVectors.velocity);
}

StateVectors Orbit::generateStateVectors(ClassicalElements elements)
{
    return m_state_vector_generator->generateFromClassicalElements(elements);
}

EMSCRIPTEN_BINDINGS(orbit_bindings) {
    class_<Orbit>("Orbit")
        .constructor()
        .function("generateClassicalElements", &Orbit::generateClassicalElements)
        .function("generateStateVectors", &Orbit::generateStateVectors);
}