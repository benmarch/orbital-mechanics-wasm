#include <emscripten/bind.h>
#include <math.h>
#include "ThrustCalculator.hpp"
#include "constants.hpp"

double ThrustCalculator::calculateJetPowerFromMassFlowRateAndEffectiveExhaustVelocity(double massFlowRate,
                                                                                      double effectiveExhaustVelocity)
{
    return massFlowRate * pow(effectiveExhaustVelocity, 2) / 2;
}

double ThrustCalculator::calculateThrustFromMassFlowRateAndEffectiveExhaustVelocity(double massFlowRate,
                                                                                    double effectiveExhaustVelocity)
{
    return massFlowRate * effectiveExhaustVelocity;
}

double ThrustCalculator::calculateThrustFromDeltaMomentumAndDeltaTime(double deltaMomentum, double deltaTime)
{
    return deltaMomentum / deltaTime;
}

double ThrustCalculator::calculateImpulseFromThrustAndDeltaTime(double thrust, double deltaTime)
{
    return thrust * deltaTime;
}

double ThrustCalculator::calculateSpecificImpulseFromImpulseAndPropellantMass(double impulse, double propellantMass)
{
    return impulse / propellantMass / GRAVITY_AT_SL;
}

double ThrustCalculator::calculateSpecificImpulseFromThrustAndMassFlowRate(double thrust, double massFlowRate)
{
    return thrust / massFlowRate / GRAVITY_AT_SL;
}

double ThrustCalculator::calculateSpecificImpulseFromEffectiveExhaustVelocity(double effectiveExhaustVelocity)
{
    return effectiveExhaustVelocity / GRAVITY_AT_SL;
}

double ThrustCalculator::calculateDeltaVUsingEffectiveExhaustVelocity(double effectiveExhaustVelocity,
                                                                      double initialMass, double finalMass)
{
    return effectiveExhaustVelocity * log(initialMass / finalMass);
}

double ThrustCalculator::calculateDeltaVUsingSpecificImpulse(double specificImpulse, double initialMass,
                                                             double finalMass)
{
    return specificImpulse * GRAVITY_AT_SL * log(initialMass / finalMass);
}

double ThrustCalculator::calculatePropellantMassFromDeltaVAndEffectiveExhaustVelocityAndInitialMass(double deltaV,
                                                                                                    double effectiveExhaustVelocity,
                                                                                                    double initialMass)
{
    return initialMass - initialMass / exp(deltaV / effectiveExhaustVelocity);
}

EMSCRIPTEN_BINDINGS(thrust_calculator_bindings) {
    using namespace emscripten;

    class_<ThrustCalculator>("ThrustCalculator")
        .constructor()
        .function("calculateJetPowerFromMassFlowRateAndEffectiveExhaustVelocity", &ThrustCalculator::calculateJetPowerFromMassFlowRateAndEffectiveExhaustVelocity)
        .function("calculateThrustFromMassFlowRateAndEffectiveExhaustVelocity", &ThrustCalculator::calculateThrustFromMassFlowRateAndEffectiveExhaustVelocity)
        .function("calculateThrustFromDeltaMomentumAndDeltaTime", &ThrustCalculator::calculateThrustFromDeltaMomentumAndDeltaTime)
        .function("calculateImpulseFromThrustAndDeltaTime", &ThrustCalculator::calculateImpulseFromThrustAndDeltaTime)
        .function("calculateSpecificImpulseFromImpulseAndPropellantMass", &ThrustCalculator::calculateSpecificImpulseFromImpulseAndPropellantMass)
        .function("calculateSpecificImpulseFromThrustAndMassFlowRate", &ThrustCalculator::calculateSpecificImpulseFromThrustAndMassFlowRate)
        .function("calculateSpecificImpulseFromEffectiveExhaustVelocity", &ThrustCalculator::calculateSpecificImpulseFromEffectiveExhaustVelocity)
        .function("calculateDeltaVUsingEffectiveExhaustVelocity", &ThrustCalculator::calculateDeltaVUsingEffectiveExhaustVelocity)
        .function("calculateDeltaVUsingSpecificImpulse", &ThrustCalculator::calculateDeltaVUsingSpecificImpulse)
        .function("calculatePropellantMassFromDeltaVAndEffectiveExhaustVelocityAndInitialMass", &ThrustCalculator::calculatePropellantMassFromDeltaVAndEffectiveExhaustVelocityAndInitialMass);

}