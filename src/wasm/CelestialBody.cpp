#include "CelestialBody.hpp"

CelestialBody::CelestialBody(double mass, double orbitalRadius, double mu):
    m_mass{mass}, m_orbitalRadius{orbitalRadius}, m_mu{mu}
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