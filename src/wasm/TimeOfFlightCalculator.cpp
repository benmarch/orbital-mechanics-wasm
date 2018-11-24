#include <math.h>
#include <emscripten/bind.h>
#include "TimeOfFlightCalculator.hpp"
#include "Orbit.hpp"
#include "mutil.hpp"

TimeOfFlightCalculator::TimeOfFlightCalculator(Orbit &orbit):
    m_orbit{orbit}
{

}

TimeOfFlightElements TimeOfFlightCalculator::calculateTimeOfFlight(const double initialTrueAnomaly, const double finalTrueAnomaly)
{
    TimeOfFlightElements timeOfFlightElements = {
        .initialTrueAnomaly = initialTrueAnomaly,
        .finalTrueAnomaly = finalTrueAnomaly,
        .meanMotion = calculateMeanMotion()
    };

    timeOfFlightElements.initialEccentricAnomaly = calculateEccentricAnomalyFromTrueAnomaly(initialTrueAnomaly);
    timeOfFlightElements.initialMeanAnomaly = calculateMeanAnomalyFromEccentricAnomaly(timeOfFlightElements.initialEccentricAnomaly);
    timeOfFlightElements.finalEccentricAnomaly = calculateEccentricAnomalyFromTrueAnomaly(finalTrueAnomaly);
    timeOfFlightElements.finalMeanAnomaly = calculateMeanAnomalyFromEccentricAnomaly(timeOfFlightElements.finalEccentricAnomaly);
    timeOfFlightElements.timeOfFlight = solveForTimeOfFlight(timeOfFlightElements);

    return timeOfFlightElements;
}

TimeOfFlightElements TimeOfFlightCalculator::calculateFinalTrueAnomaly(const double initialTrueAnomaly,
                                                                       const double timeOfFlight)
{
    TimeOfFlightElements timeOfFlightElements = {
        .initialTrueAnomaly = initialTrueAnomaly,
        .timeOfFlight = timeOfFlight,
        .meanMotion = calculateMeanMotion()
    };

    timeOfFlightElements.initialEccentricAnomaly = calculateEccentricAnomalyFromTrueAnomaly(initialTrueAnomaly);
    timeOfFlightElements.initialMeanAnomaly = calculateMeanAnomalyFromEccentricAnomaly(timeOfFlightElements.initialEccentricAnomaly);
    timeOfFlightElements.finalMeanAnomaly = solveForFinalMeanAnomaly(timeOfFlightElements);
    timeOfFlightElements.finalEccentricAnomaly = calculateEccentricAnomalyFromMeanAnomaly(timeOfFlightElements.finalMeanAnomaly);
    timeOfFlightElements.finalTrueAnomaly = calculateTrueAnomalyFromEccentricAnomaly(timeOfFlightElements.finalEccentricAnomaly);

    return timeOfFlightElements;
}

TimeOfFlightElements TimeOfFlightCalculator::calculateInitialTrueAnomaly(const double finalTrueAnomaly,
                                                                         const double timeOfFlight)
{
    TimeOfFlightElements timeOfFlightElements = {
        .finalTrueAnomaly = finalTrueAnomaly,
        .timeOfFlight = timeOfFlight,
        .meanMotion = calculateMeanMotion()
    };

    timeOfFlightElements.finalEccentricAnomaly = calculateEccentricAnomalyFromTrueAnomaly(finalTrueAnomaly);
    timeOfFlightElements.finalMeanAnomaly = calculateMeanAnomalyFromEccentricAnomaly(timeOfFlightElements.finalEccentricAnomaly);
    timeOfFlightElements.initialMeanAnomaly = solveForInitialMeanAnomaly(timeOfFlightElements);
    timeOfFlightElements.initialEccentricAnomaly = calculateEccentricAnomalyFromMeanAnomaly(timeOfFlightElements.initialMeanAnomaly);
    timeOfFlightElements.initialTrueAnomaly = calculateTrueAnomalyFromEccentricAnomaly(timeOfFlightElements.initialEccentricAnomaly);

    return timeOfFlightElements;
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

double TimeOfFlightCalculator::solveForTimeOfFlight(const TimeOfFlightElements &timeOfFlightElements)
{
    double timeOfFlight = (timeOfFlightElements.finalMeanAnomaly - timeOfFlightElements.initialMeanAnomaly) / timeOfFlightElements.meanMotion;

    /*while (timeOfFlight >= 2 * M_PI) {
        timeOfFlight -= 2 * M_PI;
    }*/

    return timeOfFlight;
}

double TimeOfFlightCalculator::solveForFinalMeanAnomaly(const TimeOfFlightElements &timeOfFlightElements)
{
    return timeOfFlightElements.meanMotion * timeOfFlightElements.timeOfFlight + timeOfFlightElements.initialMeanAnomaly;
}

double TimeOfFlightCalculator::solveForInitialMeanAnomaly(const TimeOfFlightElements &timeOfFlightElements)
{
    return -1 *(timeOfFlightElements.meanMotion * timeOfFlightElements.timeOfFlight - timeOfFlightElements.finalMeanAnomaly);
}

EMSCRIPTEN_BINDINGS(time_of_flight_bindings) {
    using namespace emscripten;

    class_<TimeOfFlightCalculator>("TimeOfFlightCalculator")
        .constructor<Orbit&>()
        .function("calculateTimeOfFlight", &TimeOfFlightCalculator::calculateTimeOfFlight)
        .function("calculateFinalTrueAnomaly", &TimeOfFlightCalculator::calculateFinalTrueAnomaly)
        .function("calculateInitialTrueAnomaly", &TimeOfFlightCalculator::calculateInitialTrueAnomaly);

    value_object<TimeOfFlightElements>("TimeOfFlightElements")
        .field("nu_i", &TimeOfFlightElements::initialTrueAnomaly)
        .field("nu_f", &TimeOfFlightElements::finalTrueAnomaly)
        .field("E_i", &TimeOfFlightElements::initialEccentricAnomaly)
        .field("E_f", &TimeOfFlightElements::finalEccentricAnomaly)
        .field("M_i", &TimeOfFlightElements::initialMeanAnomaly)
        .field("M_f", &TimeOfFlightElements::finalMeanAnomaly)
        .field("tof", &TimeOfFlightElements::timeOfFlight)
        .field("n", &TimeOfFlightElements::meanMotion);
}