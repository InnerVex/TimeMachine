#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget *centralWidget = new QWidget;
    QHBoxLayout *timeLayout = new QHBoxLayout(centralWidget);

    TimeBar *timeBar = new TimeBar(centralWidget);
    timeBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    timeLayout->addWidget(timeBar);
    centralWidget->setLayout(timeLayout);

    setCentralWidget(centralWidget);
    setWindowTitle(tr("TimeBar"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
