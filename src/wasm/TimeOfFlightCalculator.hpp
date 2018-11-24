#ifndef ORBITAL_MECHANICS_TIMEOFFLIGHTCALCULATOR_HPP
#define ORBITAL_MECHANICS_TIMEOFFLIGHTCALCULATOR_HPP

#include "Orbit.hpp"

class TimeOfFlightCalculator
{
public:
    TimeOfFlightCalculator(Orbit &orbit);

    double calculateTimeOfFlight(const double initialTrueAnomaly, const double finalTrueAnomaly);
    double calculateFinalTrueAnomaly(const double initialTrueAnomaly, const double timeOfFlight);
    double calculateInitialTrueAnomaly(const double finalTrueAnomaly, const double timeOfFlight);

    double getMeanMotion() const;
    double getInitialEccentricAnomaly() const;
    double getFinalEccentricAnomaly() const;
    double getInitialMeanAnomaly() const;
    double getFinalMeanAnomaly() const;
    double getTimeOfFlight() const;
    double getInitialTrueAnomaly() const;
    double getFinalTrueAnomaly() const;

private:
    double calculateMeanMotion();
    double calculateEccentricAnomalyFromTrueAnomaly(double trueAnomaly);
    double calculateEccentricAnomalyFromMeanAnomaly(double meanAnomaly);
    double calculateMeanAnomalyFromEccentricAnomaly(double eccentricAnomaly);
    double calculateTrueAnomalyFromEccentricAnomaly(double eccentricAnomaly);
    double solveForTimeOfFlight();
    double solveForInitialMeanAnomaly();
    double solveForFinalMeanAnomaly();

    const Orbit& m_orbit;
    double m_meanMotion;
    double m_initialEccentricAnomaly;
    double m_finalEccentricAnomaly;
    double m_initialMeanAnomaly;
    double m_finalMeanAnomaly;
    double m_initialTrueAnomaly;
    double m_finalTrueAnomaly;
    double m_timeOfFlight;
};


#endif //ORBITAL_MECHANICS_TIMEOFFLIGHTCALCULATOR_HPP
