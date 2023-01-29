#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QShortcut>
#include <QGroupBox>
#include <QPushButton>
#include <QSlider>
#include <QResource>

#include "NGonFigure.hpp"
#include "canvas.hpp"

class Window : public QWidget
{
public:
    Window(QWidget *parent = nullptr);
    ~Window();
private:
    void setupIcon();
    QTableWidget *table;
    NGonFigure *ngonfigure;
    Canvas *canvas_;
};
#endif // WINDOW_H
