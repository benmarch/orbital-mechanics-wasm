#ifndef ORBITAL_MECHANICS_PATCHEDCONICAPPROXIMATION_HPP
#define ORBITAL_MECHANICS_PATCHEDCONICAPPROXIMATION_HPP

#include "Orbit.hpp"

class PatchedConicApproximation
{
public:
    PatchedConicApproximation(
        const Orbit &originParkingOrbit,
        const Orbit &originPlanetOrbit,
        const Orbit &targetParkingOrbit,
        const Orbit &targetPlanetOrbit
    );

    Orbit getTransferOrbit() const;
    double getDeltaVBoost() const;
    double getDeltaVRetro() const;
    double getDeltaVMission() const;
    double getVInfOrigin() const;
    double getVInfTarget() const;
    double getTimeOfFlight() const;

private:
    void calculateTransferOrbit();
    void calculateVInfOrigin();
    void calculateVInfTarget();
    void calculateDeltaVBoost();
    void calculateDeltaVRetro();
    void calculateTimeOfFlight();

    Orbit m_originParkingOrbit;
    Orbit m_originPlanetOrbit;
    Orbit m_targetParkingOrbit;
    Orbit m_targetPlanetOrbit;
    Orbit m_transferOrbit;

    double m_deltaVBoost;
    double m_deltaVRetro;
    double m_velocityInfOrigin;
    double m_velocityInfTarget;
    double m_timeOfFlight;
};


#endif //ORBITAL_MECHANICS_PATCHEDCONICAPPROXIMATION_HPP
