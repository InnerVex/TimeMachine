#pragma once

#include <timebarconstants.h>

#include <ctime>

#include <QWidget>
#include <QScrollArea>
#include <QAbstractSlider>
#include <QSlider>
#include <QVBoxLayout>
#include <QPainter>
#include <QDebug>
#include <QWheelEvent>
#include <QPen>

class TimeBar : public QWidget
{
    Q_OBJECT

public:
    explicit TimeBar(QWidget *parent = 0);
    QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:

signals:

protected:
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    //Параметры шкалы
    int widgetWidth;
    int scale = 8;
    int divValue;
    int firstVisibleTime = 1477980226;
    int minDivSignPeriod;

    //Параметры деления шкалы
    int divXPos;
    int divHeight;
    bool divSigned;
    long unixtime;
    struct tm *tm;
    double mDivWidth;

    //Функция отрисовки подписи под шкалой
    void drawDivSign(QPainter &painter, const char* format);
};
