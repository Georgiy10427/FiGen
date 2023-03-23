#include "canvas.hpp"

Canvas::Canvas(QWidget *parent) : QWidget(parent) {
    this->setMinimumWidth(250);
    this->setMinimumHeight(100);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

QRectF Canvas::posAtCenter(QRectF rect) {
    return QRectF(this->size().width() / 2 - rect.width() / 2,
                  this->size().height() / 2 - rect.height() / 2, rect.width(),
                  rect.height());
}

void Canvas::paintEvent(QPaintEvent *) {
    if (currentFigure == ErrorType::FTriangle) {
        drawTriangle();
    } else {
        drawEmpty();
    }
}

void Canvas::setCurrentFigure(Triangle t) {
    this->current_triangle = t;
    currentFigure = ErrorType::FTriangle;
    update();
}

QPoint Canvas::rotatePoint(QPoint origin, double angle, QPoint point) {
    double s = std::sin(toRadians(angle));
    double c = std::cos(toRadians(angle));

    point.setX(point.x() - origin.x());
    point.setY(point.y() - origin.y());

    double xnew = point.x() * c - point.y() * s;
    double ynew = point.x() * s + point.y() * c;

    point.setX(round(xnew + origin.x()));
    point.setY(round(ynew + origin.y()));
    return point;
}

QPolygon Canvas::getTriangleGeometry(QSize canvasSize, Triangle triangle,
                                     double scale) {
    using std::min, std::max;
    double maxLineLength, maxSideLength, scaleFactor, aFrontMargin,
        marginBottom;
    QPoint firstPoint, secondPoint, thirdPoint, forthPoint;

    // calculate scaleFactor and margins
    maxLineLength = min(canvasSize.width(), canvasSize.height()) * scale;
    maxSideLength = max(triangle.a, max(triangle.b, triangle.c));
    scaleFactor = maxLineLength / maxSideLength;
    aFrontMargin = (canvasSize.width() - triangle.a * scaleFactor) / 2;
    marginBottom =
        canvasSize.height() * 0.65; // get 66 percents as the margin bottom

    // place 3 points
    firstPoint = QPoint(aFrontMargin, marginBottom);
    secondPoint =
        QPoint(aFrontMargin + triangle.a * scaleFactor, marginBottom);
    thirdPoint =
        QPoint(secondPoint.x() - triangle.b * scaleFactor, marginBottom);
    forthPoint = QPoint(aFrontMargin + triangle.c * scaleFactor, marginBottom);

    // rotate B side
    thirdPoint = rotatePoint(secondPoint, triangle.gamma, thirdPoint);
    forthPoint = rotatePoint(firstPoint, -triangle.beta, forthPoint);

    return QPolygon({firstPoint, secondPoint, thirdPoint, forthPoint});
}

QList<QLine> Canvas::getTriangleShape(QSize canvasSize, Triangle triangle,
                                      double scale) {
    using std::min, std::max;
    double maxLineLength, maxSideLength, scaleFactor, aFrontMargin,
        marginBottom;
    QPoint firstPoint, secondPoint, thirdPoint, forthPoint;

    // calculate scaleFactor and margins
    maxLineLength = min(canvasSize.width(), canvasSize.height()) * scale;
    maxSideLength = max(triangle.a, max(triangle.b, triangle.c));
    scaleFactor = maxLineLength / maxSideLength;
    aFrontMargin = round((canvasSize.width() - triangle.a * scaleFactor) / 2);
    marginBottom = round(canvasSize.height() *
                         0.65); // get 66 percents as the margin bottom

    // place 3 points
    firstPoint = QPoint(aFrontMargin, marginBottom);
    secondPoint =
        QPoint(round(aFrontMargin + triangle.a * scaleFactor), marginBottom);
    thirdPoint = QPoint(round(secondPoint.x() - triangle.b * scaleFactor),
                        marginBottom);
    forthPoint =
        QPoint(round(aFrontMargin + triangle.c * scaleFactor), marginBottom);

    // rotate B side
    thirdPoint = rotatePoint(secondPoint, triangle.gamma, thirdPoint);
    forthPoint = rotatePoint(firstPoint, -triangle.beta, forthPoint);

    return QList<QLine>({QLine(firstPoint, secondPoint),
                         QLine(secondPoint, thirdPoint),
                         QLine(firstPoint, forthPoint)});
}

int Canvas::distance(QPoint p1, QPoint p2) {
    using std::sqrt, std::pow;
    return round(sqrt(pow(p2.x() - p1.x(), 2) + pow(p2.y() - p1.y(), 2)));
}

void Canvas::drawTriangle() {
    QPainter painter(this);
    QPen pen = QPen(QPalette().color(QPalette::Highlight), 3);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setFont(QFont("Droid Sans", 15, 600, false));
    painter.setPen(pen);

    QPolygon trianglePoints = getTriangleGeometry(size(), current_triangle);
    painter.drawLines(getTriangleShape(size(), current_triangle));
    painter.drawEllipse(trianglePoints[0], 2, 2);
    painter.drawEllipse(trianglePoints[1], 2, 2);
    painter.drawEllipse(trianglePoints[2], 2, 2);

    pen.setColor(QPalette().color(QPalette::WindowText));
    painter.setPen(pen);

    // drawing points name
    QPointF captionA = trianglePoints[2], captionB = trianglePoints[0],
            captionC = trianglePoints[1];
    captionA.setX(trianglePoints[2].x() - labelsMargin);
    captionA.setY(trianglePoints[2].y() - labelsMargin);

    captionB.setX(trianglePoints[0].x() - labelsMargin * 3.2);
    captionB.setY(trianglePoints[0].y() + labelsMargin);

    captionC.setX(trianglePoints[1].x() + labelsMargin);
    captionC.setY(trianglePoints[1].y() + labelsMargin + labelsMargin / 4);

    painter.drawText(captionA, "A");
    painter.drawText(captionB, "B");
    painter.drawText(captionC, "C");

    int captionYPosition = trianglePoints[2].y() - captionMarginBottom;

    QRect titlePosition = rect();
    titlePosition.setY(captionYPosition);
    painter.drawText(titlePosition, Qt::AlignHCenter,
                     getTriangleTypeName(current_triangle));

    QString squarePropertyCaption;
    if (current_triangle.isValidTriangle()) {
        QString squareCaptionTemplate = size().height() > 250
                                            ? "S = %1\nr = %2\nR = %3"
                                            : "S = %1; r = %2; R = %3";
        squarePropertyCaption =
            QString(squareCaptionTemplate)
                .arg(current_triangle.square)
                .arg(current_triangle.inscribedCircleRadius)
                .arg(current_triangle.circumscribedCircleRadius);
    } else {
        squarePropertyCaption = "Это не треугольник.";
    }
    painter.drawText(QRectF(titlePosition.x(), trianglePoints[0].y() + 15,
                            titlePosition.width(), titlePosition.height()),
                     Qt::AlignHCenter, squarePropertyCaption);
}

void Canvas::setCurrentFigureToEmpty() {
    this->currentFigure = ErrorType::noData;
    update();
}

void Canvas::drawEmpty() {
    QPainter painter(this);
    QPen pen = QPen(QPalette().color(QPalette::Highlight), 3);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setFont(QFont("Droid Sans", 15, 600, false));
    painter.setPen(pen);

    QPolygon trianglePoints =
        getTriangleGeometry(size(), Triangle(std::vector<double>({79, 71, 85}),
                                             std::vector<double>()));
    painter.drawLines(
        getTriangleShape(size(), Triangle(std::vector<double>({79, 71, 85}),
                                          std::vector<double>())));
    painter.drawEllipse(trianglePoints[0], 2, 2);
    painter.drawEllipse(trianglePoints[1], 2, 2);
    painter.drawEllipse(trianglePoints[2], 2, 2);

    pen.setColor(QPalette().color(QPalette::WindowText));
    painter.setPen(pen);

    // drawing points name
    QPointF captionA = trianglePoints[2], captionB = trianglePoints[0],
            captionC = trianglePoints[1];
    captionA.setX(trianglePoints[2].x() - labelsMargin);
    captionA.setY(trianglePoints[2].y() - labelsMargin);

    captionB.setX(trianglePoints[0].x() - labelsMargin * 3.2);
    captionB.setY(trianglePoints[0].y() + labelsMargin);

    captionC.setX(trianglePoints[1].x() + labelsMargin);
    captionC.setY(trianglePoints[1].y() + labelsMargin + labelsMargin / 4);

    painter.drawText(captionA, "A");
    painter.drawText(captionB, "B");
    painter.drawText(captionC, "C");

    int captionYPosition = trianglePoints[2].y() - captionMarginBottom;

    QRect titlePosition = rect();
    titlePosition.setY(captionYPosition);
    painter.drawText(titlePosition, Qt::AlignHCenter, "Недостаточно данных");
}

QString Canvas::getTriangleTypeName(Triangle triangle) {
    const QString arbitrary = "произвольный", isoscales = "равнобедренный",
                  equileterial = "равносторонний",
                  rectangular = "прямоугольный", basename = "треугольник";
    QString result;
    if (triangle.isRectangular()) {
        result += rectangular + " ";
    }
    if (not triangle.isRectangular() or !(size().width() < 500)) {
        if (triangle.isEquilateral()) {
            result += equileterial + " ";
        } else if (triangle.isIsosceles()) {
            result += isoscales + " ";
        } else {
            result += arbitrary + " ";
        }
    }
    result += basename;
    result[0] = result[0].toUpper();
    return result;
}
