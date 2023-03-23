#ifndef TABLEDISPATCHER_H
#define TABLEDISPATCHER_H

#include "canvas.hpp"
#include <QFrame>
#include <QHeaderView>
#include <QSpinBox>
#include <QSysInfo>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <triangle.hpp>

class TableDispatcher : public QObject, BaseFigure {
  public:
    TableDispatcher(QTableWidget *table, Canvas *canvas, QSpinBox *decimalBox,
                    QObject *parent = nullptr);
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

    static constexpr int minColumnQuantity = 3;
    static constexpr int maxColumnQuantity = 3;
    const int frontsRow = 0, anglesRow = 1, sinValuesRow = 2, cosValuesRow = 3,
              tanValuesRow = 4, ctanValuesRow = 5;
    const int trigonametryFunctionsDecimal = 4;
    const QStringList frontsLabels = {"BC", "AC", "AB"};
    const QStringList anglesLabels = {"A", "B", "C"};
    QStringList currentVHeaders = frontsLabels;
    const QStringList HHeaders = {"Сторона", "Угол", "cos",
                                  "sin",     "tan",  "ctg"};

    QTableWidget *table = nullptr;
    QSpinBox *decimalBox;

    QMap<int, double> angles;
    QMap<int, double> fronts;
    QMap<int, double> cos_values, sin_values, tan_values, ctan_values;
    bool tableLock = false;

    Canvas *canvas;
    const QString windowsCSSPatch = "QHeaderView::section{"
                                    "border-top:0px solid #D8D8D8;"
                                    "border-left:0px solid #D8D8D8;"
                                    "border-right:1px solid #D8D8D8;"
                                    "border-bottom: 1px solid #D8D8D8;"
                                    "background-color:white;"
                                    "padding:4px;"
                                    "}"
                                    "QTableCornerButton::section{"
                                    "border-top:0px solid #D8D8D8;"
                                    "border-left:0px solid #D8D8D8;"
                                    "border-right:1px solid #D8D8D8;"
                                    "border-bottom: 1px solid #D8D8D8;"
                                    "background-color:white;"
                                    "}";
};

#endif // TABLEDISPATCHER_H
