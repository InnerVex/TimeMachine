#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "characterwidget.h"
#include "timebar.h"
#include <QMainWindow>
#include <QComboBox>
#include <QScrollArea>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
