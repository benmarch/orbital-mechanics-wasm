#include "CelestialBody.hpp"

CelestialBody::CelestialBody(double mass, double orbitalRadius, double mu, double radius, double j2):
    m_mass{mass}, m_orbitalRadius{orbitalRadius}, m_mu{mu}, m_radius{radius}, m_j2{j2}
{

}

double CelestialBody::getMass() const
{
    return m_mass;
}

double CelestialBody::getOrbitalRadius() const
{
    return m_orbitalRadius;
}

double CelestialBody::getMu() const
{
    return m_mu;
}

double CelestialBody::getRadius() const
{
    return m_radius;
}

double CelestialBody::getJ2() const
{
    return m_j2;
}