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
  if (isValidFronts()) {
    fillMissingAngle();
  }

  if (frontsQuantity() < 3) {
    fillRectangularTriangle();
    fillIsoscalesTriangle();
  }

  if (frontsQuantity() == 3 && isValidFronts() && anglesQuantity() < 3) {
    if (anglesQuantity() == 2 && validAvailableAngles(angles)) {
      fillMissingAngle();
    }
    calculateMissingAngles();
  } else if (frontsQuantity() == 2 && validAvailableAngles(angles)) {
    if (anglesQuantity() == 2) {
      fillMissingAngle();
    }
    if (calculateMissingFront()) {
      calculateMissingAngles();
    }
  }

  calculateSquare();
  calculateInscribedCircleRadius();
  calculateCircumscribedCircleRadius();

  if (not isValidTriangle()) {
    // rollback
    unpackFromMap(fronts, angles);
  } else {
    // round values
    kRound(&alpha, angles_precision);
    kRound(&beta, angles_precision);
    gamma = 180 - (alpha + beta);
    kRound(&a, fronts_precision);
    kRound(&b, fronts_precision);
    kRound(&c, fronts_precision);
  }
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
  fillMissingAngle();
  if (!alpha) {
    this->alpha = toDegrees(acos((b * b + c * c - a * a) / (2 * b * c)));
  }
  if (!beta) {
    this->beta = toDegrees(acos((a * a + c * c - b * b) / (2 * a * c)));
  }
  if (!gamma) {
    this->gamma = toDegrees(acos((a * a + b * b - c * c) / (2 * a * b)));
  }
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
  if (a == b && a && max(alpha, beta) != 90) alpha = beta = max(alpha, beta);
  if (a == c && a && max(alpha, gamma) != 90) alpha = gamma = max(alpha, gamma);
  if (b == c && b && max(beta, gamma) != 90) beta = gamma = max(beta, gamma);

  // fill a missing front
  if (alpha == beta && alpha) a = b = max(a, b);
  if (alpha == gamma && alpha) a = c = max(a, c);
  if (beta == gamma && beta) b = c = max(b, c);
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
    a = kRound(sqrt(b * b + c * c), fronts_precision);
  } else if (beta == 90 && a && c and !b) {
    b = kRound(sqrt(a * a + c * c), fronts_precision);
  } else if (gamma == 90 && a && b and !c) {
    c = kRound(sqrt(a * a + b * b), fronts_precision);
  }

  // find a cathet by the Pythagorean theorem
  if (alpha == 90 && a) {
    if (b && !c && a * a - b * b > 0)
      c = kRound(sqrt(a * a - b * b), fronts_precision);
    else if (c && !b && a * a - c * c > 0)
      b = kRound(sqrt(a * a - c * c), fronts_precision);
  } else if (beta == 90 && b) {
    if (a && c && b * b - a * a > 0)
      c = kRound(sqrt(b * b - a * a), fronts_precision);
    else if (c && a && b * b - c * c > 0)
      a = kRound(sqrt(b * b - c * c), fronts_precision);
  } else if (gamma == 90 && c) {
    if (b && !a && c * c - b * b > 0)
      a = kRound(sqrt(c * c - b * b), fronts_precision);
    else if (a && !b && c * c - a * a > 0)
      b = kRound(sqrt(c * c - a * a), fronts_precision);
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
  if (alpha) angles.insert(0, alpha);
  if (beta) angles.insert(1, beta);
  if (gamma) angles.insert(2, gamma);
  return angles;
}

QMap<int, double> Triangle::frontsAsMap() {
  QMap<int, double> fronts;
  if (a) fronts.insert(0, a);
  if (b) fronts.insert(1, b);
  if (c) fronts.insert(2, c);
  return fronts;
}

bool Triangle::isValidTriangle() {
  using std::max;
  using std::min;

  if (not isValidFronts() or not isValidAngles()) {
    return false;
  }

  if (a == b && a == c) {
    return alpha == beta && alpha == gamma && round(alpha) == 60;
  }
  if (alpha == beta && alpha == gamma && alpha == 60) {
    return kRound(a, fronts_precision) == kRound(b, fronts_precision) &&
           kRound(a, fronts_precision) == kRound(c, fronts_precision);
  } else {
    double largestSide = max(a, max(b, c));
    double leastSide = min(a, min(b, c));
    double largestAngle = max(alpha, max(beta, gamma));
    double leastAngle = min(alpha, min(beta, gamma));

    // check the least side and angle
    if (leastSide == a && leastAngle != alpha) {
      return false;
    }
    if (leastSide == b && leastAngle != beta) {
      return false;
    }
    if (leastSide == c && leastAngle != gamma) {
      return false;
    }

    // check the largest side and angle
    if (largestSide == a && largestAngle != alpha) {
      return false;
    }
    if (largestSide == b && largestAngle != beta) {
      return false;
    }
    if (largestSide == c && largestAngle != gamma) {
      return false;
    }
    return true;
  }
}
