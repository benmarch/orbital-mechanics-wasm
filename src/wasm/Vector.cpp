#include <math.h>
#include <emscripten/bind.h>
#include <iostream>

#include "Vector.hpp"
#include "mutil.hpp"

// default constructor
Vector::Vector(double x, double y, double z):
    m_x{x}, m_y{y}, m_z{z}
{
    setMagnitude();
}

// copy constructor
Vector::Vector(const Vector &vec):
    Vector{vec.getX(), vec.getY(), vec.getZ()}
{

}

double Vector::getX() const
{
    return m_x;
}

double Vector::getY() const
{
    return m_y;
}

double Vector::getZ() const
{
    return m_z;
}

void Vector::setX(double x)
{
    m_x = x;
    setMagnitude();
}

void Vector::setY(double y)
{
    m_y = y;
    setMagnitude();
}

void Vector::setZ(double z)
{
    m_z = z;
    setMagnitude();
}

double Vector::getMagnitude() const
{
    return mag;
}

void Vector::setMagnitude()
{
    mag = sqrt(pow(getX(), 2) + pow(getY(), 2) + pow(getZ(), 2));
}

Vector Vector::operator*(double scale)
{
    return Vector{getX() * scale, getY() * scale, getZ() * scale};
}

Vector Vector::operator+(const Vector &vec2)
{
    return Vector{getX() + vec2.getX(), getY() + vec2.getY(), getZ() + vec2.getZ()};
}

Vector Vector::operator-(const Vector &vec2)
{
    return Vector{getX() - vec2.getX(), getY() - vec2.getY(), getZ() - vec2.getZ()};
}

bool Vector::operator==(const Vector &vec2)
{
    return withinPrecision(getX(), vec2.getX()) &&
            withinPrecision(getY(), vec2.getY()) &&
            withinPrecision(getZ(), vec2.getZ());
}

std::ostream& operator<<(std::ostream &out, Vector &vec)
{
    out << "Vector{" << vec.getX() << ", " << vec.getY() << ", " << vec.getZ() << "}(" << vec.getMagnitude() << ")";
    return out;
}

double Vector::dot(const Vector &vec2)
{
    return getX() * vec2.getX() + getY() * vec2.getY() + getZ() * vec2.getZ();
}

Vector Vector::cross(const Vector &vec2)
{
    return Vector{
        (getY() * vec2.getZ()) - (getZ() * vec2.getY()),
        (getZ() * vec2.getX()) - (getX() * vec2.getZ()),
        (getX() * vec2.getY()) - (getY() * vec2.getX())
    };
}

Vector Vector::normalize()
{
    return *this * (1/mag);
}

double Vector::angleBetween(const Vector &vec2)
{
    double magnitude = mag * vec2.getMagnitude();
    return acos(dot(vec2) / magnitude);
}

EMSCRIPTEN_BINDINGS(vector_bindings) {
        emscripten::value_array<Vector>("Vector")
                .element(&Vector::getX, &Vector::setX)
                .element(&Vector::getY, &Vector::setY)
                .element(&Vector::getZ, &Vector::setZ)
                ;
};
