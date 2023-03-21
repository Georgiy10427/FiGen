#include "BaseFigure.hpp"

double BaseFigure::toDegrees(double radians) {
    return radians * (180.f / kPi);
}

double BaseFigure::toRadians(double degrees) { return degrees * kPi / 180.f; }

void BaseFigure::kRound(double *value, double decimalPlaces) {
    *value = round(*value * pow(10, decimalPlaces)) / pow(10, decimalPlaces);
}

double BaseFigure::kRound(double value, double decimalPlaces) {
    return round(value * pow(10, decimalPlaces)) / pow(10, decimalPlaces);
}

double BaseFigure::rand(double minValue, double maxValue, bool isFloat,
                        int decimal) {
    std::random_device RandomDevice;
    unsigned seed = RandomDevice();
    std::default_random_engine re(seed);
    std::uniform_real_distribution<double> doubleDist(minValue, maxValue);
    std::uniform_int_distribution<int> intDist(minValue, maxValue);
    if (isFloat) {
        return kRound(doubleDist(re), decimal);
    } else {
        return kRound(intDist(re), decimal);
    }
}
