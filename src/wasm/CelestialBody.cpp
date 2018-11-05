#include "CelestialBody.hpp"

CelestialBody::CelestialBody(double mass, double radius, double mu):
    m_mass{mass}, m_radius{radius}, m_mu{mu}
{

}

double CelestialBody::getMass() const
{
    return m_mass;
}

double CelestialBody::getRadius() const
{
    return m_radius;
}

double CelestialBody::getMu() const
{
    return m_mu;
}