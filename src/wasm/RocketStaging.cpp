#include <emscripten/bind.h>
#include <iostream>
#include "RocketStaging.hpp"
#include "ThrustCalculator.hpp"

Rocket RocketStaging::calculateTotalDeltaV(Rocket &rocket)
{
    ThrustCalculator thrustCalculator = ThrustCalculator{};
    double totalDeltaV{0};

    // determine the stages
    for (RocketStage *stage : {&rocket.stage1, &rocket.stage2, &rocket.stage3, &rocket.stage4, &rocket.stage5}) {
        if (stage->specificImpulse) {
            rocket.stages.push_back(stage);
        }
    }

    std::cout << rocket.stages.size() << std::endl;

    // get total mass of the rocket
    rocket.totalMass = calculateTotalMass(rocket);

    // assign initial and final masses to each stage, and calculate ∆V
    double stageMass = rocket.totalMass;
    for (auto i = 0; i < rocket.stages.size(); i++) {
        auto stage = rocket.stages.at(i);
        stage->initialMass = stageMass;
        stageMass -= stage->propellantMass;
        stage->finalMass = stageMass;

        stage->deltaV = thrustCalculator.calculateDeltaVUsingSpecificImpulse(stage->specificImpulse, stage->initialMass, stage->finalMass);
        totalDeltaV += stage->deltaV;

        stageMass -= stage->structureMass;
    }

    // assign total ∆V
    rocket.deltaV = totalDeltaV;

    return rocket;
}

double RocketStaging::calculateTotalMass(Rocket &rocket)
{
    double totalMass{rocket.payloadMass};

    for (const auto stage : rocket.stages) {
        totalMass += stage->structureMass + stage->propellantMass;
    }

    return totalMass;
}

EMSCRIPTEN_BINDINGS(rocket_staging_bindings) {
    using namespace emscripten;

    class_<RocketStaging>("RocketStaging")
        .constructor()
        .function("calculateTotalDeltaV", &RocketStaging::calculateTotalDeltaV);

    value_object<RocketStage>("RocketStage")
        .field("specificImpulse", &RocketStage::specificImpulse)
        .field("structureMass", &RocketStage::structureMass)
        .field("propellantMass", &RocketStage::propellantMass)
        .field("initialMass", &RocketStage::initialMass)
        .field("finalMass", &RocketStage::finalMass)
        .field("deltaV", &RocketStage::deltaV);

    value_object<Rocket>("Rocket")
        .field("payloadMass", &Rocket::payloadMass)
        .field("totalMass", &Rocket::totalMass)
        .field("deltaV", &Rocket::deltaV)
        .field("stage1", &Rocket::stage1)
        .field("stage2", &Rocket::stage2)
        .field("stage3", &Rocket::stage3)
        .field("stage4", &Rocket::stage4)
        .field("stage5", &Rocket::stage5);
}