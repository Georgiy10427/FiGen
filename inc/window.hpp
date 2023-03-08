#ifndef WINDOW_H
#define WINDOW_H

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QResource>
#include <QShortcut>
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
    QTableWidget *table;
    NGonFigure *ngonfigure;
    Canvas *canvas;
    QCheckBox *intRandGeneration, *isEquileterialChk, *isIsoscalesChk,
        *isRectangularChk;
    QDoubleSpinBox *minRandSpinbox, *maxRandSpinbox;

  private slots:
    void deleteSelectedCells();
    void validateRandGenerationRange(double value);
    void validateRandGenerationProperties(int state);
    void generateTriangle();
};
#endif // WINDOW_H
