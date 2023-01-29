#include "window.hpp"

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    this->setMinimumWidth(800);
    this->setMinimumHeight(200);
    setupIcon();

    QHBoxLayout *layout = new QHBoxLayout();
    this->setLayout(layout);

    this->table = new QTableWidget(this);
    this->canvas_ = new Canvas(this);
    this->ngonfigure = new NGonFigure(table, canvas_, this);

    QTabWidget *sidebar = new QTabWidget(this);

    QPushButton *clearTableBtn = new QPushButton("Очистить таблицу");
    QPushButton *randGenBtn = new QPushButton("Сгенерировать");
    QPushButton *countBtn = new QPushButton("Расчитать");
    QHBoxLayout *pbuttons_layout = new QHBoxLayout();
    pbuttons_layout->setSpacing(10);
    pbuttons_layout->addWidget(countBtn);
    pbuttons_layout->addWidget(randGenBtn);
    pbuttons_layout->addWidget(clearTableBtn);

    QHBoxLayout *toolsPanelLayout = new QHBoxLayout();
    toolsPanelLayout->addItem(pbuttons_layout);

    QWidget *tab1 = new QWidget();
    QVBoxLayout *tab1Layout = new QVBoxLayout();
    tab1->setLayout(tab1Layout);
    tab1Layout->addWidget(table);
    QGroupBox *tools = new QGroupBox("Быстрые действия");
    tools->setLayout(toolsPanelLayout);
    tab1Layout->addWidget(tools);
    tab1->setLayout(tab1Layout);

    sidebar->insertTab(0, tab1, "Многоугольники");

    QShortcut *flushTableShortcut = new QShortcut(this);
    flushTableShortcut->setKey(Qt::CTRL | Qt::Key_D);
    QShortcut *calcFigureShortcut = new QShortcut(this);
    calcFigureShortcut->setKey(Qt::CTRL | Qt::Key_C);

    layout->addWidget(canvas_);
    layout->addWidget(sidebar);

    connect(flushTableShortcut, &QShortcut::activated, ngonfigure, &NGonFigure::resetData);
    connect(calcFigureShortcut, &QShortcut::activated, ngonfigure, &NGonFigure::calcNgon);
    connect(clearTableBtn, &QPushButton::clicked, ngonfigure, &NGonFigure::resetData);
    connect(countBtn, &QPushButton::clicked, ngonfigure, &NGonFigure::calcNgon);
}

void Window::setupIcon()
{
    this->setWindowIcon(QIcon("://icon64"));
    this->setWindowTitle("Figen");
}

Window::~Window()
{
}

