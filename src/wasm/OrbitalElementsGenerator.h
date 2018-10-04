#ifndef ORBITAL_MECHANICS_ORBITALELEMENTSGENERATOR_H
#define ORBITAL_MECHANICS_ORBITALELEMENTSGENERATOR_H

#include "Vector.h"
#include "constants.h"

class OrbitalElementsGenerator
{
public:
    OrbitalElements generateFromStateVectors(Vector &position, Vector &velocity);

private:
    OrbitalElements m_elements;
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


#endif //ORBITAL_MECHANICS_ORBITALELEMENTSGENERATOR_H
