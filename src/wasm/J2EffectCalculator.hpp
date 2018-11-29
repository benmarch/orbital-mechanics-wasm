#ifndef ORBITAL_MECHANICS_J2EFFECTCALCULATOR_HPP
#define ORBITAL_MECHANICS_J2EFFECTCALCULATOR_HPP

#include "Orbit.hpp"

class J2EffectCalculator
{
public:
    J2EffectCalculator(const Orbit &orbit);

    double getArgumentOfPerigeeRateOfChange() const;
    double getRightAscensionRateOfChange() const;
    double getMeanAnomalyRateOfChange() const;

private:
    void calculateSemiLatticeRectum();
    void calculateMeanMotion();
    void calculateArgumentOfPerigeeRateOfChange();
    void calculateRightAscensionRateOfChange();
    void calculateMeanAnomalyRateOfChange();

    Orbit m_orbit;
    double m_semiLatticeRectum;
    double m_meanMotion;
    double m_argumentOfPerigeeRateOfChange;
    double m_rightAscensionRateOfChange;
    double m_meanAnomalyRateOfChange;
};


#endif //ORBITAL_MECHANICS_J2EFFECTCALCULATOR_HPP
