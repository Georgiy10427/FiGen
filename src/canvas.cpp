#include "canvas.hpp"

Canvas::Canvas(QWidget *parent) : QWidget(parent)
{
    this->setMinimumWidth(250);
    this->setMinimumHeight(100);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

QRectF Canvas::posAtCenter(QRectF rect)
{
    return QRectF(
                this->size().width()/2 - rect.width()/2,
                this->size().height()/2 - rect.height()/2,
                rect.width(),
                rect.height()
    );
}

void Canvas::paintEvent(QPaintEvent *)
{
    if(currentFigure == FType::FTriangle)
    {
        drawTriangle();
    }
    else
    {
        drawEmpty();
    }
}

void Canvas::setCurrentFigure(Triangle t)
{
    this->current_triangle = t;
    currentFigure = FTriangle;
    update();
}

QPoint Canvas::rotatePoint(QPoint origin, double angle, QPoint point)
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

QPolygon Canvas::getTriangleGeometry(QSize canvasSize, Triangle triangle, double scale)
{
    using std::min, std::max;
    double maxLineLength, maxSideLength, scaleFactor, aFrontMargin, marginBottom;
    QPoint firstPoint, secondPoint, thirdPoint, forthPoint;

    // calculate scaleFactor and margins
    maxLineLength = min(canvasSize.width(), canvasSize.height())*scale;
    maxSideLength = max(triangle.a, max(triangle.b, triangle.c));
    scaleFactor = maxLineLength/maxSideLength;
    aFrontMargin = (canvasSize.width() - triangle.a*scaleFactor)/2;
    marginBottom = canvasSize.height()*0.65; // get 66 percents as the margin bottom

    // place 3 points
    firstPoint = QPoint(aFrontMargin, marginBottom);
    secondPoint = QPoint(aFrontMargin + triangle.a*scaleFactor, marginBottom);
    thirdPoint = QPoint(secondPoint.x() - triangle.b*scaleFactor, marginBottom);
    forthPoint = QPoint(aFrontMargin + triangle.c*scaleFactor, marginBottom);

    // rotate B side
    thirdPoint = rotatePoint(secondPoint, triangle.gamma, thirdPoint);
    forthPoint = rotatePoint(firstPoint, -triangle.beta, forthPoint);

    return QPolygon({firstPoint, secondPoint, thirdPoint, forthPoint});
}

QList<QLine> Canvas::getTriangleShape(QSize canvasSize, Triangle triangle, double scale)
{
    using std::min, std::max;
    double maxLineLength, maxSideLength, scaleFactor, aFrontMargin, marginBottom;
    QPoint firstPoint, secondPoint, thirdPoint, forthPoint;

    // calculate scaleFactor and margins
    maxLineLength = min(canvasSize.width(), canvasSize.height())*scale;
    maxSideLength = max(triangle.a, max(triangle.b, triangle.c));
    scaleFactor = maxLineLength/maxSideLength;
    aFrontMargin = (canvasSize.width() - triangle.a*scaleFactor)/2;
    marginBottom = canvasSize.height()*0.65; // get 66 percents as the margin bottom

    // place 3 points
    firstPoint = QPoint(aFrontMargin, marginBottom);
    secondPoint = QPoint(aFrontMargin + triangle.a*scaleFactor, marginBottom);
    thirdPoint = QPoint(secondPoint.x() - triangle.b*scaleFactor, marginBottom);
    forthPoint = QPoint(aFrontMargin + triangle.c*scaleFactor, marginBottom);

    // rotate B side
    thirdPoint = rotatePoint(secondPoint, triangle.gamma, thirdPoint);
    forthPoint = rotatePoint(firstPoint, -triangle.beta, forthPoint);

    return QList<QLine>({QLine(firstPoint, secondPoint),
                         QLine(secondPoint, thirdPoint),
                         QLine(firstPoint, forthPoint)});
}

int Canvas::distance(QPoint p1, QPoint p2)
{
    using std::sqrt, std::pow;
    return round(sqrt(pow(p2.x()-p1.x(), 2) + pow(p2.y()-p1.y(), 2)));
}

void Canvas::drawTriangle()
{
    QPainter painter(this);
    QPen pen = QPen(QPalette().color(QPalette::Highlight), 3);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setFont(QFont("Droid Sans", 14, 400, false));
    painter.setPen(pen);

    QPolygon trianglePoints = getTriangleGeometry(size(), current_triangle);
    painter.drawLines(getTriangleShape(size(), current_triangle));

    pen.setColor(QPalette().color(QPalette::WindowText));
    painter.setPen(pen);

    int captionYPosition = trianglePoints[2].y()/2;
    QRect titlePosition = rect();
    titlePosition.setY(captionYPosition);
    painter.drawText(titlePosition, Qt::AlignHCenter, "Треугольник");

    QString squarePropertyCaption;
    if(current_triangle.isValidTriangle())
    {
        squarePropertyCaption = QString("S = %1").arg(current_triangle.square);
    } else {
        squarePropertyCaption = "Некорректный треугольник.";
    }
    painter.drawText(
                QRectF(titlePosition.x(),
                       trianglePoints[0].y() + 15,
                       titlePosition.width(),
                       titlePosition.height()),
                Qt::AlignHCenter,
                squarePropertyCaption
    );
}

void Canvas::setCurrentFigureToEmpty()
{
    this->currentFigure = FType::noData;
    update();
}

void Canvas::drawEmpty()
{
    QPainter painter(this);
    QPen pen = QPen(QPalette().color(QPalette::Text), 3);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setFont(QFont("Droid Sans", 14, 10, false));
    painter.setPen(pen);
    painter.drawText(rect(), Qt::AlignCenter, "Не хватает данных\nдля отображения");
}
