#ifndef ORBITAL_MECHANICS_RENDEZVOUS_HPP
#define ORBITAL_MECHANICS_RENDEZVOUS_HPP

#include "Orbit.hpp"

class CoplanarRendezvous
{
public:
    CoplanarRendezvous(Orbit& orbitFrom, Orbit& orbitTo);

    double getTOF() const;
    double getInterceptorAngularVelocity() const;
    double getTargetAngularVelocity() const;
    double getLeadAngle() const;
    double getPhaseAngle() const;
    double getWaitTime() const;

private:
    void calculateTransferOrbit();
    void calculateTOF();
    void calculateInterceptorAngularVelocity();
    void calculateTargetAngularVelocity();
    void calculateLeadAngle();
    void calculatePhaseAngle();
    void calculateWaitTime();

    double calculateAngularVelocity(Orbit);

    Orbit m_orbitFrom;
    Orbit m_orbitTo;
    double m_a_t;
    double m_TOF;
    double m_intAngularV;
    double m_tgtAngularV;
    double m_leadAngle;
    double m_phaseAngle;
    double m_waitTime;
};


#endif //ORBITAL_MECHANICS_RENDEZVOUS_HPP
