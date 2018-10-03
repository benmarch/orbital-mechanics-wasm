#ifndef ORBITAL_MECHANICS_MUTIL_H
#define ORBITAL_MECHANICS_MUTIL_H

#include "constants.h"
#include "Vector.h"

double fixError(double val);

Vector fixError(const Vector &vec);

bool withinPrecision(double a, double b);

bool withinPrecision(const Vector &a, const Vector &b);

bool withinPrecision(double a, double b, double relEpsilon);

#endif //ORBITAL_MECHANICS_MUTIL_H
