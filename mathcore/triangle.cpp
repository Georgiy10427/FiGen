#include "triangle.hpp"

Triangle::Triangle() {}

Triangle::Triangle(QMap<int, double> fronts, QMap<int, double> angles,
                   int fronts_precision, int angles_precision,
                   bool rollbackOnFail) {
    this->fronts_precision = fronts_precision;
    this->angles_precision = angles_precision;
    addMissingInformation(fronts, angles, rollbackOnFail);
}

Triangle::Triangle(std::vector<double> fronts, std::vector<double> angles,
                   int fronts_precision, int angles_precision,
                   bool rollbackOnFail) {
    unpackFromVectors(fronts, angles);
    this->fronts_precision = fronts_precision;
    this->angles_precision = angles_precision;
    addMissingInformation(frontsAsMap(), anglesAsMap(), rollbackOnFail);
}

void Triangle::addMissingInformation(QMap<int, double> fronts,
                                     QMap<int, double> angles,
                                     bool rollbackOnFail) {
    /* Solve the triangle and check it. */
    unpackFromMap(fronts, angles);
    print("1. Start solving triangle");

    if (frontsQuantity() == 3 and anglesQuantity() == 3) {
        calculateProperties();
        print("2. Already solved", true);
        return;
    }

    fillRectangularTriangle();
    print("2. Tried to apply rectangular triangle properties");

    fillIsoscalesTriangle();
    print("3. Tried to apply isoscale triangle properties");

    if (isValidAngles()) {
        fillMissingAngle();
        print("- Results of calculating third angle");
    }

    if (frontsQuantity() == 3 && isValidFronts()) {
        calculateMissingAngles();
        print("- Tried to calculate missing angle");
    } else if (frontsQuantity() == 2 && isValidAngles()) {
        if (calculateMissingFront()) {
            calculateMissingAngles();
            print("- Results of calculating a missing side and angle");
        }
    }

    calculateProperties();
    print("4. Square and radious were calculated.");

    roundFields();
    print("5. Rounded sides and angles values.", !rollbackOnFail);

    if (rollbackOnFail) {
        if (isValidTriangle()) {
            print("Triangle was solved.", true);
        } else {
            print("Triangle isn't valid. Rollback.", true);
            unpackFromMap(fronts, angles);
        }
    }
}

void Triangle::unpackFromMap(QMap<int, double> fronts,
                             QMap<int, double> angles) {
    /* Move values from the maps to class private fields. */
    a = b = c = 0;
    double *pfronts[] = {&a, &b, &c};
    for (int i = 0; i < 3; ++i) {
        if (fronts.contains(i) and fronts[i] > 0) {
            *pfronts[i] = fronts[i];
        }
    }

    alpha = beta = gamma = 0;
    double *pangles[] = {&alpha, &beta, &gamma};
    for (int i = 0; i < 3; ++i) {
        if (angles.contains(i) and angles[i] > 0) {
            *pangles[i] = angles[i];
        }
    }
}

void Triangle::unpackFromVectors(std::vector<double> fronts,
                                 std::vector<double> angles) {
    /* Move values from the vectors to class private fields. */
    a = b = c = 0;
    double *pfronts[] = {&a, &b, &c};
    for (int i = 0; i < 3; ++i) {
        try {
            if (fronts.at(i) > 0) {
                *pfronts[i] = fronts.at(i);
            }
        } catch (const std::out_of_range &e) {
            continue;
        }
    }

    alpha = beta = gamma = 0;
    double *pangles[] = {&alpha, &beta, &gamma};
    for (int i = 0; i < 3; ++i) {
        try {
            if (angles.at(i) > 0) {
                *pangles[i] = angles.at(i);
            }
        } catch (const std::out_of_range &e) {
            continue;
        }
    }
}

void Triangle::calculateMissingAngles() {
    /* Implements the inverse cosine theorem to calculate missing angles. */
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
    /* It uses sum of angles to calculate third angle */
    if (alpha && beta && !gamma) {
        gamma = 180 - (alpha + beta);
    } else if (alpha && gamma && !beta) {
        beta = 180 - (alpha + gamma);
    } else if (beta && gamma && !alpha) {
        alpha = 180 - (beta + gamma);
    }
}

bool Triangle::calculateMissingFront() {
    /* Use the inverse cosine theorem to calculate missing front */
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
    /* It returns true, if the triangle has 2 same fronts.*/
    if (alpha && alpha == beta)
        return true;
    else if (beta && beta == gamma)
        return true;
    else if (alpha && alpha == gamma)
        return true;
    return false;
}

bool Triangle::isIsoscelesByFronts() {
    /* It returns true, if the triangle has 2 same angles.*/
    if (a && a == b)
        return true;
    else if (b && b == c)
        return true;
    else if (a && a == c)
        return true;
    return false;
}

