#ifndef ORBITAL_MECHANICS_CELESTIALBODY_HPP
#define ORBITAL_MECHANICS_CELESTIALBODY_HPP


class CelestialBody
{
public:
    CelestialBody(double mass, double orbitalRadius, double mu);

    double getMass() const;
    double getOrbitalRadius() const;
    double getMu() const;

private:
    double m_mass;
    double m_orbitalRadius;
    double m_mu;
};


#endif //ORBITAL_MECHANICS_CELESTIALBODY_HPP
