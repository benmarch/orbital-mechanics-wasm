#include <emscripten/bind.h>
#include <math.h>
#include "LaunchRequirementCalculator.hpp"
#include "LaunchWindowCalculator.hpp"

LaunchRequirementCalculator::LaunchRequirementCalculator(const Orbit &orbit, const LaunchSite &launchSite):
    m_orbit{orbit}, m_launchSite{launchSite}
{ }

double LaunchRequirementCalculator::calculateRequiredDeltaV(double deltaVLosses)
{
    m_deltaVLosses = deltaVLosses;
    m_vLS = calculateLaunchSiteVelocity(m_launchSite);
    m_vLG = calculateVelocityLossToGravity(m_orbit);
    m_vBO = calculateVelocityAtBurnout(m_orbit, m_launchSite);
    m_deltaVNeeded = calculateDeltaVNeeded(m_vLS, m_vLG, m_vBO);
    m_deltaVDesign = calculateDeltaVDesign(m_deltaVNeeded, m_deltaVLosses);

    return m_deltaVDesign;
}

Vector LaunchRequirementCalculator::getLaunchSiteVelocity() const
{
    return m_vLS;
}

Vector LaunchRequirementCalculator::getVelocityLossToGravity() const
{
    return m_vLG;
}

Vector LaunchRequirementCalculator::getVelocityAtBurnout() const
{
    return m_vBO;
}

double LaunchRequirementCalculator::getDeltaVNeeded() const
{
    return m_deltaVNeeded;
}

double LaunchRequirementCalculator::getDeltaVDesign() const
{
    return m_deltaVDesign;
}

Vector LaunchRequirementCalculator::calculateLaunchSiteVelocity(const LaunchSite &launchSite)
{
    return Vector{0, 0.4651 * cos(launchSite.lat), 0};
}

Vector LaunchRequirementCalculator::calculateVelocityLossToGravity(const Orbit &orbit)
{
    const double z = sqrt((2 * orbit.getMu() * (orbit.getRadius() - EARTH.getRadius())) / (EARTH.getRadius() * orbit.getRadius()));

    return Vector{0, 0, z};
}

Vector LaunchRequirementCalculator::calculateVelocityAtBurnout(const Orbit &orbit, const LaunchSite &launchSite)
{
    const double v = orbit.getVelocity();
    const LaunchWindowCalculator launchWindowCalculator{orbit, launchSite};
    const LaunchWindow launchWindow = launchWindowCalculator.getNextOpportunity();
    const double beta = launchWindow.beta;

    std::cout << beta << std::endl;

    return Vector{
        -1 * v * cos(beta),
        v * sin(beta),
        0
    };
}

double LaunchRequirementCalculator::calculateDeltaVNeeded(Vector vLS, Vector vLG, Vector vBO)
{
    return (vLG + vBO - vLS).getMagnitude();
}

double LaunchRequirementCalculator::calculateDeltaVDesign(double deltaVNeeded, double deltaVLosses)
{
    return deltaVNeeded + deltaVLosses;
}

EMSCRIPTEN_BINDINGS(launch_requirement_calculator_bindings) {
    using namespace emscripten;

    class_<LaunchRequirementCalculator>("LaunchRequirementCalculator")
        .constructor<Orbit&, LaunchSite&>()
        .function("calculateRequiredDeltaV", &LaunchRequirementCalculator::calculateRequiredDeltaV)
        .property("vLS", &LaunchRequirementCalculator::getLaunchSiteVelocity)
        .property("vLG", &LaunchRequirementCalculator::getVelocityLossToGravity)
        .property("vBO", &LaunchRequirementCalculator::getVelocityAtBurnout)
        .property("deltaVNeeded", &LaunchRequirementCalculator::getDeltaVNeeded)
        .property("deltaVDesign", &LaunchRequirementCalculator::getDeltaVDesign);
}