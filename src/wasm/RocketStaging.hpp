#ifndef ORBITAL_MECHANICS_ROCKETSTAGING_HPP
#define ORBITAL_MECHANICS_ROCKETSTAGING_HPP

typedef struct RocketStage {
    double specificImpulse;
    double structureMass;
    double propellantMass;
    double initialMass; // mass of rocket before this stage burns
    double finalMass; // mass of rocket after this stage burns
    double deltaV; // this stage's contribution to the rocket's ∆V
} RocketStage;

typedef struct Rocket {
    double payloadMass;
    double totalMass;
    double deltaV;
    std::vector<RocketStage*> stages;
    RocketStage stage1; // cant get arrays or vectors to work with emscripten
    RocketStage stage2;
    RocketStage stage3;
    RocketStage stage4;
    RocketStage stage5;
} Rocket;

class RocketStaging
{
public:
    Rocket calculateTotalDeltaV(Rocket &rocket);

private:
    double calculateTotalMass(Rocket &rocket);
};


#endif //ORBITAL_MECHANICS_ROCKETSTAGING_HPP
