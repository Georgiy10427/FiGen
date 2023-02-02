#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "BaseFigure.hpp"
#include <QList>
#include <QMap>
#include <QString>
#include <iostream>
#include <algorithm>

// To-do: rewrite the square/circles_radius function to more flexible
// write checks for rectangular triangles

class Triangle : private BaseFigure
{
public:
    Triangle();
    Triangle(QMap<int, double> fronts, QMap<int, double> angles,
             int fronts_precision=0, int angles_precision=0);

    /* validators */
    bool validAvailableAngles(QMap<int, double> angles);
    bool validFronts(QMap<int, double> fronts);

    /* properties */
    bool isIsosceles();
    bool isIsoscelesByFronts();
    bool isIsoscelesByAngles();

    QMap<int, double> anglesAsMap();
    QMap<int, double> frontsAsMap();


    double a, b, c;
    double alpha, beta, gamma;
    double square;
    double circumscribedCircleRadius, inscribedCircleRadius;
    int fronts_precision = 0, angles_precision = 0;
private:
    void unpackFromMap(QMap<int, double> fronts, QMap<int, double> angles);

    /* finders: */
    void addMissingInformation(QMap<int, double> fronts, QMap<int, double> angles);
    void fillIsoscalesTriangle();
    void fillRectangularTriangle();
    void calculateMissingAngles();
    void fillMissingAngle();
    bool calculateMissingFront();

    /* simple calculators */
    void calculateSquare();
    void calculateCircumscribedCircleRadius();
    void calculateInscribedCircleRadius();
};

#endif // TRIANGLE_H
