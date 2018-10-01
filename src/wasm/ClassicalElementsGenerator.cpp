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
    m_elements.eps = pow(m_V, 2)/2 - MU/m_R;
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
    Vector scaledPosition = scale(m_R_vec, (1/MU) * (pow(m_V, 2) - MU/m_R));
    Vector scaledVelocity = scale(m_V_vec, (1/MU) * dot(m_R_vec, m_V_vec));

    m_elements.e_vec = subtract(scaledPosition, scaledVelocity);
    m_elements.e = magnitude(m_elements.e_vec);
}

void ClassicalElementsGenerator::calculateAngularMomentum()
{
    m_elements.h_vec = cross(m_R_vec, m_V_vec);
    m_elements.h = magnitude(m_elements.h_vec);
}

void ClassicalElementsGenerator::calculateInclination()
{
    m_elements.i = acos(m_elements.h_vec.c/m_elements.h);
}

void ClassicalElementsGenerator::calculateNodalVector()
{
    Vector k_hat = Vector{0, 0, 1};
    m_elements.n_vec = cross(k_hat, m_elements.h_vec);
    m_elements.n = magnitude(m_elements.n_vec);
}

void ClassicalElementsGenerator::calculateRightAscensionOfTheAscendingNode()
{
    double rightAscension = acos(m_elements.n_vec.a/m_elements.n);

    // quadrant check
    if (m_elements.n_vec.b > 0) {
        m_elements.Om = rightAscension;
    } else if (m_elements.n_vec.b < 0) {
        m_elements.Om = 2*M_PI - rightAscension;
    } else if (m_elements.n_vec.a > 0) {
        m_elements.Om = 0;
    } else {
        m_elements.Om = M_PI;
    }
}

void ClassicalElementsGenerator::calculateArgumentOfPerigee()
{
    double argumentOfPerigee = acos(dot(m_elements.n_vec, m_elements.e_vec) / m_elements.n / m_elements.e);

    // quadrant check
    if (m_elements.e_vec.c > 0) {
        m_elements.o = argumentOfPerigee;
    } else if (m_elements.e_vec.c < 0) {
        m_elements.o = 2*M_PI - argumentOfPerigee;
    } else {
        m_elements.o = argumentOfPerigee; // no?
    }
}

void ClassicalElementsGenerator::calculateTrueAnomaly()
{
    double trueAnomaly = acos(dot(m_R_vec, m_elements.e_vec) / m_R / m_elements.e);

    double r_dot_v = dot(m_R_vec, m_V_vec);

    // quadrant check
    if (r_dot_v > 0) {
        m_elements.nu = trueAnomaly;
    } else if (r_dot_v < 0) {
        m_elements.nu =  2*M_PI - trueAnomaly;
    } else if (m_elements.a * (1 - m_elements.e)) {
        m_elements.nu = 0;
    } else {
        m_elements.nu = M_PI;
    }
}

ClassicalElements ClassicalElementsGenerator::generateFromStateVectors(Vector &position, Vector &velocity)
{
    m_R_vec = position;
    m_V_vec = velocity;
    m_R = magnitude(position);
    m_V = magnitude(velocity);

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
                .field("eps", &ClassicalElements::eps)
                .field("e_vec", &ClassicalElements::e_vec)
                .field("e", &ClassicalElements::e)
                .field("h_vec", &ClassicalElements::h_vec)
                .field("h", &ClassicalElements::h)
                .field("i", &ClassicalElements::i)
                .field("n_vec", &ClassicalElements::n_vec)
                .field("o", &ClassicalElements::o)
                .field("Om", &ClassicalElements::Om)
                .field("nu", &ClassicalElements::nu)

        ;

        emscripten::class_<ClassicalElementsGenerator>("ClassicalElementsGenerator")
        .constructor()
        .function("generateFromStateVectors", &ClassicalElementsGenerator::generateFromStateVectors)
        ;
};
