#ifndef NGONFIGURE_HPP
#define NGONFIGURE_HPP

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QFrame>
#include "canvas.hpp"
#include <triangle.hpp>

class NGonFigure : public QObject, BaseFigure
{
public:
    NGonFigure(QTableWidget *table, Canvas *canvas, QObject *parent = nullptr);
    void setTable(QTableWidget *table);
public slots:
    void tableCellChanged(QTableWidgetItem *item);
    void resetData();
    void calcNgon();
private:
    void setupTableAppearence();
    QStringList generateEnglishAlphabet(bool upper_case=true);
    QMap<int, double> getRowItems(int row);
    void updateProperties();
    void updatePropertiesInTable();
    void updateTableColumnsQuantity();
    void alignItemTextAtCenter(QTableWidgetItem *item);
    void updateAnglesFunctions();
    void drawNgonSuggestion();
    void setRowItems(int row, QMap<int, double> items);

    const int minColumnQuantity = 3;
    const int maxColumnQuantity = 5;
    QTableWidget *table = nullptr;

    QMap<int, double> angles;
    QMap<int, double> fronts;
    QMap<int, double> cos_values, sin_values, tan_values;

    Canvas *canvas;
};

#endif // NGONFIGURE_HPP
