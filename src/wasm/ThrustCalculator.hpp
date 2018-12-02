#ifndef ORBITAL_MECHANICS_THRUSTCALCULATOR_HPP
#define ORBITAL_MECHANICS_THRUSTCALCULATOR_HPP


class ThrustCalculator
{
public:
    double calculateJetPowerFromMassFlowRateAndEffectiveExhaustVelocity(double massFlowRate, double effectiveExhaustVelocity);

    double calculateThrustFromMassFlowRateAndEffectiveExhaustVelocity(double massFlowRate, double effectiveExhaustVelocity);
    double calculateThrustFromDeltaMomentumAndDeltaTime(double deltaMomentum, double deltaTime);

    double calculateImpulseFromThrustAndDeltaTime(double thrust, double deltaTime);

    double calculateSpecificImpulseFromImpulseAndPropellantMass(double impulse, double propellantMass);
    double calculateSpecificImpulseFromThrustAndMassFlowRate(double thrust, double massFlowRate);
    double calculateSpecificImpulseFromEffectiveExhaustVelocity(double effectiveExhaustVelocity);

    double calculateDeltaVUsingEffectiveExhaustVelocity(double effectiveExhaustVelocity, double initialMass, double finalMass);
    double calculateDeltaVUsingSpecificImpulse(double specificImpulse, double initialMass, double finalMass);

    double calculatePropellantMassFromDeltaVAndEffectiveExhaustVelocityAndInitialMass(double deltaV, double effectiveExhaustVelocity, double initialMass);

};


#endif //ORBITAL_MECHANICS_THRUSTCALCULATOR_HPP
