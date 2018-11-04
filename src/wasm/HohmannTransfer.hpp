#ifndef ORBITAL_MECHANICS_HOHMANNTRANSFER_H
#define ORBITAL_MECHANICS_HOHMANNTRANSFER_H

#include "Orbit.hpp"

class HohmannTransfer
{
public:
    HohmannTransfer(Orbit &orbitFrom, Orbit &orbitTo);
    double getDeltaV1() const;
    double getDeltaV2() const;
    double getDeltaVTotal() const;
    double getTimeOfFlight() const;
    Orbit getTransferOrbit() const;

private:
    void calculateSemimajorAxisOfTransferOrbit();
    void calculateEnergyOfTransferOrbit();
    void calculateTimeOfFlight();
    void calculateDeltaV1();
    void calculateDeltaV2();


    Orbit m_orbitFrom;
    Orbit m_orbitTo;
    double m_a_t;
    double m_eps_t;
    double m_TOF;
    double m_deltaV1;
    double m_deltaV2;
};


#endif //ORBITAL_MECHANICS_HOHMANNTRANSFER_H
