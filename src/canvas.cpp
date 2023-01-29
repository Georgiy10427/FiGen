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
    this->triangle = t;
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

void Canvas::drawTriangle()
{
    QPainter painter(this);
    QPen pen = QPen(QPalette().color(QPalette::Highlight), 3);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setFont(QFont("Droid Sans", 14, 600, false));
    painter.setPen(pen);

    double scaleFactor = std::min(this->size().width(), this->size().height())*scale;
    double maxFrontWidth = std::max(triangle.a, std::max(triangle.b, triangle.c));
    scaleFactor /= maxFrontWidth;
    double triangleHeight;
    double y_center = size().height()/2;
    double aFrontMargin = (size().width() - triangle.a*scaleFactor)/2;

    QPoint firstPoint = QPoint(aFrontMargin, y_center);
    QPoint secondPoint = QPoint(aFrontMargin + triangle.a*scaleFactor, y_center);
    QPoint thirdPoint = QPoint(secondPoint.x() - triangle.b*scaleFactor, y_center);

    triangleHeight = thirdPoint.y() - firstPoint.y()/2;
    thirdPoint = rotatePoint(secondPoint, triangle.gamma, thirdPoint);

    // Shift triangle to center by Y
    firstPoint += QPoint(-triangle.a/2, triangleHeight/2);
    secondPoint += QPoint(-triangle.a/2, triangleHeight/2);
    thirdPoint += QPoint(-triangle.a/2, triangleHeight/2);

    painter.drawPolygon(QPolygon({firstPoint, secondPoint, thirdPoint}));

    pen.setColor(QPalette().color(QPalette::Text));
    painter.setPen(pen);
    int captionYPosition = thirdPoint.y()/2;
    QRect titlePosition = rect();
    titlePosition.setY(captionYPosition);
    painter.drawText(titlePosition, Qt::AlignHCenter, "Треугольник");
    painter.drawText(
                QRectF(titlePosition.x(),
                       firstPoint.y() + 15,
                       titlePosition.width(),
                       titlePosition.height()),
                Qt::AlignHCenter,
                QString("S = %1").arg(triangle.square)
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
