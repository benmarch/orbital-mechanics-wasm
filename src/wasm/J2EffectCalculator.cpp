#include <emscripten/bind.h>
#include <math.h>
#include "J2EffectCalculator.hpp"
#include "constants.hpp"

J2EffectCalculator::J2EffectCalculator(const Orbit &orbit):
    m_orbit{orbit}
{
    calculateMeanMotion();
    calculateSemiLatticeRectum();
    calculateArgumentOfPerigeeRateOfChange();
    calculateRightAscensionRateOfChange();
    calculateMeanAnomalyRateOfChange();
}

double J2EffectCalculator::getArgumentOfPerigeeRateOfChange() const
{
    return m_argumentOfPerigeeRateOfChange;
}

double J2EffectCalculator::getRightAscensionRateOfChange() const
{
    return m_rightAscensionRateOfChange;
}

double J2EffectCalculator::getMeanAnomalyRateOfChange() const
{
    return m_meanAnomalyRateOfChange;
}

void J2EffectCalculator::calculateMeanMotion()
{
    m_meanMotion = sqrt(m_orbit.getMu()/pow(m_orbit.getElements().a, 3));
}

void J2EffectCalculator::calculateSemiLatticeRectum()
{
    m_semiLatticeRectum = m_orbit.getElements().a * (1 - pow(m_orbit.getElements().e.getMagnitude(), 2));
}

void J2EffectCalculator::calculateMeanAnomalyRateOfChange()
{
    const double i = m_orbit.getElements().i;
    const double e = m_orbit.getElements().e.getMagnitude();
    const double n = m_meanMotion;
    const double p = m_semiLatticeRectum;
    const double r = EARTH.getRadius();
    const double j = EARTH.getJ2();

    m_meanAnomalyRateOfChange = 3 * n * pow(r, 2) * j * sqrt(1 - pow(e, 2)) * (2 - 3 * pow(sin(i), 2)) / (4 * pow(p, 2));
}

void J2EffectCalculator::calculateArgumentOfPerigeeRateOfChange()
{
    const double i = m_orbit.getElements().i;
    const double n = m_meanMotion;
    const double p = m_semiLatticeRectum;
    const double r = EARTH.getRadius();
    const double j = EARTH.getJ2();

    m_argumentOfPerigeeRateOfChange = 2 * n * pow(r, 2) * j * (4 - 5 * pow(sin(i), 2)) / (4 * pow(p, 2));
}

void J2EffectCalculator::calculateRightAscensionRateOfChange()
{
    const double i = m_orbit.getElements().i;
    const double n = m_meanMotion;
    const double p = m_semiLatticeRectum;
    const double r = EARTH.getRadius();
    const double j = EARTH.getJ2();

    m_rightAscensionRateOfChange = -1 * 3 * n * pow(r, 2) * j * cos(i) / (2 * pow(p, 2));
}

EMSCRIPTEN_BINDINGS(j2_effect_calculator_bindings) {
    using namespace emscripten;

    class_<J2EffectCalculator>("J2EffectCalculator")
        .constructor<Orbit&>()
        .property("argumentOfPerigeeRateOfChange", &J2EffectCalculator::getArgumentOfPerigeeRateOfChange)
        .property("rightAscensionRateOfChange", &J2EffectCalculator::getRightAscensionRateOfChange)
        .property("meanAnomalyRateOfChange", &J2EffectCalculator::getMeanAnomalyRateOfChange);
}