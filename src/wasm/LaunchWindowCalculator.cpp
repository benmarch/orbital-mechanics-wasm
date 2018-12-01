#include <emscripten/bind.h>
#include <math.h>
#include "LaunchWindowCalculator.hpp"
#include "mutil.hpp"

LaunchWindowCalculator::LaunchWindowCalculator(const Orbit &orbit, LaunchSite &launchSite):
    m_orbit{orbit}, m_launchSite{launchSite}
{
    calculateNumberOfLaunchOpportunities();

    if (m_numLaunchOpportunities == 0) {
        return;
    }

    if (m_numLaunchOpportunities == 1) {
        calculateOnlyOpportunity();
    } else {
        calculateAscendingNodeOpportunity();
        calculateDescendingNodeOpportunity();

        // which opportunity is next?
        if (m_ascendingNodeOpportunity.lwst - m_launchSite.lst >= 0 && m_descendingNodeOpportunity.lwst - m_launchSite.lst <= 0) {
            m_ascendingNodeOpportunity.isNext = true;
        } else if (m_descendingNodeOpportunity.lwst - m_launchSite.lst >= 0 && m_ascendingNodeOpportunity.lwst - m_launchSite.lst <= 0) {
            m_descendingNodeOpportunity.isNext = true;
        } else if (m_ascendingNodeOpportunity.lwst < m_descendingNodeOpportunity.lwst) {
            m_ascendingNodeOpportunity.isNext = true;
        } else {
            m_descendingNodeOpportunity.isNext = true;
        }
    }
}

int LaunchWindowCalculator::getNumLaunchOpportunities() const
{
    return m_numLaunchOpportunities;
}

LaunchWindow LaunchWindowCalculator::getOnlyOpportunity() const
{
    return m_onlyOpportunity;
}

LaunchWindow LaunchWindowCalculator::getAscendingNodeOpportunity() const
{
    return m_ascendingNodeOpportunity;
}

LaunchWindow LaunchWindowCalculator::getDescendingNodeOpportunity() const
{
    return m_descendingNodeOpportunity;
}

void LaunchWindowCalculator::calculateNumberOfLaunchOpportunities()
{
    if (withinPrecision(m_orbit.getElements().i, m_launchSite.lat)) {
        m_numLaunchOpportunities = 1;
    } else if (m_orbit.getElements().i < m_launchSite.lat) {
        m_numLaunchOpportunities = 0;
    } else {
        m_numLaunchOpportunities = 2;
    }
}

void LaunchWindowCalculator::calculateOnlyOpportunity() {
    m_onlyOpportunity = LaunchWindow{};

    m_onlyOpportunity.lwst = reduceToWithin(m_orbit.getElements().Om + M_PI, 2 * M_PI);
    m_onlyOpportunity.waitTime = (m_onlyOpportunity.lwst >= m_launchSite.lst ?
                                 m_onlyOpportunity.lwst - m_launchSite.lst :
                                 2 * M_PI - m_launchSite.lst + m_onlyOpportunity.lwst) / (15*M_PI/180);
    m_onlyOpportunity.isNext = true;
}

void LaunchWindowCalculator::calculateAscendingNodeOpportunity()
{
    m_ascendingNodeOpportunity = LaunchWindow{};

    m_ascendingNodeOpportunity.alpha = m_orbit.getElements().i;
    m_ascendingNodeOpportunity.gamma = asin(cos(m_ascendingNodeOpportunity.alpha) / cos(m_launchSite.lat));
    m_ascendingNodeOpportunity.delta = acos(cos(m_ascendingNodeOpportunity.gamma) / sin(m_ascendingNodeOpportunity.alpha));
    m_ascendingNodeOpportunity.lwst = reduceToWithin(m_orbit.getElements().Om + m_ascendingNodeOpportunity.delta, 2 * M_PI);
    m_ascendingNodeOpportunity.waitTime = (m_ascendingNodeOpportunity.lwst >= m_launchSite.lst ?
                                          m_ascendingNodeOpportunity.lwst - m_launchSite.lst :
                                           2 * M_PI - m_launchSite.lst + m_ascendingNodeOpportunity.lwst) / (15*M_PI/180);
    m_ascendingNodeOpportunity.beta = m_ascendingNodeOpportunity.gamma;
}

void LaunchWindowCalculator::calculateDescendingNodeOpportunity()
{
    m_descendingNodeOpportunity = LaunchWindow{};

    m_descendingNodeOpportunity.alpha = m_orbit.getElements().i;
    m_descendingNodeOpportunity.gamma = asin(cos(m_descendingNodeOpportunity.alpha) / cos(m_launchSite.lat));
    m_descendingNodeOpportunity.delta = acos(cos(m_descendingNodeOpportunity.gamma) / sin(m_descendingNodeOpportunity.alpha));
    m_descendingNodeOpportunity.lwst = reduceToWithin(m_orbit.getElements().Om + (M_PI - m_descendingNodeOpportunity.delta), 2 * M_PI);
    m_descendingNodeOpportunity.waitTime = (m_descendingNodeOpportunity.lwst >= m_launchSite.lst ?
                                            m_descendingNodeOpportunity.lwst - m_launchSite.lst :
                                            2 * M_PI - m_launchSite.lst + m_descendingNodeOpportunity.lwst) / (15*M_PI/180);

    m_descendingNodeOpportunity.beta = M_PI - m_descendingNodeOpportunity.gamma;
}

EMSCRIPTEN_BINDINGS(launch_window_calculator_bindings) {
    using namespace emscripten;

    class_<LaunchWindowCalculator>("LaunchWindowCalculator")
        .constructor<Orbit&, LaunchSite&>()
        .property("numLaunchOpportunities", &LaunchWindowCalculator::getNumLaunchOpportunities)
        .property("onlyOpportunity", &LaunchWindowCalculator::getOnlyOpportunity)
        .property("ascendingNodeOpportunity", &LaunchWindowCalculator::getAscendingNodeOpportunity)
        .property("descendingNodeOpportunity", &LaunchWindowCalculator::getDescendingNodeOpportunity);

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
}