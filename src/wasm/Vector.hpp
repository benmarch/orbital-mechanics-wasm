#ifndef ORBITAL_MECHANICS_VECTOR_H
#define ORBITAL_MECHANICS_VECTOR_H

#include <iostream>

class Vector
{
public:
    Vector(double x=0, double y=0, double z=0);
    Vector(const Vector &vec);

    double dot(const Vector &vec2);
    Vector cross(const Vector &vec2);
    Vector normalize();
    double angleBetween(const Vector &vec2);

    Vector operator*(double scale);
    Vector operator+(const Vector &vec2);
    Vector operator-(const Vector &vec2);
    bool operator==(const Vector &vec2);
    friend std::ostream& operator<<(std::ostream &out, const Vector &vec);

    double getX() const;
    double getY() const;
    double getZ() const;
    double getMagnitude() const;

    void setX(double x);
    void setY(double y);
    void setZ(double z);

private:
    void setMagnitude();

    double m_x;
    double m_y;
    double m_z;
    double mag;
};

std::ostream& operator<<(std::ostream &out, Vector &vec);

#endif //ORBITAL_MECHANICS_VECTOR_H
