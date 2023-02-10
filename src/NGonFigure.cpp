#include "NGonFigure.hpp"

NGonFigure::NGonFigure(QTableWidget *table, Canvas *canvas, QObject *parent) : QObject(parent)
{
    setTable(table);
    this->canvas = canvas;
}

void NGonFigure::setTable(QTableWidget *table)
{
    if(this->table == nullptr)
    {
        this->table = table;
        setupTableAppearence();
        connect(this->table, &QTableWidget::itemChanged,this, &NGonFigure::tableCellChanged);
    } else {
        disconnect(this->table, &QTableWidget::itemChanged, this, &NGonFigure::tableCellChanged);
        this->table = table;
        setupTableAppearence();
        connect(this->table, &QTableWidget::itemChanged,this, &NGonFigure::tableCellChanged);
    }
}

void NGonFigure::setupTableAppearence()
{
    table->setRowCount(5);
    table->setColumnCount(minColumnQuantity);
    table->clear();
    table->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    table->setVerticalHeaderLabels({"Сторона", "Угол", "cos", "sin", "tan"});
    table->setHorizontalHeaderLabels(generateEnglishAlphabet());

    for(int i = 0; i < table->rowCount(); ++i)
    {
        for(int j = 0; j < maxColumnQuantity; ++j)
        {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            if(i == 2 || i == 3 || i == 4) item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            table->setItem(i, j, item);
        }
    }
}

QStringList NGonFigure::generateEnglishAlphabet(bool upper_case)
{
    QStringList alphabet;
    char start, end;

    if(upper_case)
    {
        start = 'A';
        end = 'Z';
    } else {
        start = 'a';
        end = 'z';
    }

    for(char i = start; i < end; ++i)
    {
        alphabet.push_back(QString() + QChar(i));
    }
    return alphabet;
}

void NGonFigure::tableCellChanged(QTableWidgetItem *item)
{
    updateProperties();
    updateTableColumnsQuantity();
    alignItemTextAtCenter(item);
    drawNgonSuggestion();
    updateAnglesFunctions();
    updatePropertiesInTable();
}

QMap<int, double> NGonFigure::getRowItems(int row)
{
    QMap<int, double> items;
    double cellValue;
    QString cellContent;
    QTableWidgetItem *currentCell;

    bool err;

    for(int i = 0; i < table->columnCount(); ++i)
    {
        currentCell = table->item(row, i);

        if (currentCell == nullptr)
        {
            continue;
        }

        cellContent = currentCell->text();
        cellContent = cellContent.replace(",", ".");
        cellValue = cellContent.toDouble(&err);

        if(err && !cellContent.isEmpty() && !cellContent.isNull() && cellValue)
        {
           items.insert(i, cellValue);
        } else {
           continue;
        }
    }
    return items;
}

void NGonFigure::updateProperties()
{
    fronts.clear();
    angles.clear();
    fronts = getRowItems(0);
    angles = getRowItems(1);
}

void NGonFigure::setRowItems(int row, QMap<int, double> items)
{
    for(int i = 0; i < table->columnCount(); ++i)
    {
        QTableWidgetItem *cell = table->item(row, i);
        if(cell == nullptr)
        {
            continue;
        }
        if(items.contains(i))
        {
            cell->setText(QString::number(items[i]));
        }
    }
}

void NGonFigure::updatePropertiesInTable()
{
    setRowItems(0, fronts);
    setRowItems(1, angles);
    setRowItems(2, cos_values);
    setRowItems(3, sin_values);
    setRowItems(4, tan_values);
}

void NGonFigure::updateTableColumnsQuantity()
{
    int currentColumnsQuantity = fronts.size();
    if(currentColumnsQuantity < minColumnQuantity)
    {
        currentColumnsQuantity = minColumnQuantity;
    } else if (currentColumnsQuantity + 1 <= maxColumnQuantity) {
        ++currentColumnsQuantity;
    }
    table->setColumnCount(currentColumnsQuantity);
    table->setHorizontalHeaderLabels(generateEnglishAlphabet());
}

void NGonFigure::alignItemTextAtCenter(QTableWidgetItem *item)
{
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

void NGonFigure::updateAnglesFunctions()
{
    cos_values.clear();
    sin_values.clear();
    tan_values.clear();
    for(int i = 0; i < angles.size(); ++i)
    {
        if(angles.contains(i))
        {
            cos_values.insert(i, round( cos(toRadians(angles[i]))*10000)/10000 );
            sin_values.insert(i, round( sin(toRadians(angles[i]))*10000)/10000 );
            tan_values.insert(i, round( tan(toRadians(angles[i]))*10000)/10000 );
        }
    }
}

void NGonFigure::drawNgonSuggestion()
{
    if(angles.size() <= 3 && fronts.size() <= 3)
    {
        Triangle triangle(fronts, angles);
        if(triangle.isValidTriangle())
        {
            canvas->setCurrentFigure(triangle);
            canvas->update();
        } else {
            canvas->setCurrentFigureToEmpty();
        }
    } else {
        canvas->setCurrentFigureToEmpty();
    }
}

void NGonFigure::calcNgon()
{
    if(angles.size() <= 3 && fronts.size() <= 3)
    {
        Triangle triangle(fronts, angles);
        if(triangle.isValidTriangle())
        {
            angles = triangle.anglesAsMap();
            fronts = triangle.frontsAsMap();
        }
    }
    updateAnglesFunctions();
    updatePropertiesInTable();
    drawNgonSuggestion();
}

void NGonFigure::resetData()
{
    angles.clear();
    fronts.clear();
    table->clear();
    setupTableAppearence();
    canvas->setCurrentFigureToEmpty();
    canvas->update();
}
