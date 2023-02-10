#include "triangle.hpp"

Triangle::Triangle() {}

Triangle::Triangle(QMap<int, double> fronts, QMap<int, double> angles,
                   int fronts_precision, int angles_precision) {
  this->fronts_precision = fronts_precision;
  this->angles_precision = angles_precision;
  unpackFromMap(fronts, angles);
  addMissingInformation(fronts, angles);
}

void Triangle::addMissingInformation(QMap<int, double> fronts,
                                     QMap<int, double> angles) {
  if(frontsQuantity() == 3 and anglesQuantity() == 3)
  {
      calculateSquare();
      calculateInscribedCircleRadius();
      calculateCircumscribedCircleRadius();
      return;
  }

  if(isValidAngles())
  {
      fillMissingAngle();
  }

  fillRectangularTriangle();
  fillIsoscalesTriangle();

  qDebug() << "A:" << a << " " << b << " " << c;
  qDebug() << alpha << " " << beta << " "<< gamma;

  if (frontsQuantity() == 3 && isValidFronts()) {
    calculateMissingAngles();
  } else if (frontsQuantity() == 2 && validAvailableAngles(angles)) {
    if (calculateMissingFront()) {
      calculateMissingAngles();
    }
  }

  calculateSquare();
  calculateInscribedCircleRadius();
  calculateCircumscribedCircleRadius();
  roundFields();
  /*if (not isValidTriangle()) {
    unpackFromMap(fronts, angles);
  } else {
    // round values
    auto pointFronts = frontsAsMap();
    auto pointAngles = anglesAsMap();

    kRound(&alpha, angles_precision);
    kRound(&beta, angles_precision);
    gamma = 180 - (alpha + beta);
    kRound(&a, fronts_precision);
    kRound(&b, fronts_precision);
    kRound(&c, fronts_precision);

    if(not isValidTriangle())
    {
        unpackFromMap(pointFronts, pointAngles);
    }
  }*/
}

void Triangle::unpackFromMap(QMap<int, double> fronts,
                             QMap<int, double> angles) {
  alpha = 0;
  beta = 0;
  gamma = 0;
  if (angles.contains(0)) this->alpha = angles[0];
  if (angles.contains(1)) this->beta = angles[1];
  if (angles.contains(2)) this->gamma = angles[2];

  a = 0;
  b = 0;
  c = 0;
  if (fronts.contains(0)) this->a = fronts[0];
  if (fronts.contains(1)) this->b = fronts[1];
  if (fronts.contains(2)) this->c = fronts[2];
}

void Triangle::calculateMissingAngles() {
  using std::acos;
  fillMissingAngle();
  if (!alpha) {
    this->alpha = toDegrees(acos((b * b + c * c - a * a) / (2 * b * c)));
  }
  fillMissingAngle();
  if (!beta) {
    this->beta = toDegrees(acos((a * a + c * c - b * b) / (2 * a * c)));
  }
  fillMissingAngle();
  if (!gamma) {
    this->gamma = toDegrees(acos((a * a + b * b - c * c) / (2 * a * b)));
  }
  fillMissingAngle();
}

void Triangle::fillMissingAngle() {
  if (alpha && beta && !gamma) {
    gamma = 180 - (alpha + beta);
  } else if (alpha && gamma && !beta) {
    beta = 180 - (alpha + gamma);
  } else if (beta && gamma && !alpha) {
    alpha = 180 - (beta + gamma);
  }
}

bool Triangle::calculateMissingFront() {
  if (a && b && !c && gamma) {
    c = sqrt(a * a + b * b - 2 * a * b * cos(toRadians(gamma)));
    return true;
  }
  if (a && c && !b && beta) {
    b = sqrt(a * a + c * c - 2 * a * c * cos(toRadians(beta)));
    return true;
  }
  if (b && c && !a && alpha) {
    a = sqrt(b * b + c * c - 2 * b * c * cos(toRadians(alpha)));
    return true;
  }
  return false;
}

bool Triangle::isIsoscelesByAngles() {
  if (alpha && alpha == beta)
    return true;
  else if (beta && beta == gamma)
    return true;
  else if (alpha && alpha == gamma)
    return true;
  return false;
}

bool Triangle::isIsoscelesByFronts() {
  if (a && a == b)
    return true;
  else if (b && b == c)
    return true;
  else if (a && a == c)
    return true;
  return false;
}

bool Triangle::isIsosceles() {
  return isIsoscelesByAngles() || isIsoscelesByFronts();
}

