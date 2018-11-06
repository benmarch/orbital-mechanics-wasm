#include <math.h>
#include <emscripten/bind.h>
#include "PatchedConicApproximation.hpp"
#include "HohmannTransfer.hpp"

PatchedConicApproximation::PatchedConicApproximation(const Orbit &originParkingOrbit, const Orbit &originPlanetOrbit,
                                                     const Orbit &targetParkingOrbit, const Orbit &targetPlanetOrbit):
    m_originParkingOrbit{originParkingOrbit},
    m_originPlanetOrbit{originPlanetOrbit},
    m_targetParkingOrbit{targetParkingOrbit},
    m_targetPlanetOrbit{targetPlanetOrbit}
{
    calculateTransferOrbit();
    calculateVInfOrigin();
    calculateVInfTarget();
    calculateDeltaVBoost();
    calculateDeltaVRetro();
    calculateTimeOfFlight();
}

void PatchedConicApproximation::calculateTransferOrbit()
{
    HohmannTransfer transfer = HohmannTransfer{m_originPlanetOrbit, m_targetPlanetOrbit};
    m_transferOrbit = transfer.getTransferOrbit();
}

void PatchedConicApproximation::calculateVInfOrigin()
{
    double vOriginPlanet = m_originPlanetOrbit.getVelocity();
    double trueLatitudeOfOrigin = m_originPlanetOrbit.getRadius() > m_targetPlanetOrbit.getRadius() ? M_PI : 0;
    double vTransferAtOrigin = m_transferOrbit.getVelocity(trueLatitudeOfOrigin);

    m_velocityInfOrigin = abs(vTransferAtOrigin - vOriginPlanet);
}

void PatchedConicApproximation::calculateVInfTarget()
{
    double vTargetPlanet = m_targetPlanetOrbit.getVelocity();
    double trueLatitudeOfTarget = m_originPlanetOrbit.getRadius() > m_targetPlanetOrbit.getRadius() ? 0 : M_PI;
    double vTransferAtTarget = m_transferOrbit.getVelocity(trueLatitudeOfTarget);

    m_velocityInfTarget = abs(vTransferAtTarget - vTargetPlanet);
}

void PatchedConicApproximation::calculateDeltaVBoost()
{
    double vOriginPark = m_originParkingOrbit.getVelocity();
    double epsHypOrigin = pow(m_velocityInfOrigin, 2) / 2;
    double vHyp = sqrt(2 * (m_originParkingOrbit.getMu() / m_originParkingOrbit.getRadius() + epsHypOrigin));

    m_deltaVBoost = abs(vHyp - vOriginPark);
}

void PatchedConicApproximation::calculateDeltaVRetro()
{
    double vTargetPark = m_targetParkingOrbit.getVelocity();
    double epsHypTarget = pow(m_velocityInfTarget, 2) / 2;
    double vHyp = sqrt(2 * (m_targetParkingOrbit.getMu() / m_targetParkingOrbit.getRadius() + epsHypTarget));

    m_deltaVRetro = abs(vHyp - vTargetPark);
}

void PatchedConicApproximation::calculateTimeOfFlight()
{
    m_timeOfFlight = M_PI * sqrt(pow(m_transferOrbit.getElements().a, 3) / m_transferOrbit.getMu());
}

Orbit PatchedConicApproximation::getTransferOrbit() const
{
    return m_transferOrbit;
}

double PatchedConicApproximation::getDeltaVBoost() const
{
    return m_deltaVBoost;
}

double PatchedConicApproximation::getDeltaVRetro() const
{
    return m_deltaVRetro;
}

double PatchedConicApproximation::getDeltaVMission() const
{
    return getDeltaVBoost() + getDeltaVRetro();
}

double PatchedConicApproximation::getVInfOrigin() const
{
    return m_velocityInfOrigin;
}

double PatchedConicApproximation::getVInfTarget() const
{
    return m_velocityInfTarget;
}

double PatchedConicApproximation::getTimeOfFlight() const
{
    return m_timeOfFlight;
}

EMSCRIPTEN_BINDINGS(patched_conic_approx_bindings)
{
    using namespace emscripten;

    class_<PatchedConicApproximation>("PatchedConicApproximation")
        .constructor<Orbit&, Orbit&, Orbit&, Orbit&>()
        .property("transferOrbit", &PatchedConicApproximation::getTransferOrbit)
        .property("deltaVMission", &PatchedConicApproximation::getDeltaVMission)
        .property("deltaVBoost", &PatchedConicApproximation::getDeltaVBoost)
        .property("deltaVRetro", &PatchedConicApproximation::getDeltaVRetro)
        .property("vInfOrigin", &PatchedConicApproximation::getVInfOrigin)
        .property("vInfTarget", &PatchedConicApproximation::getVInfTarget)
        .property("timeOfFlight", &PatchedConicApproximation::getTimeOfFlight);
}