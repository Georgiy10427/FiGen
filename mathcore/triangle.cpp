#include "triangle.hpp"


Triangle::Triangle()
{

}

Triangle::Triangle(QMap<int, double> fronts, QMap<int, double> angles, int fronts_precision, int angles_precision)
{
    this->fronts_precision = fronts_precision;
    this->angles_precision = angles_precision;
    unpackFromMap(fronts, angles);
    addMissingInformation(fronts, angles);
}

void Triangle::addMissingInformation(QMap<int, double> fronts, QMap<int, double> angles)
{
    if(isValidFronts())
    {
        fillMissingAngle();
    }

    fillRectangularTriangle();
    fillIsoscalesTriangle();

    fronts = frontsAsMap();
    angles = anglesAsMap();

    if(fronts.size() == 3 && isValidFronts())
    {
        if(angles.size() == 2 && validAvailableAngles(angles))
        {
            fillMissingAngle();
        }
        else if(angles.size() <= 1)
        {
            calculateMissingAngles();
        }
        calculateMissingAngles();
        calculateSquare();
        calculateInscribedCircleRadius();
        calculateCircumscribedCircleRadius();
    }
    else if(fronts.size() == 2 && validAvailableAngles(angles))
    {
        if(angles.size() == 2)
        {
            fillMissingAngle();
        }
        if(calculateMissingFront())
        {
            calculateMissingAngles();
        }
    }

    if(!validAvailableAngles(anglesAsMap()) || !validFronts(frontsAsMap()))
    {
        unpackFromMap(fronts, angles);
    } else {
        kRound(&alpha, angles_precision);
        kRound(&beta, angles_precision);
        gamma = 180-(alpha+beta);
        kRound(&a, fronts_precision);
        kRound(&b, fronts_precision);
        kRound(&c, fronts_precision);
    }
}

void Triangle::unpackFromMap(QMap<int, double> fronts, QMap<int, double> angles)
{
    alpha = 0; beta = 0; gamma = 0;
    if(angles.contains(0)) this->alpha = angles[0];
    if(angles.contains(1)) this->beta = angles[1];
    if(angles.contains(2)) this->gamma = angles[2];

    a = 0; b = 0; c = 0;
    if(fronts.contains(0)) this->a = fronts[0];
    if(fronts.contains(1)) this->b = fronts[1];
    if(fronts.contains(2)) this->c = fronts[2];
}

void Triangle::calculateMissingAngles()
{
    fillMissingAngle();
    if(!alpha)
    {
        this->alpha = toDegrees(
            acos( (b*b+c*c-a*a)/(2*b*c) )
        );
    }
    if(!beta)
    {
        this->beta = toDegrees(
            acos( (a*a+c*c-b*b)/(2*a*c) )
        );
    }
    if(!gamma)
    {
        this->gamma = toDegrees(
            acos( (a*a+b*b-c*c)/(2*a*b) )
        );
    }
}

void Triangle::fillMissingAngle()
{
    if(alpha && beta && !gamma)
    {
        gamma = 180-(alpha+beta);
    }
    else if(alpha && gamma && !beta)
    {
        beta = 180-(alpha+gamma);
    }
    else if(beta && gamma && !alpha)
    {
        alpha = 180-(beta+gamma);
    }
}

bool Triangle::calculateMissingFront()
{
    if(a && b && !c && gamma)
    {
        c = sqrt( a*a+b*b-2*a*b * cos(toRadians(gamma)) );
        return true;
    }
    if(a && c && !b && beta)
    {
        b = sqrt( a*a+c*c-2*a*c * cos(toRadians(beta)) );
        return true;
    }
    if(b && c && !a && alpha)
    {
        a = sqrt(b*b+c*c-2*b*c * cos(toRadians(alpha)));
        return true;
    }
    return false;
}


bool Triangle::isIsoscelesByAngles()
{
    if(alpha && alpha == beta) return true;
    else if(beta && beta == gamma) return true;
    else if(alpha && alpha == gamma) return true;
    return false;
}

bool Triangle::isIsoscelesByFronts()
{
    if(a && a == b) return true;
    else if(b && b == c) return true;
    else if(a && a == c) return true;
    return false;
}

bool Triangle::isIsosceles()
{
    return isIsoscelesByAngles() || isIsoscelesByFronts();
}

void Triangle::calculateSquare()
{
    if(a > 0 and b > 0 and c > 0)
    {
        double p = (a+b+c)/2;
        square = kRound(sqrt(p*(p-a)*(p-b)*(p-c)), fronts_precision);
    }
}

