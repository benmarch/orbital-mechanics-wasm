/**
 * Orbital Mechanics
 *
 * Created by Ben March on 27 September 2018
 *
 * Calculates orbital elements
 */

#include "Application.h"
#include <emscripten/bind.h>
#include <emscripten/emscripten.h>

using namespace emscripten;

Orbit Application::createOrbit(StateVectors stateVectors)
{
    Orbit orbit{};
    orbit.generateClassicalElements(stateVectors);
    return orbit;
}

Orbit Application::createOrbit(ClassicalElements elements)
{
    Orbit orbit{};
    orbit.generateStateVectors(elements);
    return orbit;
}

EMSCRIPTEN_BINDINGS(om_bindings) {
    class_<Application>("OrbitalMechanics")
        .constructor()
        .function("createOrbit", select_overload<Orbit(StateVectors)>(&Application::createOrbit))
        .function("createOrbit", select_overload<Orbit(ClassicalElements)>(&Application::createOrbit))
        ;
};
