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
    orbit.generateOrbitalElements(stateVectors);
    return orbit;
}

Orbit Application::createOrbit(OrbitalElements elements)
{
    Orbit orbit{};
    orbit.generateStateVectors(elements);
    return orbit;
}

EMSCRIPTEN_BINDINGS(om_bindings) {
    class_<Application>("OrbitalMechanics")
        .constructor()
        .function("createOrbit", select_overload<Orbit(StateVectors)>(&Application::createOrbit))
        .function("createOrbit", select_overload<Orbit(OrbitalElements)>(&Application::createOrbit))
        ;

    emscripten::value_object<OrbitalElements>("OrbitalElements")
            .field("a", &OrbitalElements::a)
            .field("e", &OrbitalElements::e)
            .field("i", &OrbitalElements::i)
            .field("o", &OrbitalElements::o)
            .field("Om", &OrbitalElements::Om)
            .field("nu", &OrbitalElements::nu)
            .field("u", &OrbitalElements::u)
            .field("Pi", &OrbitalElements::Pi)
            .field("l", &OrbitalElements::l)
            ;

    emscripten::value_object<StateVectors>("StateVectors")
            .field("position", &StateVectors::position)
            .field("velocity", &StateVectors::velocity)
            ;
};
