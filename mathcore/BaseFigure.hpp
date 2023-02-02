#ifndef BASEFIGURE_H
#define BASEFIGURE_H

#include <cmath>

class BaseFigure
{
protected:
    double toRadians(double degrees);
    double toDegrees(double radians);
    void kRound(double *value, double decimalPlaces=kDecimal);
    double kRound(double value, double decimalPlaces=kDecimal);
    double kPi = std::acos(-1);
    static constexpr int kDecimal = 2;
};

#endif // BASEFIGURE_H
