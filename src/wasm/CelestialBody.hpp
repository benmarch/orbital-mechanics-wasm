#ifndef ORBITAL_MECHANICS_CELESTIALBODY_HPP
#define ORBITAL_MECHANICS_CELESTIALBODY_HPP


class CelestialBody
{
public:
    CelestialBody(double mass, double orbitalRadius, double mu, double radius, double j2);

    double getMass() const;
    double getOrbitalRadius() const;
    double getMu() const;
    double getRadius() const;
    double getJ2() const;

private:
    double m_mass;
    double m_orbitalRadius;
    double m_mu;
    double m_radius;
    double m_j2;
};


#endif //ORBITAL_MECHANICS_CELESTIALBODY_HPP
