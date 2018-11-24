#include <math.h>
#include <emscripten/bind.h>
#include "TimeOfFlightCalculator.hpp"
#include "Orbit.hpp"
#include "mutil.hpp"

TimeOfFlightCalculator::TimeOfFlightCalculator(Orbit &orbit):
    m_orbit{orbit}
{

}

double TimeOfFlightCalculator::calculateTimeOfFlight(const double initialTrueAnomaly, const double finalTrueAnomaly)
{
    m_initialTrueAnomaly = initialTrueAnomaly;
    m_finalTrueAnomaly = finalTrueAnomaly;
    m_meanMotion = calculateMeanMotion();
    m_initialEccentricAnomaly = calculateEccentricAnomalyFromTrueAnomaly(initialTrueAnomaly);
    m_initialMeanAnomaly = calculateMeanAnomalyFromEccentricAnomaly(m_initialEccentricAnomaly);
    m_finalEccentricAnomaly = calculateEccentricAnomalyFromTrueAnomaly(finalTrueAnomaly);
    m_finalMeanAnomaly = calculateMeanAnomalyFromEccentricAnomaly(m_finalEccentricAnomaly);
    m_timeOfFlight = solveForTimeOfFlight();

    return m_timeOfFlight;
}

double TimeOfFlightCalculator::calculateFinalTrueAnomaly(const double initialTrueAnomaly, const double timeOfFlight)
{
    m_initialTrueAnomaly = initialTrueAnomaly;
    m_timeOfFlight = timeOfFlight;
    m_meanMotion = calculateMeanMotion();
    m_initialEccentricAnomaly = calculateEccentricAnomalyFromTrueAnomaly(initialTrueAnomaly);
    m_initialMeanAnomaly = calculateMeanAnomalyFromEccentricAnomaly(m_initialEccentricAnomaly);
    m_finalMeanAnomaly = solveForFinalMeanAnomaly();
    m_finalEccentricAnomaly = calculateEccentricAnomalyFromMeanAnomaly(m_finalMeanAnomaly);
    m_finalTrueAnomaly = calculateTrueAnomalyFromEccentricAnomaly(m_finalEccentricAnomaly);

    return m_finalTrueAnomaly;
}

double TimeOfFlightCalculator::calculateInitialTrueAnomaly(const double finalTrueAnomaly, const double timeOfFlight)
{

    m_finalTrueAnomaly = finalTrueAnomaly;
    m_timeOfFlight = timeOfFlight;
    m_meanMotion = calculateMeanMotion();
    m_finalEccentricAnomaly = calculateEccentricAnomalyFromTrueAnomaly(finalTrueAnomaly);
    m_finalMeanAnomaly = calculateMeanAnomalyFromEccentricAnomaly(m_finalEccentricAnomaly);
    m_initialMeanAnomaly = solveForInitialMeanAnomaly();
    m_initialEccentricAnomaly = calculateEccentricAnomalyFromMeanAnomaly(m_initialMeanAnomaly);
    m_initialTrueAnomaly = calculateTrueAnomalyFromEccentricAnomaly(m_initialEccentricAnomaly);

    return m_initialTrueAnomaly;
}

double TimeOfFlightCalculator::getMeanMotion() const
{
    return m_meanMotion;
}

double TimeOfFlightCalculator::getInitialEccentricAnomaly() const{
    return m_initialEccentricAnomaly;
}

double TimeOfFlightCalculator::getFinalEccentricAnomaly() const{
    return m_finalEccentricAnomaly;
}

double TimeOfFlightCalculator::getInitialMeanAnomaly() const{
    return m_initialMeanAnomaly;
}

double TimeOfFlightCalculator::getFinalMeanAnomaly() const{
    return m_finalMeanAnomaly;
}

double TimeOfFlightCalculator::getTimeOfFlight() const{
    return m_timeOfFlight;
}

double TimeOfFlightCalculator::getInitialTrueAnomaly() const{
    return m_initialTrueAnomaly;
}

double TimeOfFlightCalculator::getFinalTrueAnomaly() const{
    return m_finalTrueAnomaly;
}

double TimeOfFlightCalculator::calculateMeanMotion()
{
    return sqrt(m_orbit.getMu() / pow(m_orbit.getElements().a, 3));
}

