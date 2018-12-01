#ifndef ORBITAL_MECHANICS_REENTRY_HPP
#define ORBITAL_MECHANICS_REENTRY_HPP

class Reentry
{
public:
    void calculateFromReentryConditions(double reentryVelocity, double flightpathAngle, double ballisticCoefficient);

    double getMaxDeceleration() const;
    double getAltitudeMaxDeceleration() const;
    double getAltitudeMaxHeating() const;
    double getVelocityMaxHeating() const;

private:
    void calculateMaxDeceleration();
    void calculateAltitudeAtMaxDeceleration();
    void calculateAltitudeAtMaxHeatingRate();
    void calculateVelocityAtMaxHeatingRate();

    double m_reentryVelocity;
    double m_flightpathAngle;
    double m_ballisticCoefficient;
    double m_maxDeceleration;
    double m_altitudeMaxDeceleration;
    double m_altitudeMaxHeating;
    double m_velocityMaxHeating;
};


#endif //ORBITAL_MECHANICS_REENTRY_HPP