void Triangle::calculateCircumscribedCircleRadius()
{
    circumscribedCircleRadius = (a*b*c)/(4*square);
}

void Triangle::calculateInscribedCircleRadius()
{
    inscribedCircleRadius = square/( (a+b+c)/2);
}

void Triangle::fillIsoscalesTriangle()
{
    // to-do: refactor
    if(a and b and c and not isValidFronts()) return;
    auto user_fronts = frontsAsMap();
    auto user_angles = anglesAsMap();

    // fill a missing angle
    if(a == b && a && std::max(alpha, beta) != 90) alpha = beta = std::max(alpha, beta);
    if(a == c && a && std::max(alpha, gamma) != 90) alpha = gamma = std::max(alpha, gamma);
    if(b == c && b && std::max(beta, gamma) != 90) beta = gamma = std::max(beta, gamma);

    // fill a missing front
    if(alpha == beta && alpha) a = b = std::max(a, b);
    if(alpha == gamma && alpha) a = c = std::max(a, c);
    if(beta == gamma && beta) b = c = std::max(b, c);

    if(!isValidAngles())
    {
        unpackFromMap(user_fronts, user_angles);
    }
}

void Triangle::fillRectangularTriangle()
{
    if(validAvailableAngles(anglesAsMap()))
    {
        // a cathet opposite of a 30 degrees angle equals half past of a hypotenuse
        if(alpha == 90 && a)
        {
            if(gamma == 30 && !c) c = a/2;
            else if(beta == 30 && !b) b = a/2;
        }
        else if(gamma == 90 && c)
        {
            if(beta == 30 && !b) b = c/2;
            else if(alpha == 30 && !a) a = c/2;
        }
        else if(beta == 90 && b)
        {
            if(alpha == 30 && !a) a = b/2;
            if(gamma == 30 && !c) c = b/2;
        }

        // find a hypotenuse by the Pythagorean theorem
        if(alpha == 90 && b && c and !a)
        {
            a = kRound(sqrt(b*b+c*c), fronts_precision);
        }
        else if(beta == 90 && a && c and !b)
        {
            b = kRound(sqrt(a*a+c*c), fronts_precision);
        }
        else if(gamma == 90 && a && b and !c)
        {
            c = kRound(sqrt(a*a+b*b), fronts_precision);
        }

        // find a cathet by the Pythagorean theorem
        if(alpha == 90 && a)
        {
            if(b && !c) c = kRound(sqrt(a*a-b*b), fronts_precision);
            else if (c && !b) b = kRound(sqrt(a*a-c*c), fronts_precision);
        }
        else if(beta == 90 && b)
        {
            if(a && c) c = kRound(sqrt(b*b-a*a), fronts_precision);
            else if(c && a) a = kRound(sqrt(b*b-c*c), fronts_precision);
        }
        else if(gamma == 90 && c)
        {
            if(b && !a) a = kRound(sqrt(c*c-b*b), fronts_precision);
            else if(a && !b) b = kRound(sqrt(c*c-a*a), fronts_precision);
        }
    }
}


bool Triangle::validAvailableAngles(QMap<int, double> angles)
{
    double sum = 0;
    for(auto angle : angles.values())
    {
        sum += angle;
    }
    if(round(sum) == 180 && angles.size() == 3) return true;
    else if(sum < 180 && angles.size() < 3) return true;
    else {
        return false;
    }
}

bool Triangle::isValidAngles()
{
    double sum = alpha+beta+gamma;
    int quantity = (alpha > 0) + (beta > 0) + (gamma > 0);
    if(round(sum) == 180 && quantity == 3) return true;
    else if(sum < 180 && quantity < 3) return true;
    else {
        return false;
    }
}

bool Triangle::validFronts(QMap<int, double> fronts)
{
    // to-do: refactor
    a = fronts[0];
    b = fronts[1];
    c = fronts[2];
    if (a >= b+c) return false;
    if (c >= a+b) return false;
    if (b >= a+c) return false;
    return true;
}

bool Triangle::isValidFronts()
{
    if (a >= b+c) return false;
    if (c >= a+b) return false;
    if (b >= a+c) return false;
    return true;
}

QMap<int, double> Triangle::anglesAsMap()
{
    QMap<int, double> angles;
    if(alpha) angles.insert(0, alpha);
    if(beta) angles.insert(1, beta);
    if(gamma) angles.insert(2, gamma);
    return angles;
}

QMap<int, double> Triangle::frontsAsMap()
{
    QMap<int, double> fronts;
    if(a) fronts.insert(0, a);
    if(b) fronts.insert(1, b);
    if(c) fronts.insert(2, c);
    return fronts;
}
