#ifndef BASEFIGURE_H
#define BASEFIGURE_H

#include <cmath>
#include <QDebug>

class BaseFigure
{
protected:
    double toRadians(double degrees);
    double toDegrees(double radians);
    void kRound(double *value, double decimalPlaces=kdefaultDecimal);
    double kRound(double value, double decimalPlaces=kdefaultDecimal);
    double kPi = std::acos(-1);
    static constexpr int kdefaultDecimal = 2;
    static constexpr int kmaxDecimal = 6;
};

#endif // BASEFIGURE_H
