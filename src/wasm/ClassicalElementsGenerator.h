#ifndef ORBITAL_MECHANICS_CLASSICALELEMENTSGENERATOR_H
#define ORBITAL_MECHANICS_CLASSICALELEMENTSGENERATOR_H

#include "Vector.h"

typedef struct {
    double eps; // total mechanical energy
    double a; // semi-major axis
    double e; // eccentricity
    Vector e_vec; // eccentricity
    Vector h_vec; // angular momentum
    double h; // angular momentum
    double i; // inclination
    Vector n_vec; // nodal vector
    double n; // nodal vector
    double o; // argument of perigee
    double Om; // right ascension of the ascending node
    double nu; // true anomaly
} ClassicalElements;

class ClassicalElementsGenerator {
public:
    ClassicalElements generateFromStateVectors(Vector &position, Vector &velocity);

private:
    ClassicalElements m_elements;
    Vector m_R_vec;
    Vector m_V_vec;
    double m_R;
    double m_V;

    void calculateTotalMechanicalEnergy();
    void calculateSemimajorAxis();
    void calculateEccentricityVector();
    void calculateAngularMomentum();
    void calculateInclination();
    void calculateNodalVector();
    void calculateRightAscensionOfTheAscendingNode();
    void calculateArgumentOfPerigee();
    void calculateTrueAnomaly();
};


#endif //ORBITAL_MECHANICS_CLASSICALELEMENTSGENERATOR_H
