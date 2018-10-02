#include <emscripten/bind.h>
#include "Orbit.h"

using namespace emscripten;

Orbit::Orbit()
{
    m_classicalElementsGenerator = new ClassicalElementsGenerator();
    m_stateVectorGenerator = new StateVectorGenerator();
}

Orbit::~Orbit()
{
    delete m_classicalElementsGenerator;
    delete m_stateVectorGenerator;
}

ClassicalElements Orbit::generateClassicalElements(StateVectors &stateVectors)
{
    return m_classicalElementsGenerator->generateFromStateVectors(stateVectors.position, stateVectors.velocity);
}

StateVectors Orbit::generateStateVectors(ClassicalElements elements)
{
    return m_stateVectorGenerator->generateFromClassicalElements(elements);
}

EMSCRIPTEN_BINDINGS(orbit_bindings) {
    class_<Orbit>("Orbit")
        .constructor()
        .function("generateClassicalElements", &Orbit::generateClassicalElements)
        .function("generateStateVectors", &Orbit::generateStateVectors);
}