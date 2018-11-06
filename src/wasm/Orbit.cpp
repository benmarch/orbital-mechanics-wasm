#include <emscripten/bind.h>
#include "Orbit.hpp"
#include "orbitutil.hpp"
#include "constants.hpp"

Orbit::Orbit(): Orbit(EARTH)
{

}

Orbit::Orbit(CelestialBody orbitingBody): Orbit(orbitingBody.getMu())
{

}

Orbit::Orbit(double mu):
    m_orbitalElementsGenerator{new OrbitalElementsGenerator(mu)},
    m_stateVectorGenerator{new StateVectorGenerator(mu)},
    m_mu{mu}
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

double Orbit::getRadius() const
{
    return getRadius(m_elements.nu);
}

double Orbit::getVelocity() const
{
    return getVelocity(m_elements.nu);
}

double Orbit::getRadius(double nu) const
{
    if (isCircular()) {
        return m_stateVectors.position.getMagnitude();
    }

    StateVectorGenerator stateVectorGenerator{m_mu};
    OrbitalElements elements = m_elements;

    elements.nu = nu;
    return stateVectorGenerator.generateFromOrbitalElements(elements).position.getMagnitude();
}

double Orbit::getVelocity(double nu) const
{
    if (isCircular()) {
        return m_stateVectors.velocity.getMagnitude();
    }

    StateVectorGenerator stateVectorGenerator{m_mu};
    OrbitalElements elements = m_elements;

    elements.nu = nu;
    return stateVectorGenerator.generateFromOrbitalElements(elements).velocity.getMagnitude();
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

double Orbit::getMu() const
{
    return m_mu;
}

void Orbit::setMu(double mu)
{
    m_mu = mu;
    m_stateVectorGenerator->setMu(mu);
    m_orbitalElementsGenerator->setMu(mu);
}

EMSCRIPTEN_BINDINGS(orbit_bindings) {
    using namespace emscripten;

    class_<Orbit>("Orbit")
        .constructor()
        .constructor<double>()
        .function("updateFromStateVectors", &Orbit::updateFromStateVectors)
        .function("updateFromOrbitalElements", &Orbit::updateFromOrbitalElements)
        .function("setMu", &Orbit::setMu)
        .property("elements", &Orbit::getElements)
        .property("stateVectors", &Orbit::getStateVectors)
        .property("mu", &Orbit::getMu)
        .property("isCircular", &Orbit::isCircular)
        .property("isOpen", &Orbit::isOpen)
        .property("isEquatorial", &Orbit::isEquatorial);
}