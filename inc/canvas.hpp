#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <QWidget>
#include <QPainter>
#include <QGraphicsWidget>
#include <cmath>

#include "BaseFigure.hpp"
#include "triangle.hpp"

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
    QRectF posAtCenter(QRectF rect);

    enum FType {FTriangle, FQuadrilateral, noData};
    FType currentFigure = noData;
    Triangle triangle;

    static constexpr int captionMarginBottom = 45;
};

#endif // CANVAS_HPP
