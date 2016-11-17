#pragma once

#include <timebarconstants.h>

#include <ctime>

#include <QWidget>

class TimeBar : public QWidget
{
    Q_OBJECT

public:
    explicit TimeBar(QWidget *parent = 0);
    QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:

signals:

    void sendMessageToStatusBar(QString message);

protected:
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    QWidget *mainWindow;

    //Параметры шкалы
    int widgetWidth;
    int scale = 8;
    int divValue;
    quint32 firstVisibleTime = 1477980226;
    int minDivSignPeriod;

    //Параметры деления шкалы
    int divXPos;
    int divHeight;
    bool divSigned;
    quint32 unixtime;
    struct tm *tm;
    double mDivWidth;

    //Функция отрисовки подписи под шкалой
    void drawDivSign(QPainter &painter, const char* format);
};
