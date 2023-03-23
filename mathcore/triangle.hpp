#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "BaseFigure.hpp"
#include <QDebug>
#include <QList>
#include <QMap>
#include <QPoint>
#include <QSize>
#include <QString>
#include <QTextStream>
#include <algorithm>
#include <iostream>
#include <memory.h>
#include <ostream>
#include <random>

class Triangle : private BaseFigure {
  public:
    Triangle();
    Triangle(QMap<int, double> fronts, QMap<int, double> angles,
             int fronts_precision = 0, int angles_precision = 0,
             bool rollbackOnFail = true);

    /* validators */
    bool isValidTriangle();
    bool validAvailableAngles(QMap<int, double> angles);
    bool validFronts(QMap<int, double> fronts);

    /* properties */
    bool isIsosceles();
    bool isIsoscelesByFronts();
    bool isIsoscelesByAngles();
    bool isRectangular();
    bool isEquilateral();
    bool isEquilateralByFronts();
    bool isEquilaterialByAngles();

    int frontsQuantity();
    int anglesQuantity();
    bool isValidAngles();
    bool isValidFronts();
    bool isValidRectangularTriangle();
    void roundFields();

    // generators
    void generate(double minSide, double maxSide, bool rectangular = false,
                  bool isoscales = false, bool equileterial = false,
                  bool preferInt = true);
    std::vector<uint64_t> generatePythogoreanThree(int minBorder,
                                                   int maxBorder);

    void print(std::string stepName = "", bool bigSeparator = false,
               bool debugOnly = true);
    std::vector<double> shuffleArray(std::vector<double> array);

    void unpackFromMap(QMap<int, double> fronts, QMap<int, double> angles);
    void unpackFromVectors(std::vector<double> fronts,
                           std::vector<double> angles);

    QMap<int, double> frontsAsMap();
    std::vector<double> frontsAsVector();
    QMap<int, double> anglesAsMap();
    std::vector<double> anglesAsVector();

    double a = 0, b = 0, c = 0;
    double alpha = 0, beta = 0, gamma = 0;
    double square;
    double circumscribedCircleRadius, inscribedCircleRadius;
    int fronts_precision = 0, angles_precision = 0;

  private:
    /* finders: */
    void addMissingInformation(QMap<int, double> fronts,
                               QMap<int, double> angles,
                               bool rollbackOnFail = true);
    void fillMissingAngle();
    void fillIsoscalesTriangle();
    void fillRectangularTriangle();
    void calculateMissingAngles();
    bool calculateMissingFront();
    void calcSideOpposite30Angle(double *hypotenuse, double *catet);
    double *getSideOppositeAngle(double angle);
    double *getMissingCatet();

    /* simple calculators */
    void calculateProperties();
    void calculateSquare();
    void calculateCircumscribedCircleRadius();
    void calculateInscribedCircleRadius();
};

#endif // TRIANGLE_H
