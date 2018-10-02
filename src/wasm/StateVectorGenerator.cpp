#include <math.h>
#include <emscripten/bind.h>
#include <iostream>

#include "StateVectorGenerator.h"
#include "constants.h"

StateVectors StateVectorGenerator::generateFromClassicalElements(ClassicalElements elements)
{
    m_elements = elements;

    determineRotationMatrix();
    calculatePosition();
    calculateVelocity();

    return m_state_vectors;
}

void StateVectorGenerator::determineRotationMatrix()
{
    double O = m_elements.Om;
    double o = m_elements.o;
    double i = m_elements.i;

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
    double i = m_rotation_matrix.row1.getX() * vec.getX() + m_rotation_matrix.row1.getY() * vec.getY() + m_rotation_matrix.row1.getZ() * vec.getZ();
    double j = m_rotation_matrix.row2.getX() * vec.getX() + m_rotation_matrix.row2.getY() * vec.getY() + m_rotation_matrix.row2.getZ() * vec.getZ();
    double k = m_rotation_matrix.row3.getX() * vec.getX() + m_rotation_matrix.row3.getY() * vec.getY() + m_rotation_matrix.row3.getZ() * vec.getZ();

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

EMSCRIPTEN_BINDINGS(state_vector_bindings) {
    emscripten::value_object<StateVectors>("StateVectors")
        .field("position", &StateVectors::position)
        .field("velocity", &StateVectors::velocity)
        ;

    emscripten::class_<StateVectorGenerator>("StateVectorGenerator")
        .constructor()
        .function("generateFromClassicalElements", &StateVectorGenerator::generateFromClassicalElements)
        ;
};
