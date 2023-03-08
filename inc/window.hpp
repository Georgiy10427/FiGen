#ifndef WINDOW_H
#define WINDOW_H

#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QResource>
#include <QShortcut>
#include <QSlider>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "NGonFigure.hpp"
#include "canvas.hpp"

class Window : public QWidget {
  public:
    Window(QWidget *parent = nullptr);
    ~Window();

  private:
    void setupIcon();
    void deleteTablePart();
    QTableWidget *table;
    NGonFigure *ngonfigure;
    Canvas *canvas_;
};
#endif // WINDOW_H
