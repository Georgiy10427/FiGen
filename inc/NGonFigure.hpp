#ifndef NGONFIGURE_HPP
#define NGONFIGURE_HPP

#include "canvas.hpp"
#include <QFrame>
#include <QHeaderView>
#include <QSysInfo>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <triangle.hpp>

class NGonFigure : public QObject, BaseFigure {
  public:
    NGonFigure(QTableWidget *table, Canvas *canvas, QObject *parent = nullptr);
    void setTable(QTableWidget *table);
    void setSidesAndAngles(QMap<int, double> sides, QMap<int, double> angles);
  public slots:
    void tableCellChanged(QTableWidgetItem *item);
    void resetData();
    void calcNgon();

  private:
    void setupTableAppearence();
    QMap<int, double> getRowItems(int row);
    void updateProperties();
    void updatePropertiesInTable();
    void updateTableColumnsQuantity();
    void alignItemTextAtCenter(QTableWidgetItem *item);
    void updateAnglesFunctions();
    void drawNgonSuggestion();
    void setRowItems(int row, QMap<int, double> items,
                     bool rewriteEmpty = false);
    void updateVHeaders();
    void updateHeaders();

    const int minColumnQuantity = 3;
    const int maxColumnQuantity = 3;
    const int frontsRow = 0, anglesRow = 1, sinValuesRow = 2, cosValuesRow = 3,
              tanValuesRow = 4;
    const int trigonametryFunctionsDecimal = 4;
    const QStringList frontsLabels = {"BC", "AC", "AB"};
    const QStringList anglesLabels = {"A", "B", "C"};
    QStringList currentVHeaders = frontsLabels;

    QTableWidget *table = nullptr;

    QMap<int, double> angles;
    QMap<int, double> fronts;
    QMap<int, double> cos_values, sin_values, tan_values;

    Canvas *canvas;
};

#endif // NGONFIGURE_HPP
