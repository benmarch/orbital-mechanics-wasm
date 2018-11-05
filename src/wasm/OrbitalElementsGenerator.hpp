#ifndef ORBITAL_MECHANICS_ORBITALELEMENTSGENERATOR_H
#define ORBITAL_MECHANICS_ORBITALELEMENTSGENERATOR_H

#include "Vector.hpp"
#include "constants.hpp"

class OrbitalElementsGenerator
{
public:
    OrbitalElementsGenerator();
    OrbitalElementsGenerator(double mu);

    OrbitalElements generateFromStateVectors(Vector &position, Vector &velocity);

private:
    OrbitalElements m_elements;
    Vector m_radius;
    Vector m_velocity;
    double m_mu;

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
};


#endif //ORBITAL_MECHANICS_ORBITALELEMENTSGENERATOR_H
