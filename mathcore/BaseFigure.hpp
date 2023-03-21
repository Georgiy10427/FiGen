#ifndef BASEFIGURE_H
#define BASEFIGURE_H

#include <QDebug>
#include <cerrno>
#include <cfenv>
#include <cmath>
#include <random>

class BaseFigure {
  protected:
    double toRadians(double degrees);
    double toDegrees(double radians);
    void kRound(double *value, double decimalPlaces = kdefaultDecimal);
    double kRound(double value, double decimalPlaces = kdefaultDecimal);
    double rand(double minValue, double maxValue, bool isFloat = false,
                int decimal = 3);
    double randInt(double minValue, double maxValue, int decimal);
    double kPi = std::acos(-1);
    static constexpr int kdefaultDecimal = 2;
    static constexpr int kmaxDecimal = 6;
};

#endif // BASEFIGURE_H
