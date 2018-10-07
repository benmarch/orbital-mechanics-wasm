#include <math.h>
#include <emscripten/bind.h>
#include <iostream>

#include "OrbitalElementsGenerator.hpp"
#include "orbitutil.hpp"
#include "mutil.hpp"

using namespace std;

/**
 * Calculates epsilon (specific mechanical energy)
 */
void OrbitalElementsGenerator::calculateTotalMechanicalEnergy()
{
    m_elements.eps = pow(m_velocity.getMagnitude(), 2)/2 - MU/m_radius.getMagnitude();
}

/**
 * Calculates a (semi-major axis)
 */
void OrbitalElementsGenerator::calculateSemimajorAxis()
{
    m_elements.a = -1*MU/2/m_elements.eps;
}

void OrbitalElementsGenerator::calculateEccentricityVector()
{
    Vector scaledPosition = m_radius * ((1 / MU) * (pow(m_velocity.getMagnitude(), 2) - MU / m_radius.getMagnitude()));
    Vector scaledVelocity = m_velocity * ((1 / MU) * m_radius.dot(m_velocity));

    m_elements.e = scaledPosition - scaledVelocity;
}

void OrbitalElementsGenerator::calculateAngularMomentum()
{
    m_elements.h = m_radius.cross(m_velocity);
}

void OrbitalElementsGenerator::calculateInclination()
{
    m_elements.i = acos(m_elements.h.getZ()/m_elements.h.getMagnitude());
}

void OrbitalElementsGenerator::calculateNodalVector()
{
    // there is no ascending node vector for equatorial orbits
    if (isEquatorialOrbit(m_elements)) {
        m_elements.n = NULL;
        return;
    }

    Vector k_hat = Vector{0, 0, 1};
    m_elements.n = k_hat.cross(m_elements.h);
}

void OrbitalElementsGenerator::calculateRightAscensionOfTheAscendingNode()
{
    // there is no ascending node for equatorial orbits
    if (isEquatorialOrbit(m_elements)) {
        m_elements.Om = NULL;
        return;
    }

    double rightAscension = acos(m_elements.n.getX()/m_elements.n.getMagnitude());
    Vector n = fixError(m_elements.n);

    // quadrant check
    if (n.getY() > 0) {
        m_elements.Om = rightAscension;
    } else if (n.getY() < 0) {
        m_elements.Om = 2*M_PI - rightAscension;
    } else if (n.getX() > 0) {
        m_elements.Om = 0;
    } else {
        m_elements.Om = M_PI;
    }
}

void OrbitalElementsGenerator::calculateArgumentOfPerigee()
{
    // there is no perigee for circular or open orbits
    if (isEquatorialOrbit(m_elements) || isOpenOrbit(m_elements)) {
        m_elements.o = NULL;
        return;
    }

    double argumentOfPerigee = acos(m_elements.n.dot(m_elements.e) / m_elements.n.getMagnitude() / m_elements.e.getMagnitude());
    double ek = fixError(m_elements.e.getZ());

    Vector n = m_elements.n.normalize();
    Vector e = m_elements.e.normalize();

    // quadrant check
    if (ek > 0) {
        m_elements.o = argumentOfPerigee;
    } else if (ek < 0) {
        m_elements.o = 2*M_PI - argumentOfPerigee;
    } else if (withinPrecision(n, e)) {
        m_elements.o = 0;
    } else {
        m_elements.o = M_PI;
    }
}

void OrbitalElementsGenerator::calculateTrueAnomaly()
{
    // there is no perigee for circular or open orbits
    if (isCircularOrbit(m_elements) || isOpenOrbit(m_elements)) {
        m_elements.nu = NULL;
        return;
    }

    double trueAnomaly = acos(m_radius.dot(m_elements.e) / m_radius.getMagnitude() / m_elements.e.getMagnitude());
    double phi = fixError(m_radius.dot(m_velocity));

    // quadrant check
    if (phi > 0) {
        m_elements.nu = trueAnomaly;
    } else if (phi < 0) {
        m_elements.nu =  2*M_PI - trueAnomaly;
    } else if (withinPrecision(m_radius.getMagnitude(), m_elements.a * (1 - m_elements.e.getMagnitude()))) {
        m_elements.nu = 0;
    } else {
        m_elements.nu = M_PI;
    }
}

void OrbitalElementsGenerator::calculateArgumentOfLatitude()
{
    // there is no ascending node for equatorial orbits
    if (isEquatorialOrbit(m_elements)) {
        m_elements.u = NULL;
        return;
    }

    double argumentOfLatitude = acos(m_radius.dot(m_elements.n) / m_radius.getMagnitude() / m_elements.n.getMagnitude());
    Vector r = fixError(m_radius);
    Vector n = fixError(m_elements.n);

    // quadrant check
    if (r.getZ() > 0) {
        m_elements.u = argumentOfLatitude;
    } else if (r.getZ() < 0) {
        m_elements.u =  2*M_PI - argumentOfLatitude;
    } else if ((r + n).getMagnitude() >= r.getMagnitude() + n.getMagnitude()) {
        // pointing in the same direction
        m_elements.u = 0;
    } else {
        m_elements.u = M_PI;
    }
}

void OrbitalElementsGenerator::calculateLongitudeOfPerigee()
{
    // there is no perigee for circular or open orbits
    if (isCircularOrbit(m_elements) || isOpenOrbit(m_elements)) {
        m_elements.Pi = NULL;
        return;
    }

    double longitudeOfPerigee = acos(m_elements.e.getX() / m_elements.e.getMagnitude());
    Vector e = fixError(m_elements.e);

    // quadrant check
    if (e.getY() > 0) {
        m_elements.Pi = longitudeOfPerigee;
    } else if (e.getY() < 0) {
        m_elements.Pi = 2*M_PI - longitudeOfPerigee;
    } else if (e.getX() > 0) {
        m_elements.Pi = 0;
    } else {
        m_elements.Pi = M_PI;
    }
}

void OrbitalElementsGenerator::calculateTrueLongitude()
{
    double trueLongitude = acos(m_radius.getX() / m_radius.getMagnitude());
    Vector r = fixError(m_radius);

    // quadrant check
    if (r.getY() > 0) {
        m_elements.l = trueLongitude;
    } else if (r.getY() < 0) {
        m_elements.l = 2*M_PI - trueLongitude;
    } else if (r.getX() > 0) {
        m_elements.l = 0;
    } else {
        m_elements.l = M_PI;
    }
}

OrbitalElements OrbitalElementsGenerator::generateFromStateVectors(Vector &position, Vector &velocity)
{
    m_radius = position;
    m_velocity = velocity;

    calculateTotalMechanicalEnergy();
    calculateSemimajorAxis();
    calculateEccentricityVector();
    calculateAngularMomentum();
    calculateInclination();
    calculateNodalVector();
    calculateArgumentOfPerigee();
    calculateRightAscensionOfTheAscendingNode();
    calculateTrueAnomaly();
    calculateArgumentOfLatitude();
    calculateLongitudeOfPerigee();
    calculateTrueLongitude();

    return m_elements;
}