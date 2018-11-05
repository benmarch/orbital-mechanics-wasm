#ifndef ORBITAL_MECHANICS_COORBITALRENDEZVOUS_HPP
#define ORBITAL_MECHANICS_COORBITALRENDEZVOUS_HPP

#include "Orbit.hpp"

class CoorbitalRendezvous
{
public:
    CoorbitalRendezvous(Orbit &orbitFrom, Orbit &orbitTo);

    double getPhaseAngle() const;
    double getTOF() const;
    Orbit getPhasingOrbit() const;
    double getAngularVelocity() const;

private:
    void calculateInitialPhaseAngle();
    void calculateTravelPhaseAngle();
    void calculateTargetAngularVelocity();
    void calculateTOF();
    void calculatePhasingOrbit();

    Orbit m_orbitFrom;
    Orbit m_orbitTo;
    Orbit m_orbitPhasing;
    double m_initialPhaseAngle;
    double m_travelPhaseAngle;
    double m_tgtAngularV;
    double m_tof;
};


#endif //ORBITAL_MECHANICS_COORBITALRENDEZVOUS_HPP
