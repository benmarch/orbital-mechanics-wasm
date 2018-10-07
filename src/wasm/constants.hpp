#ifndef ORBITAL_MECHANICS_CONSTANTS_H
#define ORBITAL_MECHANICS_CONSTANTS_H

#include "Vector.hpp"

const double MU = 398600;
const double PRECISION = 0.00000000001;

typedef struct {
    double eps; // total mechanical energy
    double a; // semi-major axis
    Vector e; // eccentricity
    Vector h; // angular momentum
    double i; // inclination
    Vector n; // nodal vector
    double o; // argument of perigee
    double Om; // right ascension of the ascending node
    double nu; // true anomaly
    double u; // argument of latitude
    double Pi; // longitude of perigee
    double l; // true longitude
} OrbitalElements;

typedef struct {
    Vector position;
    Vector velocity;
} StateVectors;

typedef struct {
    Vector row1;
    Vector row2;
    Vector row3;
} RotationMatrix;

#endif //ORBITAL_MECHANICS_CONSTANTS_H
