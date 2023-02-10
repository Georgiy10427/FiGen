#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <QWidget>
#include <QPainter>
#include <QGraphicsWidget>
#include <cmath>

#include "BaseFigure.hpp"
#include "triangle.hpp"

struct Caption
{
    QRect pos;
    int alignmentFlag;
    QString text;
};

class Canvas : public QWidget, BaseFigure
{
public:
    Canvas(QWidget *parent=nullptr);
    void setCurrentFigure(Triangle t);
    void setCurrentFigureToEmpty();
private:
    void drawTriangle();
    void drawEmpty();
    void paintEvent(QPaintEvent *);
    QPoint rotatePoint(QPoint origin, double angle, QPoint point);
    QPolygon getTriangleGeometry(QSize canvasSize, Triangle triangle, double scale=0.45);
    QList<QLine> getTriangleShape(QSize canvasSize, Triangle triangle, double scale=0.45);
    QList<Caption> getTriangleCaptions(Triangle triangle, QPolygon polygon);
    QRectF posAtCenter(QRectF rect);
    int distance(QPoint p1, QPoint p2);

    enum FType {FTriangle, FQuadrilateral, noData};
    FType currentFigure = noData;
    Triangle current_triangle;

    static constexpr int captionMarginBottom = 45;
};

#endif // CANVAS_HPP
