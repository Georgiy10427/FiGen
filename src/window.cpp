#include "window.hpp"

Window::Window(QWidget *parent) : QWidget(parent) {
    this->setMinimumWidth(950);
    this->setMinimumHeight(515);
    setupIcon();

    auto *layout = new QHBoxLayout();
    this->setLayout(layout);

    this->table = new QTableWidget(this);
    this->canvas = new Canvas(this);
    this->ngonfigure = new NGonFigure(table, canvas, this);

    auto *sidebar = new QTabWidget(this);

    // setup buttons with actions 'Calculate', 'Generate', 'Clear table'
    auto *actionsGroup = new QGroupBox("Быстрые действия");
    auto *pbuttons_layout = new QHBoxLayout();
    pbuttons_layout->setSpacing(10);
    auto *clearTableBtn = new QPushButton("Очистить таблицу");
    auto *randGenBtn = new QPushButton("Сгенерировать");
    auto *countBtn = new QPushButton("Расчитать");
    pbuttons_layout->addWidget(countBtn);
    pbuttons_layout->addWidget(randGenBtn);
    pbuttons_layout->addWidget(clearTableBtn);
    auto *actionsPanelLayout = new QHBoxLayout();
    actionsPanelLayout->addItem(pbuttons_layout);
    actionsGroup->setLayout(actionsPanelLayout);

    // setup panel with generation params
    QGroupBox *generationParamsGroup = new QGroupBox("Параметры генерации");

    auto *rangeParamsLayout = new QVBoxLayout();
    auto *minBorderLabel = new QLabel("Минимальное значение (включительно):");
    auto *maxBorderLabel = new QLabel("Максимальное значение (включительно):");
    this->minRandSpinbox = new QDoubleSpinBox();
    this->maxRandSpinbox = new QDoubleSpinBox();
    auto *intGenerationCheckboxLabel =
        new QLabel("Предпочитать целые числа, если это возможно");
    this->intRandGeneration = new QCheckBox();
    this->intRandGeneration->setChecked(true);

    auto *minBorderBox = new QHBoxLayout();
    auto *maxBorderBox = new QHBoxLayout();
    auto *intGenerationBox = new QHBoxLayout();

    minRandSpinbox->setRange(1.0, 4900);
    minRandSpinbox->setValue(10);
    maxRandSpinbox->setRange(2, 5000);
    maxRandSpinbox->setValue(100);

    minBorderBox->addWidget(minBorderLabel);
    minBorderBox->addWidget(minRandSpinbox);
    minBorderBox->setSpacing(10);

    maxBorderBox->addWidget(maxBorderLabel);
    maxBorderBox->addWidget(maxRandSpinbox);
    maxBorderBox->setSpacing(10);

    intGenerationBox->addWidget(intGenerationCheckboxLabel);
    intGenerationBox->addWidget(intRandGeneration);
    intGenerationBox->setSpacing(31);

    rangeParamsLayout->addItem(minBorderBox);
    rangeParamsLayout->addItem(maxBorderBox);
    rangeParamsLayout->addItem(intGenerationBox);
    rangeParamsLayout->setSpacing(5);

    // setup group with triangle properties
    auto propertiesGroupbox = new QGroupBox("Свойства треугольника");
    this->isEquileterialChk = new QCheckBox("Равносторонний");
    this->isIsoscalesChk = new QCheckBox("Равнобедренный");
    this->isRectangularChk = new QCheckBox("Прямоугольный");

    auto propertiesLayout = new QVBoxLayout();
    propertiesLayout->addWidget(isEquileterialChk);
    propertiesLayout->addWidget(isIsoscalesChk);
    propertiesLayout->addWidget(isRectangularChk);
    propertiesLayout->setSpacing(10);
    propertiesGroupbox->setLayout(propertiesLayout);

    auto *paramsLayout = new QHBoxLayout();
    paramsLayout->addItem(rangeParamsLayout);
    paramsLayout->addWidget(propertiesGroupbox);
    paramsLayout->setSpacing(15);
    generationParamsGroup->setLayout(paramsLayout);

    auto *tab1 = new QWidget();
    auto *tab1Layout = new QVBoxLayout();
    tab1->setLayout(tab1Layout);
    tab1Layout->addWidget(generationParamsGroup);
    tab1Layout->addWidget(table);
    tab1Layout->addWidget(actionsGroup);
    tab1->setLayout(tab1Layout);

    sidebar->insertTab(0, tab1, "Треугольники");

    QShortcut *flushTableShortcut = new QShortcut(this);
    flushTableShortcut->setKey(Qt::CTRL | Qt::Key_D);
    QShortcut *calcFigureShortcut = new QShortcut(this);
    calcFigureShortcut->setKey(Qt::CTRL | Qt::Key_C);
    QShortcut *deleteTablePart = new QShortcut(this);
    deleteTablePart->setKey(Qt::Key_Delete);

    layout->addWidget(canvas);
    layout->addWidget(sidebar);

    connect(flushTableShortcut, &QShortcut::activated, ngonfigure,
            &NGonFigure::resetData);
    connect(calcFigureShortcut, &QShortcut::activated, ngonfigure,
            &NGonFigure::calcNgon);
    connect(deleteTablePart, &QShortcut::activated, this,
            &Window::deleteSelectedCells);

    connect(clearTableBtn, &QPushButton::clicked, ngonfigure,
            &NGonFigure::resetData);
    connect(countBtn, &QPushButton::clicked, ngonfigure,
            &NGonFigure::calcNgon);
    connect(randGenBtn, &QPushButton::clicked, this,
            &Window::generateTriangle);

    connect(isEquileterialChk, &QCheckBox::stateChanged, this,
            &Window::validateRandGenerationProperties);
    connect(isIsoscalesChk, &QCheckBox::stateChanged, this,
            &Window::validateRandGenerationProperties);
    connect(isRectangularChk, &QCheckBox::stateChanged, this,
            &Window::validateRandGenerationProperties);
}

