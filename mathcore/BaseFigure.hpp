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
    static constexpr double kPi = 3.141593;
    static constexpr int kDecimal = 2;
    static constexpr int kAnglesDecimal = 0;
};

#endif // BASEFIGURE_H
