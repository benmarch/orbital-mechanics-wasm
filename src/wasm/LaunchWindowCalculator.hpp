#ifndef ORBITAL_MECHANICS_LAUNCHWINDOWCALCULATOR_HPP
#define ORBITAL_MECHANICS_LAUNCHWINDOWCALCULATOR_HPP

#include "Orbit.hpp"
#include "constants.hpp"

class LaunchWindowCalculator
{
public:
    LaunchWindowCalculator(const Orbit &orbit, const LaunchSite &launchSite);

    int getNumLaunchOpportunities() const;
    LaunchWindow getOnlyOpportunity() const;
    LaunchWindow getAscendingNodeOpportunity() const;
    LaunchWindow getDescendingNodeOpportunity() const;
    LaunchWindow getNextOpportunity() const;

private:
    void calculateNumberOfLaunchOpportunities();
    void calculateOnlyOpportunity();
    void calculateAscendingNodeOpportunity();
    void calculateDescendingNodeOpportunity();

    const Orbit m_orbit;
    const LaunchSite m_launchSite;
    int m_numLaunchOpportunities;
    LaunchWindow m_onlyOpportunity;
    LaunchWindow m_ascendingNodeOpportunity;
    LaunchWindow m_descendingNodeOpportunity;
};


#endif //ORBITAL_MECHANICS_LAUNCHWINDOWCALCULATOR_HPP