void Window::setupIcon() {
    this->setWindowIcon(QIcon("://icon64"));
    this->setWindowTitle("FiGen");
}

void Window::deleteSelectedCells() {
    for (auto item : table->selectedItems()) {
        item->setText("");
    }
}

bool Window::validateRandGenerationRange() {
    if (minRandSpinbox->value() >= maxRandSpinbox->value()) {
        QMessageBox::warning(this, "Ошибка генерации",
                             "Неверно задан диапазон генерации");
        return false;
    }
    return true;
}

void Window::validateRandGenerationProperties(int state) {
    if (isRectangularChk->isChecked() == isEquileterialChk->isChecked()) {
        isRectangularChk->setChecked(Qt::Unchecked);
        isEquileterialChk->setChecked(Qt::Unchecked);
    }
    if (isEquileterialChk->isChecked() and isIsoscalesChk->isChecked()) {
        isIsoscalesChk->setChecked(Qt::Unchecked);
    }
    if (isRectangularChk->isChecked() && intRandGeneration->isChecked()) {
        if (minRandSpinbox->value() < 3 || maxRandSpinbox->value() < 5) {
            QMessageBox::critical(
                this, "Ошибка генерации",
                "Невозможно выполнить условия в целых числах.");
            intRandGeneration->setCheckState(Qt::Unchecked);
        }
    }
}

void Window::generateTriangle() {
    if (not validateRandGenerationRange()) {
        return;
    }
    validateRandGenerationProperties(0);
    auto minBorder = minRandSpinbox->value();
    auto maxBorder = maxRandSpinbox->value();
    bool isEquileterial = isEquileterialChk->isChecked();
    bool isRectangular = isRectangularChk->isChecked();
    bool isIsoscales = isIsoscalesChk->isChecked();
    bool preferInt = intRandGeneration->isChecked();
    auto triangle = Triangle();
    triangle.generate(minBorder, maxBorder, isRectangular, isIsoscales,
                      isEquileterial, preferInt);
    ngonfigure->resetData();
    ngonfigure->setSidesAndAngles(triangle.frontsAsMap(),
                                  triangle.anglesAsMap());
}

Window::~Window() {}
