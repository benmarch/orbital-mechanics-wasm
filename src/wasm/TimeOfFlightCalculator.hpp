#ifndef ORBITAL_MECHANICS_TIMEOFFLIGHTCALCULATOR_HPP
#define ORBITAL_MECHANICS_TIMEOFFLIGHTCALCULATOR_HPP

class Orbit;

typedef struct TimeOfFlightElements {
    double meanMotion;
    double initialEccentricAnomaly;
    double finalEccentricAnomaly;
    double initialMeanAnomaly;
    double finalMeanAnomaly;
    double initialTrueAnomaly;
    double finalTrueAnomaly;
    double timeOfFlight;
} TimeOfFlightElements;

class TimeOfFlightCalculator
{
public:
    TimeOfFlightCalculator(Orbit &orbit);

    TimeOfFlightElements calculateTimeOfFlight(const double initialTrueAnomaly, const double finalTrueAnomaly);
    TimeOfFlightElements calculateFinalTrueAnomaly(const double initialTrueAnomaly, const double timeOfFlight);
    TimeOfFlightElements calculateInitialTrueAnomaly(const double finalTrueAnomaly, const double timeOfFlight);

private:
    double calculateMeanMotion();
    double calculateEccentricAnomalyFromTrueAnomaly(double trueAnomaly);
    double calculateEccentricAnomalyFromMeanAnomaly(double meanAnomaly);
    double calculateMeanAnomalyFromEccentricAnomaly(double eccentricAnomaly);
    double calculateTrueAnomalyFromEccentricAnomaly(double eccentricAnomaly);
    double solveForTimeOfFlight(const TimeOfFlightElements &timeOfFlightElements);
    double solveForInitialMeanAnomaly(const TimeOfFlightElements &timeOfFlightElements);
    double solveForFinalMeanAnomaly(const TimeOfFlightElements &timeOfFlightElements);

    const Orbit& m_orbit;
};


#endif //ORBITAL_MECHANICS_TIMEOFFLIGHTCALCULATOR_HPP
