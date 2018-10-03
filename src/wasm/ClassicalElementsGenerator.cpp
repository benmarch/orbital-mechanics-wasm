#include <math.h>
#include <emscripten/bind.h>
#include <iostream>

#include "ClassicalElementsGenerator.h"
#include "constants.h"
#include "mutil.h"

using namespace std;

/**
 * Calculates epsilon (specific mechanical energy)
 */
void ClassicalElementsGenerator::calculateTotalMechanicalEnergy()
{
    m_elements.eps = pow(m_velocity.getMagnitude(), 2)/2 - MU/m_radius.getMagnitude();
}

/**
 * Calculates a (semi-major axis)
 */
void ClassicalElementsGenerator::calculateSemimajorAxis()
{
    m_elements.a = -1*MU/2/m_elements.eps;
}

void ClassicalElementsGenerator::calculateEccentricityVector()
{
    Vector scaledPosition = m_radius * ((1 / MU) * (pow(m_velocity.getMagnitude(), 2) - MU / m_radius.getMagnitude()));
    Vector scaledVelocity = m_velocity * ((1 / MU) * m_radius.dot(m_velocity));

    m_elements.e = scaledPosition - scaledVelocity;
}

void ClassicalElementsGenerator::calculateAngularMomentum()
{
    m_elements.h = m_radius.cross(m_velocity);
}

void ClassicalElementsGenerator::calculateInclination()
{
    m_elements.i = acos(m_elements.h.getZ()/m_elements.h.getMagnitude());
}

void ClassicalElementsGenerator::calculateNodalVector()
{
    // there is no ascending node vector for equatorial orbits
    if (m_elements.i == 0) {
        m_elements.n = NULL;
        return;
    }

    Vector k_hat = Vector{0, 0, 1};
    m_elements.n = k_hat.cross(m_elements.h);
}

void ClassicalElementsGenerator::calculateRightAscensionOfTheAscendingNode()
{
    // there is no ascending node for equatorial orbits
    if (m_elements.i == 0) {
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

void ClassicalElementsGenerator::calculateArgumentOfPerigee()
{
    // there is no perigee for circular or open orbits
    if (m_elements.e.getMagnitude() == 0 || m_elements.e.getMagnitude() >= 1) {
        return;
    }

    double argumentOfPerigee = fixError(acos(m_elements.n.dot(m_elements.e) / m_elements.n.getMagnitude() / m_elements.e.getMagnitude()));
    double ek = fixError(m_elements.e.getZ());

    // quadrant check
    if (ek > 0) {
        m_elements.o = argumentOfPerigee;
    } else if (ek < 0) {
        m_elements.o = 2*M_PI - argumentOfPerigee;
    } else if (fixError(m_elements.n.normalize()) == fixError(m_elements.e.normalize())) {
        m_elements.o = 0;
    } else {
        m_elements.o = M_PI;
    }
}

void ClassicalElementsGenerator::calculateTrueAnomaly()
{
    // there is no perigee for circular or open orbits
    if (m_elements.e.getMagnitude() == 0 || m_elements.e.getMagnitude() >= 1) {
        return;
    }

    double trueAnomaly = acos(m_radius.dot(m_elements.e) / m_radius.getMagnitude() / m_elements.e.getMagnitude());
    double phi = fixError(m_radius.dot(m_velocity));

    // quadrant check
    if (phi > 0) {
        m_elements.nu = trueAnomaly;
    } else if (phi < 0) {
        m_elements.nu =  2*M_PI - trueAnomaly;
    } else if (m_radius.getMagnitude() == m_elements.a * (1 - m_elements.e.getMagnitude())) {
        m_elements.nu = 0;
    } else {
        m_elements.nu = M_PI;
    }
}

void ClassicalElementsGenerator::calculateArgumentOfLatitude()
{
    // there is no ascending node for equatorial orbits
    if (m_elements.i == 0) {
        return;
    }

    // if both Argument of Perigee and True Anomaly are defined, this is the sum
    if (m_elements.o != NULL && m_elements.nu != NULL) {
        m_elements.u = m_elements.o + m_elements.nu;
        return;
    }

    double argumentOfLatitude = acos(m_radius.dot(m_elements.n) / m_radius.getMagnitude() / m_elements.n.getMagnitude());
    Vector r = fixError(m_radius);
    Vector n = fixError(m_elements.n);

    // quadrant check
    if (r.getY() > 0) {
        m_elements.u = argumentOfLatitude;
    } else if (r.getY() < 0) {
        m_elements.u =  2*M_PI - argumentOfLatitude;
    } else if ((r + n).getMagnitude() >= r.getMagnitude() + n.getMagnitude()) {
        // pointing in the same direction
        m_elements.u = 0;
    } else {
        m_elements.u = M_PI;
    }
}

void ClassicalElementsGenerator::calculateLongitudeOfPerigee()
{
    // there is no perigee for circular or open orbits
    if (m_elements.e.getMagnitude() == 0 || m_elements.e.getMagnitude() >= 1) {
        return;
    }

    // if both Right Ascention of the Ascending Node and Argument of Perigee are defined, this is the sum
    if (m_elements.Om != NULL && m_elements.o != NULL) {
        m_elements.Pi = m_elements.Om + m_elements.o;
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

void ClassicalElementsGenerator::calculateTrueLongitude()
{
    // if both True Anomaly and Longitude of Perigee are defined, this is the sum
    if (m_elements.nu != NULL && m_elements.Pi != NULL) {
        m_elements.l = m_elements.nu + m_elements.Pi;
        return;
    }

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

ClassicalElements ClassicalElementsGenerator::generateFromStateVectors(Vector &position, Vector &velocity)
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


EMSCRIPTEN_BINDINGS(elements_bindings) {
    emscripten::value_object<ClassicalElements>("ClassicalElements")
        .field("a", &ClassicalElements::a)
        .field("e", &ClassicalElements::e)
        .field("i", &ClassicalElements::i)
        .field("o", &ClassicalElements::o)
        .field("Om", &ClassicalElements::Om)
        .field("nu", &ClassicalElements::nu)
        .field("u", &ClassicalElements::u)
        .field("Pi", &ClassicalElements::Pi)
        .field("l", &ClassicalElements::l)
    ;

    emscripten::class_<ClassicalElementsGenerator>("ClassicalElementsGenerator")
        .constructor()
        .function("generateFromStateVectors", &ClassicalElementsGenerator::generateFromStateVectors)
    ;
};
