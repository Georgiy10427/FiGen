#ifndef WINDOW_H
#define WINDOW_H

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QResource>
#include <QShortcut>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "TableDispatcher.hpp"
#include "canvas.hpp"

class Window : public QWidget {
  public:
    Window(QWidget *parent = nullptr);
    ~Window();

  private:
    void setupIcon();
    QTableWidget *table;
    TableDispatcher *tableDispatcher;
    Canvas *canvas;
    QCheckBox *intRandGeneration, *isEquileterialChk, *isIsoscalesChk,
        *isRectangularChk;
    QDoubleSpinBox *minRandSpinbox, *maxRandSpinbox;

  private slots:
    void deleteSelectedCells();
    void validateRandGenerationProperties();
    void generateTriangle();

  private:
    bool validateRandGenerationRange();
};
#endif // WINDOW_H
