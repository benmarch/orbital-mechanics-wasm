#ifndef ORBITAL_MECHANICS_PLANECHANGE_HPP
#define ORBITAL_MECHANICS_PLANECHANGE_HPP

#include "Orbit.hpp"

class PlaneChange
{
public:
    PlaneChange(const Orbit &orbitFrom, const Orbit &orbitTo);

    void setPlaneChangeAngle(const double theta);

    double getDeltaV() const;

private:
    void calculateDeltaV();

    Orbit m_orbitFrom;
    Orbit m_orbitTo;
    double m_theta{0};
    double m_deltaV{0};
};

#endif //ORBITAL_MECHANICS_PLANECHANGE_HPP
