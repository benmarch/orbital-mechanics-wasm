#include <emscripten/bind.h>
#include "Orbit.hpp"
#include "orbitutil.hpp"

Orbit::Orbit():
    m_orbitalElementsGenerator{new OrbitalElementsGenerator()},
    m_stateVectorGenerator{new StateVectorGenerator()}
{

}

Orbit::~Orbit()
{
    delete m_orbitalElementsGenerator;
    delete m_stateVectorGenerator;
}

void Orbit::updateFromStateVectors(StateVectors &stateVectors)
{
    m_elements = m_orbitalElementsGenerator->generateFromStateVectors(stateVectors.position, stateVectors.velocity);
    m_stateVectors = stateVectors;
}

void Orbit::updateFromOrbitalElements(OrbitalElements elements)
{
    m_stateVectors =  m_stateVectorGenerator->generateFromOrbitalElements(elements);
    m_elements = elements;
}

OrbitalElements Orbit::getElements() const
{
    return m_elements;
}

StateVectors Orbit::getStateVectors() const
{
    return m_stateVectors;
}

bool Orbit::isCircular() const
{
    return isCircularOrbit(m_elements);
}

bool Orbit::isOpen() const
{
    return isOpenOrbit(m_elements);
}

bool Orbit::isEquatorial() const
{
    return isEquatorialOrbit(m_elements);
}

EMSCRIPTEN_BINDINGS(orbit_bindings) {
    using namespace emscripten;

    class_<Orbit>("Orbit")
        .constructor()
        .function("updateFromStateVectors", &Orbit::updateFromStateVectors)
        .function("updateFromOrbitalElements", &Orbit::updateFromOrbitalElements)
        .property("elements", &Orbit::getElements)
        .property("stateVectors", &Orbit::getStateVectors)
        .property("isCircular", &Orbit::isCircular)
        .property("isOpen", &Orbit::isOpen)
        .property("isEquatorial", &Orbit::isEquatorial);
}