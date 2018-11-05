#include <emscripten/bind.h>
#include <math.h>
#include "CoplanarRendezvous.hpp"
#include "Vector.hpp"
#include "mutil.hpp"
#include "constants.hpp"

CoplanarRendezvous::CoplanarRendezvous(Orbit &orbitFrom, Orbit &orbitTo):
    m_orbitFrom{orbitFrom}, m_orbitTo{orbitTo}
{
    // ensure the orbits are coplanar
    if (
        withinPrecision(orbitFrom.getElements().i,  orbitTo.getElements().i) &&
        withinPrecision(orbitFrom.getElements().Om, orbitTo.getElements().Om)
    ) {
        calculateTransferOrbit();
        calculateTOF();
        calculateInterceptorAngularVelocity();
        calculateTargetAngularVelocity();
        calculateLeadAngle();
        calculatePhaseAngle();
        calculateWaitTime();
    } else {
        std::cerr << "Orbits are not coplanar!" << std::endl;
    }
}

double CoplanarRendezvous::calculateAngularVelocity(Orbit orbit)
{
    // assuming circular orbits for now
    return sqrt(MU / pow(orbit.getElements().a, 3));
}

void CoplanarRendezvous::calculateTransferOrbit()
{
    // assuming circular orbits for now
    m_a_t = (m_orbitFrom.getElements().a + m_orbitTo.getElements().a) / 2;
}

void CoplanarRendezvous::calculateTOF()
{
    m_TOF = M_PI * sqrt(pow(m_a_t, 3) / MU);
}

void CoplanarRendezvous::calculateInterceptorAngularVelocity()
{
    m_intAngularV = calculateAngularVelocity(m_orbitFrom);
}

void CoplanarRendezvous::calculateTargetAngularVelocity()
{
    m_tgtAngularV = calculateAngularVelocity(m_orbitTo);
}

void CoplanarRendezvous::calculateLeadAngle()
{
    m_leadAngle = m_tgtAngularV * m_TOF;
}

void CoplanarRendezvous::calculatePhaseAngle()
{
    m_phaseAngle = M_PI - m_leadAngle;
}

void CoplanarRendezvous::calculateWaitTime()
{
    // double initialPhaseAngle = m_orbitFrom.getStateVectors().position.angleBetween(m_orbitTo.getStateVectors().position);

    // assume circular orbits for now
    double initialPhaseAngle = m_orbitTo.isEquatorial() ?
            m_orbitTo.getElements().l - m_orbitFrom.getElements().l :
            m_orbitTo.getElements().Pi - m_orbitFrom.getElements().Pi;

    m_waitTime = (m_phaseAngle - initialPhaseAngle) / (m_tgtAngularV - m_intAngularV);
}

double CoplanarRendezvous::getTOF() const
{
    return m_TOF;
}

double CoplanarRendezvous::getInterceptorAngularVelocity() const
{
    return m_intAngularV;
}

double CoplanarRendezvous::getTargetAngularVelocity() const
{
    return m_tgtAngularV;
}

double CoplanarRendezvous::getLeadAngle() const
{
    return m_leadAngle;
}

double CoplanarRendezvous::getPhaseAngle() const
{
    return m_phaseAngle;
}

double CoplanarRendezvous::getWaitTime() const
{
    return m_waitTime;
}

EMSCRIPTEN_BINDINGS(coplanar_rendezvous_bindings)
{
    using namespace emscripten;

    class_<CoplanarRendezvous>("CoplanarRendezvous")
        .constructor<Orbit&, Orbit&>()
        .property("timeOfFlight", &CoplanarRendezvous::getTOF)
        .property("interceptorAngularVelocity", &CoplanarRendezvous::getInterceptorAngularVelocity)
        .property("targetAngularVelocity", &CoplanarRendezvous::getTargetAngularVelocity)
        .property("leadAngle", &CoplanarRendezvous::getLeadAngle)
        .property("phaseAngle", &CoplanarRendezvous::getPhaseAngle)
        .property("waitTime", &CoplanarRendezvous::getWaitTime);
}