bool Triangle::isIsosceles() {
    /* It returns true, if the triangle is isoscale.*/
    return isIsoscelesByAngles() || isIsoscelesByFronts();
}

void Triangle::calculateSquare() {
    /* Calculates the triangle square by the Geron's formula. */
    if (a > 0 and b > 0 and c > 0) {
        double p = (a + b + c) / 2;
        square =
            kRound(sqrt(p * (p - a) * (p - b) * (p - c)), fronts_precision);
    }
}

bool Triangle::isEquilateralByFronts() { return (a == b) && (a == c); }

bool Triangle::isEquilaterialByAngles() {
    /* Returns true, if the triangle has angles equals 60 degrees. */
    return (alpha == beta) and (alpha == gamma) and alpha == 60;
}

bool Triangle::isEquilateral() {
    /* Returns true, if the triangle is equilateral. */
    return isEquilateralByFronts() || isEquilaterialByAngles();
}

bool Triangle::isRectangular() {
    /* Returns true, if the triangle is rectangular by the angle.*/
    return alpha == 90 || beta == 90 || gamma == 90;
}

bool Triangle::isValidRectangularTriangle() {
    /* Uses the Pythagorean theorem to check a rectangular triangle.*/
    if (alpha == 90 || beta == 90 || gamma == 90) {
        if (alpha == 90 && round(a * a) != round(b * b + c * c))
            return false;
        if (beta == 90 && round(b * b) != round(a * a + c * c))
            return false;
        if (gamma == 90 && round(c * c) != round(a * a + b * b))
            return false;
        return true;
    } else {
        return false;
    }
}

void Triangle::calculateCircumscribedCircleRadius() {
    /* Returns true, if the triangle has angles equals 60 degrees. */
    if (a > 0 && b > 0 && c > 0 && square > 0) {
        circumscribedCircleRadius =
            kRound((a * b * c) / (4 * square), fronts_precision);
    }
}

void Triangle::calculateInscribedCircleRadius() {
    /* Calculate the inscribed circle radius from the sides and square. */
    if (a > 0 && b > 0 && c > 0 && square > 0) {
        inscribedCircleRadius =
            kRound(square / ((a + b + c) / 2), fronts_precision);
    }
}

void Triangle::fillIsoscalesTriangle() {
    /* Implements the isoscale triangle properties */
    using std::max;
    if (a and b and c and not isValidFronts())
        return;

    // fill a missing angle
    if (a == b && a && max(alpha, beta) != 90) {
        if (alpha && beta)
            return;
        alpha = beta = max(alpha, beta);
    }
    if (a == c && a && max(alpha, gamma) != 90) {
        if (alpha && gamma)
            return;
        alpha = gamma = max(alpha, gamma);
    }
    if (b == c && b && max(beta, gamma) != 90) {
        if (beta && gamma)
            return;
        beta = gamma = max(beta, gamma);
    }

    // fill a missing front
    if (alpha == beta && alpha) {
        if (a && b)
            return;
        a = b = max(a, b);
    }
    if (alpha == gamma && alpha) {
        if (a && c)
            return;
        a = c = max(a, c);
    }
    if (beta == gamma && beta) {
        if (b && c)
            return;
        b = c = max(b, c);
    }
}

void Triangle::calcSideOpposite30Angle(double *hypotenuse, double *catet) {
    /* Add catet or hypotenuse, if the rectangular triangle has an angle equals
     * 30 degrees.*/
    if (*hypotenuse <= 0 and *catet > 0) {
        *hypotenuse = 2 * (*catet);
    } else if (*catet <= 0 and *hypotenuse > 0) {
        *catet = *hypotenuse / 2;
    }
}

double *Triangle::getSideOppositeAngle(double angle) {
    /* Returns side, which lying opposite the angle */
    double *sides[] = {&a, &b, &c};
    double *angles[] = {&alpha, &beta, &gamma};
    for (int i = 0; i < 3; ++i) {
        if (*angles[i] == angle) {
            return sides[i];
        }
    }
    return NULL;
}

