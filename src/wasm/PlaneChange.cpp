#include <math.h>
#include <emscripten/bind.h>
#include "PlaneChange.hpp"
#include "mutil.hpp"

PlaneChange::PlaneChange(const Orbit &orbitFrom, const Orbit &orbitTo):
    m_orbitFrom{orbitFrom}, m_orbitTo{orbitTo}
{
}

void PlaneChange::setPlaneChangeAngle(const double theta)
{
    m_theta = theta;
    calculateDeltaV();
}

void PlaneChange::calculateDeltaV()
{
    double vi = m_orbitFrom.getVelocity();
    double vf = m_orbitTo.getVelocity();

    m_deltaV = sqrt(pow(vi, 2) + pow(vf, 2) - 2 * vi * vf * cos(m_theta));
}

double PlaneChange::getDeltaV() const
{
    return m_deltaV;
}

EMSCRIPTEN_BINDINGS(plane_change_bindings) {
    using namespace emscripten;

    class_<PlaneChange>("PlaneChange")
        .constructor<Orbit&, Orbit&>()
        .function("setPlaneChangeAngle", &PlaneChange::setPlaneChangeAngle)
        .property("deltaV", &PlaneChange::getDeltaV);
};