#include <math.h>
#include <emscripten/bind.h>
#include <iostream>

#include "StateVectorGenerator.h"
#include "mutil.h"
#include "orbitutil.h"

StateVectors StateVectorGenerator::generateFromOrbitalElements(OrbitalElements elements)
{
    m_elements = elements;

    // adjust for circular and equatorial orbits
    if (isCircularOrbit(elements) && !isEquatorialOrbit(elements)) {
        m_elements.o = 0;
        m_elements.nu = m_elements.u;
    } else if (isEquatorialOrbit(elements) && !isCircularOrbit(elements) && !isOpenOrbit(elements)) {
        m_elements.Om = 0;
        m_elements.o = m_elements.Pi;
    } else if (isEquatorialOrbit(elements) && isCircularOrbit(elements)) {
        m_elements.Om = 0;
        m_elements.o = 0;
        m_elements.nu = m_elements.l;
    }

    determineRotationMatrix();
    calculatePosition();
    calculateVelocity();

    return m_state_vectors;
}

void StateVectorGenerator::determineRotationMatrix()
{
    const double &O = m_elements.Om;
    const double &o = m_elements.o;
    const double &i = m_elements.i;

    Vector row1 = Vector {
            cos(O) * cos(o) - sin(O) * sin(o) * cos(i),
            -1 * cos(O) * sin(o) - sin(O) * cos(o) * cos(i),
            sin(O) * sin(i)
    };

    Vector row2 = Vector {
            sin(O) * cos(o) + cos(O) * sin(o) * cos(i),
            -1 * sin(O) * sin(o) + cos(O) * cos(o) * cos(i),
            -1 * cos(O) * sin(i)
    };

    Vector row3 = Vector {
            sin(o) * sin(i),
            cos(o) * sin(i),
            cos(i)
    };

    m_rotation_matrix = RotationMatrix { row1, row2, row3 };
}

Vector StateVectorGenerator::rotateToIJK(Vector &vec)
{
    double i = m_rotation_matrix.row1.dot(vec);
    double j = m_rotation_matrix.row2.dot(vec);
    double k = m_rotation_matrix.row3.dot(vec);

    return Vector { i, j, k };
}

void StateVectorGenerator::calculatePosition()
{
    double radius = (m_elements.a * (1 - pow(m_elements.e.getMagnitude(), 2))) / (1 + m_elements.e.getMagnitude() * cos(m_elements.nu));
    Vector positionPQW = Vector { radius * cos(m_elements.nu), radius * sin(m_elements.nu), 0 };

    m_state_vectors.position = rotateToIJK(positionPQW);
}

void StateVectorGenerator::calculateVelocity()
{
    double speed = sqrt(MU/(m_elements.a * (1 - pow(m_elements.e.getMagnitude(), 2))));
    Vector velocityPQW = Vector { speed * sin(m_elements.nu) * -1, speed * (m_elements.e.getMagnitude() + cos(m_elements.nu)), 0 };

    m_state_vectors.velocity = rotateToIJK(velocityPQW);
}

