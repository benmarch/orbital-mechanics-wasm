#ifndef ORBITAL_MECHANICS_CONSTANTS_H
#define ORBITAL_MECHANICS_CONSTANTS_H

#include "Vector.hpp"
#include "CelestialBody.hpp"

const double PRECISION = 0.00000000001;

const CelestialBody SUN = CelestialBody{1.9891e30, 0, 132712400000, 0, 0};
const CelestialBody EARTH = CelestialBody{5.9737e24, 1.496e8, 398600, 6378, 1.08263E-3};
const CelestialBody VENUS = CelestialBody{4.867e24, 1.081e8, 325700, 0, 0};
const CelestialBody SATURN = CelestialBody{5.685e26, 1.43345e9, 3.7967e7, 0, 0};

const double ATMOSPHERIC_DENSITY_AT_SL = 1.225; // kg/m^3
const double ATMOSPHERIC_SCALE_HEIGHT = 0.000139; // m^-1
const double GRAVITY_AT_SL = 9.81; // m/s^2

typedef struct OrbitalElements {
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

typedef struct StateVectors {
    Vector position;
    Vector velocity;
} StateVectors;

typedef struct RotationMatrix {
    Vector row1;
    Vector row2;
    Vector row3;
} RotationMatrix;

typedef struct LaunchSite {
    double lst;
    double lat;
    double alt;
} LaunchSite;

typedef struct LaunchWindow {
    double alpha;
    double beta;
    double gamma;
    double delta;
    double lwst;
    double waitTime;
    bool isNext{false};
} LaunchWindow;

#endif //ORBITAL_MECHANICS_CONSTANTS_H