void Triangle::fillRectangularTriangle() {
    /* Implements rectangular triangle properties */
    if (not isValidAngles())
        return;

    // a cathet opposite of 30 degrees angle equals half past of a hypotenuse
    if (isRectangular() and getSideOppositeAngle(30) != NULL) {
        double *hypotenuse = getSideOppositeAngle(90);
        double *catet = getSideOppositeAngle(30);
        calcSideOpposite30Angle(hypotenuse, catet);
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
    /* Realization of triangle angles sum for map */
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
    /* Realization of triangle angles sum for private fields */
    double sum = alpha + beta + gamma;
    int quantity = anglesQuantity();
    if (round(sum) == 180 && quantity == 3)
        return true;
    else if (sum < 180 && quantity < 3)
        return true;
    else {
        return false;
    }
}

bool Triangle::validFronts(QMap<int, double> fronts) {
    /* Implements the inequality of the parties (QMap) */
    if (!fronts.contains(0) || !fronts.contains(1) || !fronts.contains(2)) {
        return false;
    }
    double sideA = fronts[0];
    double sideB = fronts[1];
    double sideC = fronts[2];
    if (sideA >= sideB + sideC)
        return false;
    if (sideC >= sideA + sideB)
        return false;
    if (sideB >= sideA + sideC)
        return false;
    return true;
}

bool Triangle::isValidFronts() {
    /* Implements the inequality of the parties */
    if (a >= b + c)
        return false;
    if (c >= a + b)
        return false;
    if (b >= a + c)
        return false;
    return true;
}

int Triangle::frontsQuantity() { return (a > 0) + (b > 0) + (c > 0); }

int Triangle::anglesQuantity() {
    return (alpha > 0) + (beta > 0) + (gamma > 0);
}

QMap<int, double> Triangle::anglesAsMap() {
    /* Packs all correct angles to map. */
    QMap<int, double> angles;
    if (alpha > 0)
        angles.insert(0, alpha);
    if (beta > 0)
        angles.insert(1, beta);
    if (gamma > 0)
        angles.insert(2, gamma);
    return angles;
}

QMap<int, double> Triangle::frontsAsMap() {
    /* Packs all correct sides to map. */
    QMap<int, double> fronts;
    if (a > 0)
        fronts.insert(0, a);
    if (b > 0)
        fronts.insert(1, b);
    if (c > 0)
        fronts.insert(2, c);
    return fronts;
}

std::vector<double> Triangle::frontsAsVector() {
    return {a > 0 ? a : 0, b > 0 ? b : 0, c > 0 ? c : 0};
}

std::vector<double> Triangle::anglesAsVector() {
    return {alpha > 0 ? alpha : 0, beta > 0 ? beta : 0, gamma > 0 ? gamma : 0};
}

void Triangle::roundFields() {
    /* Rounds fronts and angles, while they are correct. */
    using std::pow;

    auto decimalFronts = frontsAsMap();
    auto decimalAngles = anglesAsMap();
    bool wasValid = isValidTriangle();
    bool wasRectangular = isValidRectangularTriangle();

    if (not isValidAngles())
        return;

    auto rounding = [this](int frontsPrecision, int anglesPrecision) {
        kRound(&a, frontsPrecision);
        kRound(&b, frontsPrecision);
        kRound(&c, frontsPrecision);
        kRound(&alpha, anglesPrecision);
        kRound(&beta, anglesPrecision);
        gamma = 180 - (alpha + beta);
    };
    rounding(fronts_precision, angles_precision);

    if (not isValidRectangularTriangle() && wasRectangular) {
        for (int d = 0; d < kmaxDecimal && !isValidRectangularTriangle();
             ++d) {
            unpackFromMap(decimalFronts, decimalAngles);
            rounding(d, d);
            fronts_precision = angles_precision = d;
        }
    }
    if (not isValidTriangle() && wasValid) {
        for (int d = 0; d < kmaxDecimal && !isValidTriangle(); ++d) {
            unpackFromMap(decimalFronts, decimalAngles);
            rounding(d, d);
            fronts_precision = angles_precision = d;
        }
    }
}

void Triangle::calculateProperties() {
    /* Calculate the square, inscribed circle radius, circumscribed circle
     * radius for the triangle. */
    calculateSquare();
    calculateCircumscribedCircleRadius();
    calculateInscribedCircleRadius();
}

bool Triangle::isValidTriangle() {
    /* Returns true, if the triangle can exist.
     * It was implemeted by the Sine theorem.*/

    if (not isValidFronts())
        return false;
    if (not a || not b || not c || not alpha || not beta || not gamma)
        return false;
    if (not isValidAngles())
        return false;
    if (isRectangular() != isValidRectangularTriangle())
        return 0;
    if (isEquilateralByFronts() != isEquilaterialByAngles()) {
        return false;
    }

    double Avalue = a / sin(toRadians(alpha));
    double Bvalue = b / sin(toRadians(beta));
    double Cvalue = c / sin(toRadians(gamma));

    if (round(Avalue) == round(Bvalue) && round(Avalue) == round(Cvalue)) {
        return true;
    }
    return false;
}

void Triangle::print(std::string stepName, bool bigSeparator, bool debugOnly) {
    return;
    auto separator = bigSeparator ? "\n--------------------------" : "---";
#ifdef qDebug
    if (debugOnly) {
        qDebug() << "Step: " << stepName.c_str();
        qDebug() << "Triangle sides: (" << a << ';' << b << ';' << c << ')';
        qDebug() << "Triangle angles: (" << alpha << ';' << beta << ';'
                 << gamma << ')';
        qDebug() << "S=" << square << "; r=" << inscribedCircleRadius
                 << "; R=" << circumscribedCircleRadius;
        qDebug() << (isValidTriangle() ? "It can exist" : "It can't exist");
        qDebug() << separator;
    } else {
        qInfo() << "Step: " << stepName.c_str();
        qInfo() << "Triangle sides: (" << a << ';' << b << ';' << c << ')';
        qInfo() << "Triangle angles: (" << alpha << ';' << beta << ';' << gamma
                << ')';
        qInfo() << "S=" << square << "; r=" << inscribedCircleRadius
                << "; R=" << circumscribedCircleRadius;
        qInfo() << (isValidTriangle() ? "It can exist" : "It can't exist");
        qInfo() << separator;
    }
#else
    if (not debugOnly) {
        std::cout << "Step: " << stepName.c_str() << std::endl;
        std::cout << "Triangle sides: (" << a << ';' << b << ';' << c << ')'
                  << std::endl;
        std::cout << "Triangle angles: (" << alpha << ';' << beta << ';'
                  << gamma << ')' << std::endl;
        std::cout << "S=" << square << "; r=" << inscribedCircleRadius
                  << "; R=" << circumscribedCircleRadius << std::endl;
        std::cout << (isValidTriangle() ? "It can exist" : "It can't exist")
                  << std::endl;
        std::cout << separator << std::endl;
    }
#endif
}

std::vector<double> Triangle::shuffleArray(std::vector<double> array) {
    std::random_device rd;
    auto rng = std::default_random_engine(rd());
    std::shuffle(array.begin(), array.end(), rng);
    return array;
}

std::vector<uint64_t> Triangle::generatePythogoreanThree(int minBorder,
                                                         int maxBorder) {
    uint64_t n3 = 0;
    uint64_t a = 2;
    std::list<std::vector<uint64_t>> variants = {};
    while (n3 < (uint64_t)maxBorder) {
        for (uint64_t b = 1; b <= a; b++) {
            uint64_t n1 = a * a - b * b;
            uint64_t n2 = 2 * a * b;
            n3 = a * a + b * b;
            if (n3 > (uint64_t)maxBorder)
                break;
            if (n3 < (uint64_t)minBorder)
                continue;
            if (n1 == 0 or n2 == 0 or n3 == 0)
                break;
            if (std::min(n1, std::min(n2, n3)) < (uint64_t)minBorder)
                continue;
            variants.push_back({n1, n2, n3});
        }
        a = a + 1;
    }
    if (variants.size() > 0) {
        uint64_t resultIndx = rand(0, variants.size() - 1);
        auto result = variants.begin();
        std::advance(result, resultIndx);
        return *result;
    } else {
        return {};
    }
}

void Triangle::generate(double minSide, double maxSide, bool isRectangular,
                        bool isoscales, bool equileterial, bool preferInt) {
    bool preferFloat = !preferInt;
    if (isRectangular) {
        if (isoscales) {
            a = b = rand(minSide, maxSide, preferFloat);
            c = sqrt(a * a + b * b);
            unpackFromVectors(shuffleArray(frontsAsVector()),
                              anglesAsVector());
        } else {
            std::vector<uint64_t> p3inThisRange;
            if (preferInt) {
                p3inThisRange = generatePythogoreanThree(minSide, maxSide);
            }
            if (preferInt && p3inThisRange.size() == 3) {
                a = p3inThisRange[0];
                b = p3inThisRange[1];
                c = p3inThisRange[2];
            } else {
                a = rand(minSide, maxSide, preferFloat);
                b = rand(minSide, maxSide, preferFloat);
                c = sqrt(a * a + b * b);
            }
        }
        unpackFromVectors(shuffleArray(frontsAsVector()), anglesAsVector());
    } else if (isoscales) {
        a = b = rand(minSide, maxSide, preferFloat);
        c = rand(minSide, a + b - minSide * 0.001, preferFloat);
        unpackFromVectors(shuffleArray(frontsAsVector()), anglesAsVector());
    } else if (equileterial) {
        a = b = c = rand(minSide, maxSide, preferFloat);
        alpha = beta = gamma = 60;
    } else {
        for (uint64_t limit = std::pow(10, 5); limit > 0; --limit) {
            a = rand(minSide, maxSide, preferFloat);
            b = rand(minSide, maxSide, preferFloat);
            c = rand(minSide, maxSide, preferFloat);
            if (isValidFronts()) {
                break;
            }
        }
    }
    addMissingInformation(frontsAsMap(), anglesAsMap(), false);
}
