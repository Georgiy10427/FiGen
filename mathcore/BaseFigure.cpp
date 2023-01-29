#include "BaseFigure.hpp"

double BaseFigure::toDegrees(double radians)
{
    return radians * (180.f/kPi);
}

double BaseFigure::toRadians(double degrees)
{
    return degrees*kPi/180.f;
}

void BaseFigure::kRound(double *value, double decimalPlaces)
{
    *value = round(*value * pow(10, decimalPlaces)) / pow(10, decimalPlaces);
}

double BaseFigure::kRound(double value, double decimalPlaces)
{
    return round(value * pow(10, decimalPlaces)) / pow(10, decimalPlaces);
}
