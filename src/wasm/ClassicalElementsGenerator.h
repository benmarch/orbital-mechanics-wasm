#ifndef ORBITAL_MECHANICS_CLASSICALELEMENTSGENERATOR_H
#define ORBITAL_MECHANICS_CLASSICALELEMENTSGENERATOR_H

#include "Vector.h"

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
} ClassicalElements;

class ClassicalElementsGenerator
{
public:
    ClassicalElements generateFromStateVectors(Vector &position, Vector &velocity);

private:
    ClassicalElements m_elements;
    Vector m_radius;
    Vector m_velocity;

    void calculateTotalMechanicalEnergy();
    void calculateSemimajorAxis();
    void calculateEccentricityVector();
    void calculateAngularMomentum();
    void calculateInclination();
    void calculateNodalVector();
    void calculateRightAscensionOfTheAscendingNode();
    void calculateArgumentOfPerigee();
    void calculateTrueAnomaly();
    void calculateArgumentOfLatitude();
    void calculateLongitudeOfPerigee();
    void calculateTrueLongitude();
    bool isEquatorialOrbit();
    bool isCircularOrbit();
    bool isOpenOrbit();
};


#endif //ORBITAL_MECHANICS_CLASSICALELEMENTSGENERATOR_H
