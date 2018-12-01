#include <emscripten/bind.h>
#include <math.h>
#include "Reentry.hpp"
#include "constants.hpp"

void Reentry::calculateFromReentryConditions(double reentryVelocity, double flightpathAngle, double ballisticCoefficient)
{
    m_reentryVelocity = reentryVelocity;
    m_flightpathAngle = flightpathAngle;
    m_ballisticCoefficient = ballisticCoefficient;

    calculateMaxDeceleration();
    calculateAltitudeAtMaxDeceleration();
    calculateAltitudeAtMaxHeatingRate();
    calculateVelocityAtMaxHeatingRate();
}

double Reentry::getMaxDeceleration() const
{
    return m_maxDeceleration;
}

double Reentry::getAltitudeMaxDeceleration() const
{
    return m_altitudeMaxDeceleration;
}

double Reentry::getAltitudeMaxHeating() const
{
    return m_altitudeMaxHeating;
}

double Reentry::getVelocityMaxHeating() const
{
    return m_velocityMaxHeating;
}


void Reentry::calculateMaxDeceleration()
{
    const double betaKm = ATMOSPHERIC_SCALE_HEIGHT * 1000;

    m_maxDeceleration = pow(m_reentryVelocity, 2) * betaKm * sin(m_flightpathAngle) / 2 / exp(1);
}

void Reentry::calculateAltitudeAtMaxDeceleration()
{
    const double betaKm = ATMOSPHERIC_SCALE_HEIGHT * 1000;

    m_altitudeMaxDeceleration = log(ATMOSPHERIC_DENSITY_AT_SL / m_ballisticCoefficient / ATMOSPHERIC_SCALE_HEIGHT / sin(m_flightpathAngle)) / betaKm;
}

void Reentry::calculateAltitudeAtMaxHeatingRate()
{
    const double betaKm = ATMOSPHERIC_SCALE_HEIGHT * 1000;

    m_altitudeMaxHeating = log(ATMOSPHERIC_DENSITY_AT_SL / 3 / m_ballisticCoefficient / ATMOSPHERIC_SCALE_HEIGHT / sin(m_flightpathAngle)) / betaKm;
}

void Reentry::calculateVelocityAtMaxHeatingRate()
{
    m_velocityMaxHeating = 0.846 * m_reentryVelocity;
}

EMSCRIPTEN_BINDINGS(reentry_bindings) {
    using namespace emscripten;

    class_<Reentry>("Reentry")
        .constructor()
        .function("calculateFromReentryConditions", &Reentry::calculateFromReentryConditions)
        .property("maxDeceleration", &Reentry::getMaxDeceleration)
        .property("altitudeMaxDeceleration", &Reentry::getAltitudeMaxDeceleration)
        .property("altitudeMaxHeating", &Reentry::getAltitudeMaxHeating)
        .property("velocityMaxHeating", &Reentry::getVelocityMaxHeating);
}