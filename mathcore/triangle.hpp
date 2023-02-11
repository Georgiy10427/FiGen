#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QList>
#include <QMap>
#include <QString>
#include <QDebug>
#include <QPoint>
#include <QSize>
#include <algorithm>
#include <iostream>

#include "BaseFigure.hpp"

class Triangle : private BaseFigure {
 public:
  Triangle();
  Triangle(QMap<int, double> fronts, QMap<int, double> angles,
           int fronts_precision = 0, int angles_precision = 0);

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

  int frontsQuantity();
  int anglesQuantity();
  bool isValidAngles();
  bool isValidFronts();
  bool isValidRectangularTriangle();
  void roundFields();

  QMap<int, double> anglesAsMap();
  QMap<int, double> frontsAsMap();

  double a, b, c;
  double alpha, beta, gamma;
  double square;
  double circumscribedCircleRadius, inscribedCircleRadius;
  int fronts_precision = 0, angles_precision = 0, checkAccuracy = 3;

 private:
  void unpackFromMap(QMap<int, double> fronts, QMap<int, double> angles);

  /* finders: */
  void addMissingInformation(QMap<int, double> fronts,
                             QMap<int, double> angles);
  void fillMissingAngle();
  void fillIsoscalesTriangle();
  void fillRectangularTriangle();
  void calculateMissingAngles();
  bool calculateMissingFront();

  /* simple calculators */
  void calculateProperties();
  void calculateSquare();
  void calculateCircumscribedCircleRadius();
  void calculateInscribedCircleRadius();
  QPoint rotatePoint(QPoint origin, double angle, QPoint point);
};

#endif  // TRIANGLE_H