double TimeOfFlightCalculator::calculateEccentricAnomalyFromTrueAnomaly(double trueAnomaly)
{
    const double e = m_orbit.getElements().e.getMagnitude();

    const double eccentricAnomaly = acos((e + cos(trueAnomaly)) / (1 + e * cos(trueAnomaly)));

    // quadrant check
    if (withinPrecision(eccentricAnomaly, 0)) {
        return 0;
    }

    if (withinPrecision(eccentricAnomaly, M_PI)) {
        return M_PI;
    }

    if (trueAnomaly < M_PI) {
        return eccentricAnomaly;
    }

    return 2 * M_PI - eccentricAnomaly;
}

double TimeOfFlightCalculator::calculateEccentricAnomalyFromMeanAnomaly(double meanAnomaly)
{
    const double e = m_orbit.getElements().e.getMagnitude();
    double previous = meanAnomaly;
    double next;
    int count = 0;

    do {
        next = meanAnomaly + e * sin(previous);
        count += 1;
    } while (!withinPrecision(next, previous) && count < 10);

    return next;
}

double TimeOfFlightCalculator::calculateMeanAnomalyFromEccentricAnomaly(double eccentricAnomaly)
{
    const double e = m_orbit.getElements().e.getMagnitude();

    return eccentricAnomaly - e * sin(eccentricAnomaly);
}

double TimeOfFlightCalculator::calculateTrueAnomalyFromEccentricAnomaly(double eccentricAnomaly)
{
    const double e = m_orbit.getElements().e.getMagnitude();

    const double trueAnomaly = acos((cos(eccentricAnomaly) - e) / (1 - e * cos(eccentricAnomaly)));

    // quadrant check
    if (withinPrecision(trueAnomaly, 0)) {
        return 0;
    }

    if (withinPrecision(trueAnomaly, M_PI)) {
        return M_PI;
    }

    if (eccentricAnomaly < M_PI) {
        return trueAnomaly;
    }

    return 2 * M_PI - trueAnomaly;
}

double TimeOfFlightCalculator::solveForTimeOfFlight()
{
    double timeOfFlight = (m_finalMeanAnomaly - m_initialMeanAnomaly) / m_meanMotion;

    /*while (timeOfFlight >= 2 * M_PI) {
        timeOfFlight -= 2 * M_PI;
    }*/

    return timeOfFlight;
}

double TimeOfFlightCalculator::solveForFinalMeanAnomaly()
{
    return m_meanMotion * m_timeOfFlight + m_initialMeanAnomaly;
}

double TimeOfFlightCalculator::solveForInitialMeanAnomaly()
{
    return m_finalMeanAnomaly - m_meanMotion * m_timeOfFlight;
}

EMSCRIPTEN_BINDINGS(time_of_flight_bindings) {
    using namespace emscripten;

    class_<TimeOfFlightCalculator>("TimeOfFlightCalculator")
        .constructor<Orbit&>()
        .function("calculateTimeOfFlight", &TimeOfFlightCalculator::calculateTimeOfFlight)
        .function("calculateFinalTrueAnomaly", &TimeOfFlightCalculator::calculateFinalTrueAnomaly)
        .function("calculateInitialTrueAnomaly", &TimeOfFlightCalculator::calculateInitialTrueAnomaly)
        .property("initialTrueAnomaly", &TimeOfFlightCalculator::getInitialTrueAnomaly)
        .property("finalTrueAnomaly", &TimeOfFlightCalculator::getFinalTrueAnomaly)
        .property("initialEccentricAnomaly", &TimeOfFlightCalculator::getInitialEccentricAnomaly)
        .property("finalEccentricAnomaly", &TimeOfFlightCalculator::getFinalEccentricAnomaly)
        .property("initialMeanAnomaly", &TimeOfFlightCalculator::getInitialMeanAnomaly)
        .property("finalMeanAnomaly", &TimeOfFlightCalculator::getFinalMeanAnomaly)
        .property("timeOfFlight", &TimeOfFlightCalculator::getTimeOfFlight)
        .property("meanMotion", &TimeOfFlightCalculator::getMeanMotion);
}