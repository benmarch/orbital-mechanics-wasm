#include <emscripten/bind.h>
#include "Orbit.h"

using namespace emscripten;

Orbit::Orbit()
{
    m_orbitalElementsGenerator = new OrbitalElementsGenerator();
    m_stateVectorGenerator = new StateVectorGenerator();
}

Orbit::~Orbit()
{
    delete m_orbitalElementsGenerator;
    delete m_stateVectorGenerator;
}

OrbitalElements Orbit::generateOrbitalElements(StateVectors &stateVectors)
{
    return m_orbitalElementsGenerator->generateFromStateVectors(stateVectors.position, stateVectors.velocity);
}

StateVectors Orbit::generateStateVectors(OrbitalElements elements)
{
    return m_stateVectorGenerator->generateFromOrbitalElements(elements);
}

EMSCRIPTEN_BINDINGS(orbit_bindings) {
    class_<Orbit>("Orbit")
        .constructor()
        .function("generateOrbitalElements", &Orbit::generateOrbitalElements)
        .function("generateStateVectors", &Orbit::generateStateVectors);
}