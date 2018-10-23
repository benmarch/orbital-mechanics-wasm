#include <emscripten/bind.h>
#include <math.h>
#include "HohmannTransfer.hpp"
#include "orbitutil.hpp"
#include "constants.hpp"

HohmannTransfer::HohmannTransfer(Orbit &orbitFrom, Orbit &orbitTo)
{
    if (!isCircularOrbit(orbitFrom.getElements()) && !isCircularOrbit(orbitTo.getElements())) {
        throw "Both orbits must be circular to perform a Hohmann Transfer";
    };

    m_orbitFrom = orbitFrom;
    m_orbitTo = orbitTo;

    calculateSemimajorAxisOfTransferOrbit();
    calculateEnergyOfTransferOrbit();
    calculateTimeOfFlight();
    calculateDeltaV1();
    calculateDeltaV2();
}

void HohmannTransfer::calculateSemimajorAxisOfTransferOrbit()
{
    m_a_t = (m_orbitFrom.getElements().a + m_orbitTo.getElements().a)/2;
}

void HohmannTransfer::calculateEnergyOfTransferOrbit()
{
    m_eps_t = -1 * MU / (2 * m_a_t);
}

void HohmannTransfer::calculateTimeOfFlight()
{
    m_TOF = M_PI * sqrt(pow(m_a_t, 3) / MU);
}

void HohmannTransfer::calculateDeltaV1()
{
    double transferVelocityAtOrbitFrom = sqrt(2* (MU/m_orbitFrom.getStateVectors().position.getMagnitude() + m_eps_t));

    m_deltaV1 = abs(transferVelocityAtOrbitFrom - m_orbitFrom.getStateVectors().velocity.getMagnitude());
}

void HohmannTransfer::calculateDeltaV2()
{
    double transferVelocityAtOrbitTo = sqrt(2* (MU/m_orbitTo.getStateVectors().position.getMagnitude() + m_eps_t));

    m_deltaV2 = abs(transferVelocityAtOrbitTo - m_orbitTo.getStateVectors().velocity.getMagnitude());
}

double HohmannTransfer::getDeltaV1() const
{
    return m_deltaV1;
}

double HohmannTransfer::getDeltaV2() const
{
    return m_deltaV2;
}

double HohmannTransfer::getDeltaVTotal() const
{
    return m_deltaV1 + m_deltaV2;
}

double HohmannTransfer::getTimeOfFlight() const
{
    return m_TOF;
}

EMSCRIPTEN_BINDINGS(hohmann_transfer_bindings) {
        using namespace emscripten;

        class_<HohmannTransfer>("HohmannTransfer")
        .constructor<Orbit&, Orbit&>()
        .property("deltaV1", &HohmannTransfer::getDeltaV1)
        .property("deltaV2", &HohmannTransfer::getDeltaV2)
        .property("deltaVTotal", &HohmannTransfer::getDeltaVTotal)
        .property("timeOfFlight", &HohmannTransfer::getTimeOfFlight);
}