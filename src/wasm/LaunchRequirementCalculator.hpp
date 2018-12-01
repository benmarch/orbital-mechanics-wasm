#ifndef ORBITAL_MECHANICS_LAUNCHREQUIREMENTCALCULATOR_HPP
#define ORBITAL_MECHANICS_LAUNCHREQUIREMENTCALCULATOR_HPP

#include "Orbit.hpp"
#include "constants.hpp"

class LaunchRequirementCalculator
{
public:
    LaunchRequirementCalculator(const Orbit &orbit, const LaunchSite &launchSite);

    double calculateRequiredDeltaV(double deltaVLosses);

    Vector getLaunchSiteVelocity() const;
    Vector getVelocityLossToGravity() const;
    Vector getVelocityAtBurnout() const;
    double getDeltaVNeeded() const;
    double getDeltaVDesign() const;

private:
    Vector calculateLaunchSiteVelocity(const LaunchSite &launchSite);
    Vector calculateVelocityLossToGravity(const Orbit &orbit);
    Vector calculateVelocityAtBurnout(const Orbit &orbit, const LaunchSite &launchSite);
    double calculateDeltaVNeeded(Vector vLS, Vector vLG, Vector vBO);
    double calculateDeltaVDesign(double deltaVNeeded, double deltaVLosses);

    const Orbit m_orbit;
    const LaunchSite m_launchSite;
    double m_deltaVLosses;
    Vector m_vLS;
    Vector m_vLG;
    Vector m_vBO;
    double m_deltaVNeeded;
    double m_deltaVDesign;
};


#endif //ORBITAL_MECHANICS_LAUNCHREQUIREMENTCALCULATOR_HPP