void Triangle::calculateSquare() {
  if (a > 0 and b > 0 and c > 0) {
    double p = (a + b + c) / 2;
    square = kRound(sqrt(p * (p - a) * (p - b) * (p - c)), fronts_precision);
  }
}

void Triangle::calculateCircumscribedCircleRadius() {
  // refactor
  if (a > 0 && b > 0 && c > 0 && square > 0) {
    circumscribedCircleRadius = (a * b * c) / (4 * square);
  }
}

void Triangle::calculateInscribedCircleRadius() {
  if (a > 0 && b > 0 && c > 0 && square > 0) {
    inscribedCircleRadius = square / ((a + b + c) / 2);
  }
}

void Triangle::fillIsoscalesTriangle() {
  using std::max;
  if (a and b and c and not isValidFronts()) return;

  // fill a missing angle
  if (a == b && a && max(alpha, beta) != 90) {
      if(alpha && beta) return;
      alpha = beta = max(alpha, beta);
  }
  if (a == c && a && max(alpha, gamma) != 90) {
      if(alpha && gamma) return;
      alpha = gamma = max(alpha, gamma);
  }
  if (b == c && b && max(beta, gamma) != 90) {
      if(beta && gamma) return;
      beta = gamma = max(beta, gamma);
  }

  // fill a missing front
  if (alpha == beta && alpha) {
      if(a && b) return;
      a = b = max(a, b);
  }
  if (alpha == gamma && alpha) {
      if(a && c) return;
      a = c = max(a, c);
  }
  if (beta == gamma && beta) {
      if(b && c) return;
      b = c = max(b, c);
  }
}

void Triangle::fillRectangularTriangle() {
  if (not validAvailableAngles(anglesAsMap())) return;

  // a cathet opposite of 30 degrees angle equals half past of a hypotenuse
  if (alpha == 90 && a) {
    if (gamma == 30 && !c)
      c = a / 2;
    else if (beta == 30 && !b)
      b = a / 2;
  } else if (gamma == 90 && c) {
    if (beta == 30 && !b)
      b = c / 2;
    else if (alpha == 30 && !a)
      a = c / 2;
  } else if (beta == 90 && b) {
    if (alpha == 30 && !a) a = b / 2;
    if (gamma == 30 && !c) c = b / 2;
  }

  // find a hypotenuse by the Pythagorean theorem
  if (alpha == 90 && b && c and !a) {
    a = sqrt(b * b + c * c);
  } else if (beta == 90 && a && c and !b) {
    b = sqrt(a * a + c * c);
  } else if (gamma == 90 && a && b and !c) {
    c = sqrt(a * a + b * b);
  }

  // find a cathet by the Pythagorean theorem
  if (alpha == 90 && a) {
    if (b && !c && a * a - b * b > 0)
      c = sqrt(a * a - b * b);
    else if (c && !b && a * a - c * c > 0)
      b = sqrt(a * a - c * c);
  } else if (beta == 90 && b) {
    if (a && c && b * b - a * a > 0)
      c = sqrt(b * b - a * a);
    else if (c && a && b * b - c * c > 0)
      a = sqrt(b * b - c * c);
  } else if (gamma == 90 && c) {
    if (b && !a && c * c - b * b > 0)
      a = sqrt(c * c - b * b);
    else if (a && !b && c * c - a * a > 0)
      b = sqrt(c * c - a * a);
  }
}

bool Triangle::validAvailableAngles(QMap<int, double> angles) {
  double sum = 0;
  for (auto angle : angles.values()) {
    sum += angle;
  }
  if (round(sum) == 180 && angles.size() == 3)
    return true;
  else if (sum < 180 && angles.size() < 3)
    return true;
  else {
    return false;
  }
}

bool Triangle::isValidAngles() {
  double sum = alpha + beta + gamma;
  int quantity = (alpha > 0) + (beta > 0) + (gamma > 0);
  qDebug() << round(sum);
  if (round(sum) == 180 && quantity == 3)
    return true;
  else if (sum < 180 && quantity < 3)
    return true;
  else {
    return false;
  }
}

bool Triangle::validFronts(QMap<int, double> fronts) {
  if (fronts.keys() != QList<int>{0, 1, 2}) return false;
  double sideA = fronts[0];
  double sideB = fronts[1];
  double sideC = fronts[2];
  if (sideA >= sideB + sideC) return false;
  if (sideC >= sideA + sideB) return false;
  if (sideB >= sideA + sideC) return false;
  return true;
}

