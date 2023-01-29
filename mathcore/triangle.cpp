#include "triangle.hpp"

// Добавить функцию для проверки существования треугольника с такими сторонами и углами

Triangle::Triangle()
{

}

Triangle::Triangle(QMap<int, double> fronts, QMap<int, double> angles)
{
    unpackFromMap(fronts, angles);
    addMissingInformation(fronts, angles);
}

void Triangle::addMissingInformation(QMap<int, double> fronts, QMap<int, double> angles)
{
    fillIsoscalesTriangle();
    fillRectangularTriangle();

    if(fronts.size() == 3 && validFronts(fronts))
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
        kRound(&alpha, kAnglesDecimal);
    }
    if(!beta)
    {
        this->beta = toDegrees(
            acos( (a*a+c*c-b*b)/(2*a*c) )
        );
        kRound(&beta, kAnglesDecimal);
    }
    if(!gamma)
    {
        this->gamma = toDegrees(
            acos( (a*a+b*b-c*c)/(2*a*b) )
        );
        kRound(&gamma, kAnglesDecimal);
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
        qDebug("c");
        c = sqrt( a*a+b*b-2*a*b * cos(toRadians(gamma)) );
        kRound(&c);
        return true;
    }
    else if(a && c && !b && beta)
    {
        qDebug("b");
        b = sqrt( a*a+c*c-2*a*c * cos(toRadians(beta)) );
        kRound(&b);
        return true;
    }
    else if(b && c && !a && alpha)
    {
        qDebug("a");
        a = sqrt(b*b+c*c-2*b*c * cos(toRadians(alpha)));
        kRound(&a);
        return true;
    }
    return false;
}


bool Triangle::isIsoscelesByAngles()
{
    if((alpha && beta) && alpha == beta) return true;
    else if((beta && gamma) && beta == gamma) return true;
    else if((alpha && gamma) && alpha == gamma) return true;
    return false;
}

bool Triangle::isIsoscelesByFronts()
{
    if((a && b) && a == b) return true;
    else if((b && c) && b == c) return true;
    else if((a && c) && a == c) return true;
    return false;
}

bool Triangle::isIsosceles()
{
    return isIsoscelesByAngles() || isIsoscelesByFronts();
}

void Triangle::calculateSquare()
{
    if(a && b && c)
    {
        double p = (a+b+c)/2;
        square = round(sqrt(p*(p-a)*(p-b)*(p-c))*100)/100;
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
    if(isIsoscelesByFronts())
    {
        // fill a missing angle
        if(a == b && a && std::max(alpha, beta) != 90) alpha = beta = std::max(alpha, beta);
        if(a == c && a && std::max(alpha, gamma) != 90) alpha = gamma = std::max(alpha, gamma);
        if(b == c && b && std::max(beta, gamma) != 90) beta = gamma = std::max(beta, gamma);
    }
    else if(isIsoscelesByAngles() && validAvailableAngles(anglesAsMap()))
    {
        // fill a missing front
        if(alpha == beta && alpha) a = b = std::max(a, b);
        if(alpha == gamma && alpha) a = c = std::max(a, c);
        if(beta == gamma && beta) b = c = std::max(b, c);
    }
}

void Triangle::fillRectangularTriangle()
{
    if(validAvailableAngles(anglesAsMap()))
    {
        // find a hypotenuse by the Pythagorean theorem
        if(alpha == 90 && b && c)
        {
            a = kRound(sqrt(b*b+c*c));
        }
        else if(beta == 90 && a && c)
        {
            b = kRound(sqrt(a*a+c*c));
        }
        else if(gamma == 90 && a && b)
        {
            c = kRound(sqrt(a*a+b*b));
        }

        // find a cathet by the Pythagorean theorem
        if(alpha == 90 && a)
        {
            if(b) c = kRound(sqrt(a*a-b*b));
            else if (c) b = kRound(sqrt(a*a-c*c));
        }
        else if(beta == 90 && b)
        {
            if(a) c = kRound(sqrt(b*b-a*a));
            else if(c) a = kRound(sqrt(b*b-c*c));
        }
        else if(gamma == 90 && c)
        {
            if(b) a = kRound(sqrt(c*c-b*b));
            else if(a) b = kRound(sqrt(c*c-a*a));
        }

        // a cathet opposite of a 30 degrees angle equals half past of a hypotenuse
        if(alpha == 90 && a)
        {
            if(gamma == 30) c = a/2;
            else if(beta == 30) b = a/2;
        }
        else if(gamma == 90 && c)
        {
            if(beta == 30) b = c/2;
            else if(alpha == 30) a = c/2;
        }
        else if(beta == 90 && b)
        {
            if(alpha == 30) a = b/2;
            if(gamma == 30) c = b/2;
        }
    }
}


bool Triangle::validAvailableAngles(QMap<int, double> angles)
{
    bool existStraightAngle = false;
    for(auto angle : angles.values())
    {
        if(angle >= 90)
        {
            if(existStraightAngle)
            {
                return false;
            } else {
                existStraightAngle = true;
            }
        }
    }
    return true;
}

bool Triangle::validFronts(QMap<int, double> fronts)
{
    a = fronts[0];
    b = fronts[1];
    c = fronts[2];
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
