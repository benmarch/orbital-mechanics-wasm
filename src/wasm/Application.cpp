/**
 * Orbital Mechanics
 *
 * Created by Ben March on 27 September 2018
 *
 * Calculates orbital elements
 */

#include "Application.hpp"
#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(om_bindings) {
    using namespace emscripten;

    value_object<OrbitalElements>("OrbitalElements")
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

    value_object<StateVectors>("StateVectors")
            .field("position", &StateVectors::position)
            .field("velocity", &StateVectors::velocity)
            ;

    value_object<LaunchSite>("LaunchSite")
        .field("lst", &LaunchSite::lst)
        .field("lat", &LaunchSite::lat)
        .field("alt", &LaunchSite::alt);

    value_object<LaunchWindow>("LaunchWindow")
        .field("alpha", &LaunchWindow::alpha)
        .field("gamma", &LaunchWindow::gamma)
        .field("delta", &LaunchWindow::delta)
        .field("lwst", &LaunchWindow::lwst)
        .field("beta", &LaunchWindow::beta)
        .field("waitTime", &LaunchWindow::waitTime)
        .field("isNext", &LaunchWindow::isNext);
};