bool Triangle::isValidFronts() {
  if (a >= b + c) return false;
  if (c >= a + b) return false;
  if (b >= a + c) return false;
  return true;
}

int Triangle::frontsQuantity() { return (a > 0) + (b > 0) + (c > 0); }

int Triangle::anglesQuantity() {
  return (alpha > 0) + (beta > 0) + (gamma > 0);
}

QMap<int, double> Triangle::anglesAsMap() {
  QMap<int, double> angles;
  if (alpha > 0) angles.insert(0, alpha);
  if (beta > 0) angles.insert(1, beta);
  if (gamma > 0) angles.insert(2, gamma);
  return angles;
}

QMap<int, double> Triangle::frontsAsMap() {
  QMap<int, double> fronts;
  if (a > 0) fronts.insert(0, a);
  if (b > 0) fronts.insert(1, b);
  if (c > 0) fronts.insert(2, c);
  return fronts;
}

bool Triangle::isValidRectangularTriangle()
{
    if(alpha == 90 || beta == 90 || gamma == 90)
    {
        if(alpha == 90 && round(a*a) != round(b*b+c*c)) return false;
        if(beta == 90 && round(b*b) != round(a*a+c*c)) return false;
        if(gamma == 90 && round(c*c) != round(a*a+b*b)) return false;
        return true;
    } else {
        return false;
    }
}

void Triangle::roundFields()
{
    using std::pow;

    auto doubleFronts = frontsAsMap();
    auto doubleAngles = anglesAsMap();
    bool wasRectangular = isValidRectangularTriangle();

    kRound(&a, fronts_precision);
    kRound(&b, fronts_precision);
    kRound(&c, fronts_precision);
    kRound(&alpha, angles_precision);
    kRound(&beta, angles_precision);
    gamma = 180 - (alpha + beta);

    if(not isValidRectangularTriangle() && wasRectangular)
    {
        if(fronts_precision >= 3)
        {
            unpackFromMap(doubleFronts, doubleAngles);
            return;
        } else{
            ++fronts_precision;
            unpackFromMap(doubleFronts, doubleAngles);
        }
    }
}

QPoint Triangle::rotatePoint(QPoint origin, double angle, QPoint point)
{
    double s = std::sin(toRadians(angle));
    double c = std::cos(toRadians(angle));

    point.setX(point.x()-origin.x());
    point.setY(point.y()-origin.y());

    double xnew = point.x()*c-point.y()*s;
    double ynew = point.x()*s+point.y()*c;

    point.setX(xnew+origin.x());
    point.setY(ynew+origin.y());
    return point;
}

bool Triangle::isValidTriangle()
{
    using std::min;
    using std::max;

    if(not isValidFronts()) return false;

    double maxLineLength, maxSideLength, scaleFactor, aFrontMargin, marginBottom;
    double scale=1;
    QPoint firstPoint, secondPoint, thirdPoint, forthPoint;
    QSize canvasSize(300, 400);

    // calculate scaleFactor and margins
    maxLineLength = min(canvasSize.width(), canvasSize.height())*scale;
    maxSideLength = max(a, max(b, c));
    scaleFactor = maxLineLength/maxSideLength;
    aFrontMargin = (canvasSize.width() - a*scaleFactor)/2;
    marginBottom = canvasSize.height()*0.65; // get 66 percents as the margin bottom

    // place 3 points
    firstPoint = QPoint(aFrontMargin, marginBottom);
    secondPoint = QPoint(aFrontMargin + a*scaleFactor, marginBottom);
    thirdPoint = QPoint(secondPoint.x() - b*scaleFactor, marginBottom);
    forthPoint = QPoint(aFrontMargin + c*scaleFactor, marginBottom);

    // rotate B side
    thirdPoint = rotatePoint(secondPoint, gamma, thirdPoint);
    forthPoint = rotatePoint(firstPoint, -beta, forthPoint);

    qDebug() << QString("A: (%1, %2); B: (%3, %4)").arg(thirdPoint.x()).arg(thirdPoint.y()).arg(forthPoint.x()).arg(forthPoint.y());
    if(abs(thirdPoint.x()-forthPoint.x()) > 5)
    {
        return false;
    }
    if(abs(thirdPoint.y()-forthPoint.y()) > 5)
    {
        return false;
    }
    if(abs(forthPoint.x() - forthPoint.x()) > 5)
    {
        return false;
    }
    if(abs(forthPoint.y() - forthPoint.y()) > 5)
    {
        return false;
    }

    if(round(alpha) != round(180-(beta+gamma)))
    {
        return false;
    }
    return true;
}
