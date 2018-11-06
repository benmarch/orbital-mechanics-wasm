#include <emscripten/bind.h>
#include <math.h>
#include <memory>
#include "HohmannTransfer.hpp"
#include "orbitutil.hpp"
#include "constants.hpp"

HohmannTransfer::HohmannTransfer(Orbit &orbitFrom, Orbit &orbitTo)
{
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
    m_a_t = (m_orbitFrom.getRadius() + m_orbitTo.getRadius())/2;
}

void HohmannTransfer::calculateEnergyOfTransferOrbit()
{
    m_eps_t = -1 * m_orbitFrom.getMu() / (2 * m_a_t);
}

void HohmannTransfer::calculateTimeOfFlight()
{
    m_TOF = M_PI * sqrt(pow(m_a_t, 3) / m_orbitFrom.getMu());
}

void HohmannTransfer::calculateDeltaV1()
{
    double transferVelocityAtOrbitFrom = sqrt(2* (m_orbitFrom.getMu()/m_orbitFrom.getRadius() + m_eps_t));

    m_deltaV1 = abs(transferVelocityAtOrbitFrom - m_orbitFrom.getVelocity());
}

void HohmannTransfer::calculateDeltaV2()
{
    double transferVelocityAtOrbitTo = sqrt(2* (m_orbitFrom.getMu()/m_orbitTo.getRadius() + m_eps_t));

    m_deltaV2 = abs(transferVelocityAtOrbitTo - m_orbitTo.getVelocity());
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

Orbit HohmannTransfer::getTransferOrbit() const
{
    Orbit transferOrbit = Orbit{m_orbitFrom.getMu()};
    Orbit innerOrbit = m_orbitFrom.getRadius() < m_orbitTo.getRadius() ? m_orbitFrom : m_orbitTo;

    OrbitalElements elements = innerOrbit.getElements();

    elements.a = m_a_t;
    elements.e = Vector{-1 * (innerOrbit.getRadius() / m_a_t - 1), 0, 0};

    transferOrbit.updateFromOrbitalElements(elements);
    StateVectors stateVectors = transferOrbit.getStateVectors();
    transferOrbit.updateFromStateVectors(stateVectors);

    return transferOrbit;
}

EMSCRIPTEN_BINDINGS(hohmann_transfer_bindings) {
        using namespace emscripten;

        class_<HohmannTransfer>("HohmannTransfer")
        .constructor<Orbit&, Orbit&>()
        .property("deltaV1", &HohmannTransfer::getDeltaV1)
        .property("deltaV2", &HohmannTransfer::getDeltaV2)
        .property("deltaVTotal", &HohmannTransfer::getDeltaVTotal)
        .property("timeOfFlight", &HohmannTransfer::getTimeOfFlight)
        .property("transferOrbit", &HohmannTransfer::getTransferOrbit);
}