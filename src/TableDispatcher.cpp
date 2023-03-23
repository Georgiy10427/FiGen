#include "TableDispatcher.hpp"

TableDispatcher::TableDispatcher(QTableWidget *table, Canvas *canvas,
                                 QSpinBox *decimalBox, QObject *parent)
    : QObject(parent) {
    setTable(table);
    this->canvas = canvas;
    this->decimalBox = decimalBox;
}

void TableDispatcher::setTable(QTableWidget *table) {
    if (this->table == nullptr) {
        this->table = table;
        setupTableAppearence();
        connect(this->table, &QTableWidget::itemChanged, this,
                &TableDispatcher::tableCellChanged);
        connect(this->table, &QTableWidget::itemSelectionChanged, this,
                &TableDispatcher::updateVHeaders);
    } else {
        disconnect(this->table, &QTableWidget::itemChanged, this,
                   &TableDispatcher::tableCellChanged);
        disconnect(this->table, &QTableWidget::itemSelectionChanged, this,
                   &TableDispatcher::updateVHeaders);
        this->table = table;
        setupTableAppearence();
        connect(this->table, &QTableWidget::itemChanged, this,
                &TableDispatcher::tableCellChanged);
    }
}

void TableDispatcher::updateVHeaders() {
    if (table->selectedItems().size() == 1) {
        if (table->selectedItems()[0]->row() == frontsRow) {
            currentVHeaders = frontsLabels;
        } else if (table->selectedItems()[0]->row() == anglesRow) {
            currentVHeaders = anglesLabels;
        }
    } else {
        currentVHeaders = frontsLabels;
    }
    updateHeaders();
}

void TableDispatcher::updateHeaders() {
    table->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    table->setHorizontalHeaderLabels(currentVHeaders);
    table->setVerticalHeaderLabels(HHeaders);
    if ((QSysInfo::productVersion() == "10" ||
         QSysInfo::productVersion() == "11") &&
        QSysInfo::productType() == "windows") {
        table->setStyleSheet(windowsCSSPatch);
    }
}

void TableDispatcher::setupTableAppearence() {
    table->setRowCount(HHeaders.size());
    table->setColumnCount(minColumnQuantity);
    table->clear();
    updateHeaders();

    for (int i = 0; i < table->rowCount(); ++i) {
        for (int j = 0; j < maxColumnQuantity; ++j) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            if (i == sinValuesRow || i == cosValuesRow || i == tanValuesRow ||
                i == ctanValuesRow)
                item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            table->setItem(i, j, item);
        }
    }
}

void TableDispatcher::tableCellChanged(QTableWidgetItem *item) {
    if (tableLock)
        return;
    updateProperties();
    alignItemTextAtCenter(item);
    drawNgonSuggestion();
    updatePropertiesInTable();
}

QMap<int, double> TableDispatcher::getRowItems(int row) {
    QMap<int, double> items;
    double cellValue;
    QString cellContent;
    QTableWidgetItem *currentCell;

    bool err;

    for (int i = 0; i < table->columnCount(); ++i) {
        currentCell = table->item(row, i);

        if (currentCell == nullptr) {
            continue;
        }

        cellContent = currentCell->text();
        cellContent = cellContent.replace(",", ".");
        cellValue = cellContent.toDouble(&err);

        if (err && !cellContent.isEmpty() && !cellContent.isNull() &&
            cellValue) {
            items.insert(i, cellValue);
        } else {
            continue;
        }
    }
    return items;
}

void TableDispatcher::updateProperties() {
    fronts.clear();
    angles.clear();
    fronts = getRowItems(frontsRow);
    angles = getRowItems(anglesRow);
    updateAnglesFunctions();
}

void TableDispatcher::setRowItems(int row, QMap<int, double> items,
                                  bool rewriteEmpty) {
    for (int i = 0; i < table->columnCount(); ++i) {
        QTableWidgetItem *cell = table->item(row, i);
        if (cell == nullptr) {
            break;
        }
        if (items.contains(i)) {
            cell->setText(QString::number(items.value(i)));
        } else if (rewriteEmpty) {
            cell->setText("");
        }
    }
}

void TableDispatcher::updatePropertiesInTable() {
    tableLock = true;
    setRowItems(frontsRow, fronts);
    setRowItems(anglesRow, angles);

    setRowItems(sinValuesRow, cos_values, true);
    setRowItems(cosValuesRow, sin_values, true);
    setRowItems(tanValuesRow, tan_values, true);
    setRowItems(ctanValuesRow, ctan_values, true);
    tableLock = false;
}

void TableDispatcher::alignItemTextAtCenter(QTableWidgetItem *item) {
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

void TableDispatcher::updateAnglesFunctions() {
    cos_values.clear();
    sin_values.clear();
    tan_values.clear();
    ctan_values.clear();
    for (auto i : angles.keys()) {
        cos_values.insert(i, kRound(cos(toRadians(angles[i])),
                                    trigonametryFunctionsDecimal));
        sin_values.insert(i, kRound(sin(toRadians(angles[i])),
                                    trigonametryFunctionsDecimal));
        tan_values.insert(i, kRound(tan(toRadians(angles[i])),
                                    trigonametryFunctionsDecimal));
        ctan_values.insert(
            i, kRound(cos(toRadians(angles[i])) / sin(toRadians(angles[i])),
                      trigonametryFunctionsDecimal));
    }
}

void TableDispatcher::drawNgonSuggestion() {
    if (angles.size() + fronts.size() >= 3) {
        Triangle triangle(fronts, angles, decimalBox->value(),
                          decimalBox->value());
        canvas->setCurrentFigure(triangle);
        canvas->update();
    } else {
        canvas->setCurrentFigureToEmpty();
    }
}

void TableDispatcher::calcNgon() {
    if (angles.size() <= 3 && fronts.size() <= 3) {
        Triangle triangle(fronts, angles, decimalBox->value(),
                          decimalBox->value());
        if (triangle.isValidTriangle()) {
            angles = triangle.anglesAsMap();
            fronts = triangle.frontsAsMap();
        }
    }
    updateAnglesFunctions();
    updatePropertiesInTable();
    drawNgonSuggestion();
}

void TableDispatcher::resetData() {
    angles.clear();
    fronts.clear();
    table->clear();
    updateAnglesFunctions();
    setupTableAppearence();
    canvas->setCurrentFigureToEmpty();
    canvas->update();
}

void TableDispatcher::setSidesAndAngles(QMap<int, double> sides,
                                        QMap<int, double> pangles) {
    fronts = sides;
    this->angles = pangles;
    calcNgon();
}
