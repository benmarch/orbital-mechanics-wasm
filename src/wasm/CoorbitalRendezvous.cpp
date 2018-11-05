#include <emscripten/bind.h>
#include <math.h>
#include "CoorbitalRendezvous.hpp"
#include "mutil.hpp"

CoorbitalRendezvous::CoorbitalRendezvous(Orbit &orbitFrom, Orbit &orbitTo):
    m_orbitFrom{orbitFrom}, m_orbitTo{orbitTo}
{
    // ensure the orbits are coorbital
    if (
        withinPrecision(orbitFrom.getElements().a,  orbitTo.getElements().a) &&
        withinPrecision(orbitFrom.getElements().e,  orbitTo.getElements().e) &&
        withinPrecision(orbitFrom.getElements().i,  orbitTo.getElements().i) &&
        withinPrecision(orbitFrom.getElements().Om, orbitTo.getElements().Om) &&
        withinPrecision(orbitFrom.getElements().o, orbitTo.getElements().o)
    ) {
        calculateInitialPhaseAngle();
        calculateTravelPhaseAngle();
        calculateTargetAngularVelocity();
        calculateTOF();
        calculatePhasingOrbit();
    } else {
        std::cerr << "Orbits are not coorbital!" << std::endl;
    }
}

void CoorbitalRendezvous::calculateInitialPhaseAngle()
{
    if (m_orbitFrom.isEquatorial() && m_orbitFrom.isCircular()) {
        m_initialPhaseAngle = m_orbitTo.getElements().l - m_orbitFrom.getElements().l;
    } else if (m_orbitFrom.isCircular()) {
        m_initialPhaseAngle = m_orbitTo.getElements().u - m_orbitFrom.getElements().u;
    } else if (m_orbitFrom.isEquatorial()) {
        m_initialPhaseAngle = m_orbitTo.getElements().Pi - m_orbitFrom.getElements().Pi;
    } else {
        m_initialPhaseAngle = m_orbitTo.getElements().nu - m_orbitFrom.getElements().nu;
    }
}

void CoorbitalRendezvous::calculateTravelPhaseAngle()
{
    m_travelPhaseAngle = 2 * M_PI - m_initialPhaseAngle;
}

void CoorbitalRendezvous::calculateTargetAngularVelocity()
{
    m_tgtAngularV = sqrt(MU / pow(m_orbitTo.getElements().a, 3));
}

void CoorbitalRendezvous::calculateTOF()
{
    m_tof = m_travelPhaseAngle / m_tgtAngularV;
}

void CoorbitalRendezvous::calculatePhasingOrbit()
{
    OrbitalElements elements = m_orbitFrom.getElements();

    double a_phase = cbrt(MU * pow(m_tof / (2 * M_PI), 2));

    bool isPerigee = m_initialPhaseAngle >= M_PI;
    short adder = isPerigee ? -1 : 1;

    elements.a = a_phase;
    elements.e = Vector{-1 * (m_orbitFrom.getRadius() / a_phase + adder), 0, 0};

    m_orbitPhasing = Orbit{};
    m_orbitPhasing.updateFromOrbitalElements(elements);
}

double CoorbitalRendezvous::getPhaseAngle() const
{
    return m_travelPhaseAngle;
}

double CoorbitalRendezvous::getTOF() const
{
    return m_tof;
}

double CoorbitalRendezvous::getAngularVelocity() const
{
    return m_tgtAngularV;
}

Orbit CoorbitalRendezvous::getPhasingOrbit() const
{
    return m_orbitPhasing;
}

EMSCRIPTEN_BINDINGS(coorbital_rendezvous_bindings)
{
    using namespace emscripten;

    class_<CoorbitalRendezvous>("CoorbitalRendezvous")
        .constructor<Orbit&, Orbit&>()
        .property("phaseAngle", &CoorbitalRendezvous::getPhaseAngle)
        .property("timeOfFlight", &CoorbitalRendezvous::getTOF)
        .property("angularVelocity", &CoorbitalRendezvous::getAngularVelocity)
        .property("phasingOrbit", &CoorbitalRendezvous::getPhasingOrbit);
}

