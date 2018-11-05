#ifndef ORBITAL_MECHANICS_ORBIT_H
#define ORBITAL_MECHANICS_ORBIT_H

#include "Vector.hpp"
#include "constants.hpp"
#include "CelestialBody.hpp"
#include "OrbitalElementsGenerator.hpp"
#include "StateVectorGenerator.hpp"

class Orbit
{
public:
    Orbit();
    Orbit(CelestialBody orbitingBody);
    Orbit(double mu);
    ~Orbit();

    void updateFromOrbitalElements(OrbitalElements elements);
    void updateFromStateVectors(StateVectors &stateVectors);
    OrbitalElements getElements() const;
    StateVectors getStateVectors() const;

    double getVelocity() const;
    double getRadius() const;
    double getVelocity(double nu) const;
    double getRadius(double nu) const;

    bool isCircular() const;
    bool isOpen() const;
    bool isEquatorial() const;

    double getMu() const;

private:
    OrbitalElementsGenerator *m_orbitalElementsGenerator;
    StateVectorGenerator *m_stateVectorGenerator;
    OrbitalElements m_elements;
    StateVectors m_stateVectors;
    double m_mu;
};


#endif //ORBITAL_MECHANICS_ORBIT_H
