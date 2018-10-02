#include <math.h>
#include <emscripten/bind.h>
#include <iostream>

#include "ClassicalElementsGenerator.h"
#include "constants.h"

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
    Vector k_hat = Vector{0, 0, 1};
    m_elements.n = k_hat.cross(m_elements.h);
}

void ClassicalElementsGenerator::calculateRightAscensionOfTheAscendingNode()
{
    double rightAscension = acos(m_elements.n.getX()/m_elements.n.getMagnitude());

    // quadrant check
    if (m_elements.n.getY() > 0) {
        m_elements.Om = rightAscension;
    } else if (m_elements.n.getY() < 0) {
        m_elements.Om = 2*M_PI - rightAscension;
    } else if (m_elements.n.getX() > 0) {
        m_elements.Om = 0;
    } else {
        m_elements.Om = M_PI;
    }
}

void ClassicalElementsGenerator::calculateArgumentOfPerigee()
{
    double argumentOfPerigee = acos(m_elements.n.dot(m_elements.e) / m_elements.n.getMagnitude() / m_elements.e.getMagnitude());

    // quadrant check
    if (m_elements.e.getZ() > 0) {
        m_elements.o = argumentOfPerigee;
    } else if (m_elements.e.getZ() < 0) {
        m_elements.o = 2*M_PI - argumentOfPerigee;
    } else {
        m_elements.o = argumentOfPerigee; // no?
    }
}

void ClassicalElementsGenerator::calculateTrueAnomaly()
{
    double trueAnomaly = acos(m_radius.dot(m_elements.e) / m_radius.getMagnitude() / m_elements.e.getMagnitude());

    double r_dot_v = m_radius.dot(m_velocity);

    // quadrant check
    if (r_dot_v > 0) {
        m_elements.nu = trueAnomaly;
    } else if (r_dot_v < 0) {
        m_elements.nu =  2*M_PI - trueAnomaly;
    } else if (m_elements.a * (1 - m_elements.e.getMagnitude())) {
        m_elements.nu = 0;
    } else {
        m_elements.nu = M_PI;
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

        ;

        emscripten::class_<ClassicalElementsGenerator>("ClassicalElementsGenerator")
        .constructor()
        .function("generateFromStateVectors", &ClassicalElementsGenerator::generateFromStateVectors)
        ;
};
