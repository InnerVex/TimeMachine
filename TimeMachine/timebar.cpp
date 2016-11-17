#include "timebar.h"

#include <QPainter>
#include <QDebug>
#include <QToolTip>
#include <QMouseEvent>
#include <QWheelEvent>

TimeBar::TimeBar(QWidget *parent) :
    QWidget(parent)
{

}

QSize TimeBar::sizeHint() const
{
    return QSize(TB_HINT_WIDTH, TB_TIME_BAR_HEIGHT);
}

void TimeBar::dragMoveEvent(QDragMoveEvent *event)
{

}

void TimeBar::mouseMoveEvent(QMouseEvent *event)
{
    QPoint cursorPos = event->pos();
    //QToolTip::showText(mapToGlobal(cursorPos), "TOOLTIP");
    //emit sendMessageToStatusBar("CursorPos: " + QString::number(cursorPos.x()) + ", " + QString::number(cursorPos.y()));
}

void TimeBar::mousePressEvent(QMouseEvent *event)
{
    QPoint cursorPos = event->pos();

    //emit sendMessageToStatusBar("Clicked: ");
}

void TimeBar::wheelEvent(QWheelEvent *event)
{
    if(event->modifiers() & Qt::ControlModifier)
    {
        scale -= (event->delta()/120);
        scale = (scale < 0) ? 0 : (scale > TB_SCALE_MAX) ? TB_SCALE_MAX : scale;
        emit sendMessageToStatusBar("Scale: " + QString::number(scale));
    }
    else if(event->modifiers() & Qt::ShiftModifier)
    {
        firstVisibleTime -= (event->delta()/120) * divValue * 10;
        firstVisibleTime = (firstVisibleTime < 0) ? 0 : firstVisibleTime;
        emit sendMessageToStatusBar("FirstVisibleTime: " + QString::number(firstVisibleTime));
    }
    else
    {
        firstVisibleTime -= (event->delta()/120) * divValue;
        firstVisibleTime = (firstVisibleTime < 0) ? 0 : firstVisibleTime;
        emit sendMessageToStatusBar("FirstVisibleTime: " + QString::number(firstVisibleTime));
    }

    repaint();
}

void TimeBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    widgetWidth = this->width();

    //Определяем цену и ширину делений и их количество
    int divWidthIncrement = 1;
    int divSignIncrement = 1;
    if(scale == TB_SCALE_S1)
    {
        divValue = TB_DIV_VALUE_S1;
        minDivSignPeriod = TB_SCALE_S1_MIN_DIV_SIGN;
    }
    else if (scale <= TB_SCALE_S10)
    {
        divValue = TB_DIV_VALUE_S10;
        divWidthIncrement = TB_SCALE_S10 - scale + 1;
        minDivSignPeriod = TB_SCALE_S10_MIN_DIV_SIGN;
    }
    else if (scale <= TB_SCALE_M1)
    {
        divValue = TB_DIV_VALUE_M1;
        divWidthIncrement = TB_SCALE_M1 - scale + 1;
        minDivSignPeriod = TB_SCALE_M1_MIN_DIV_SIGN;
    }
    else if (scale <= TB_SCALE_M10)
    {
        divValue = TB_DIV_VALUE_M10;
        divWidthIncrement = TB_SCALE_M10 - scale + 1;
        minDivSignPeriod = TB_SCALE_M10_MIN_DIV_SIGN;
    }
    else if (scale <= TB_SCALE_H1)
    {
        divValue = TB_DIV_VALUE_H1;
        divWidthIncrement = (TB_SCALE_H1 - scale + 1);
        minDivSignPeriod = TB_SCALE_H1_MIN_DIV_SIGN;
    }
    else if (scale <= TB_SCALE_H6)
    {
        divValue = TB_DIV_VALUE_H6;
        divWidthIncrement = (TB_SCALE_H6 - scale + 1);
        minDivSignPeriod = TB_SCALE_H6_MIN_DIV_SIGN;
    }
    else if (scale <= TB_SCALE_D1)
    {
        divValue = TB_DIV_VALUE_D1;
        divWidthIncrement = TB_SCALE_D1 - scale + 1;
        minDivSignPeriod = TB_SCALE_D1_MIN_DIV_SIGN;
    }

    if(divWidthIncrement > 2)
        divSignIncrement = divWidthIncrement - divWidthIncrement % 2;
    minDivSignPeriod /= divSignIncrement;

    mDivWidth = TB_DIVISION_MIN_WIDTH * divWidthIncrement;
    int divAmount = widgetWidth / mDivWidth;

    //Крайнее левое положение должно быть кратно цене деления
    firstVisibleTime -= firstVisibleTime % divValue;

    //Отрисовываем деления
    QPen divPen(Qt::black);

    for(int idiv = 1; idiv <= divAmount; idiv++)
    {
        divPen.setWidth(1);

        divXPos = mDivWidth * idiv;
        divHeight = TB_DIVISION_HEIGHT_MINOR;

        //Соответствующее делению время
        unixtime = firstVisibleTime + divValue * idiv;;
        time_t t = unixtime;
        tm = localtime(&t);

        //Отрисовка подписи под делением
        painter.setFont(QFont("Sans-serif", 10));
        divSigned = false;

        if (tm->tm_sec == 0)
        {
            if (tm->tm_min == 0)
            {
                if (tm->tm_hour == 0)
                {
                    if (tm->tm_mday == 1)
                    {
                        if (tm->tm_mon == 0)
                        {
                            //Деление соответствует началу года
                            divHeight = TB_DIVISION_HEIGHT_YEAR;
                            drawDivSign(painter, "%Y г.");
                        }
                        else
                        {
                            //Деление соответствует началу месяца
                            divPen.setWidth(2);
                            divHeight = TB_DIVISION_HEIGHT_BIG;
                            drawDivSign(painter, "%m.%Y");
                        }
                    }
                    else
                    {
                        //Деление соответствует началу дня
                        divPen.setWidth(2);
                        divHeight = TB_DIVISION_HEIGHT_BIG;
                        if(minDivSignPeriod <= SECONDS_IN_DAY)
                            drawDivSign(painter, "%d.%m.%Y");
                    }
                }
                else
                {
                    //Деление соответствует началу часа
                    divPen.setWidth(2);
                    divHeight = TB_DIVISION_HEIGHT_MEDIUM;
                    if(minDivSignPeriod <= SECONDS_IN_HOUR)
                        drawDivSign(painter, "%H:%M:%S");
                }
            }
            else
            {
                //Деление соответствует началу минуты
                divHeight = TB_DIVISION_HEIGHT_MEDIUM;
                if(minDivSignPeriod <= SECONDS_IN_MINUTE)
                    drawDivSign(painter, "%H:%M:%S");
            }
        }

        if (!divSigned && unixtime % minDivSignPeriod == 0)
        {
            //Деление соответствует периоду, специфическому для масштаба
            divHeight = TB_DIVISION_HEIGHT_MEDIUM;
            if (minDivSignPeriod >= 86400)
            {
                //Кратно дням
                divPen.setWidth(2);
                divHeight = TB_DIVISION_HEIGHT_BIG;
                drawDivSign(painter, "%d.%m.%Y");
            }
            else if (minDivSignPeriod >= 3600)
            {
                //Кратно часам
                divPen.setWidth(2);
                divHeight = TB_DIVISION_HEIGHT_MEDIUM;
                drawDivSign(painter, "%H:%M:%S");
            }
            else if (minDivSignPeriod >= 60)
            {
                //Кратно минутам
                divHeight = TB_DIVISION_HEIGHT_MEDIUM;
                drawDivSign(painter, "%H:%M:%S");
            }
            else
            {
                drawDivSign(painter, "%H:%M:%S");
            }
        }

        painter.setPen(divPen);
        painter.drawLine(
                    QPoint(divXPos, TB_TIME_SLIDER_HEIGHT),
                    QPoint(divXPos, TB_TIME_SLIDER_HEIGHT + divHeight));
    }

    //Отрисовка области управления воспроизведением
    divPen.setWidth(1);
    painter.setPen(divPen);
    painter.setBrush(QBrush(Qt::lightGray));
    painter.drawRect(QRect(0, 0, widgetWidth - 1, TB_TIME_SLIDER_HEIGHT));

    QRect target(50, 0, TB_TIME_SLIDER_WIDTH, TB_TIME_SLIDER_HEIGHT);
    QRect source(0, 0, 0, 0);
    QPixmap pixmap(":/player/images/scroller");

    painter.drawPixmap(target, pixmap, source);
}

void TimeBar::drawDivSign(QPainter &painter, const char* format)
{
    char sign[20];
    strftime(sign, sizeof(sign), format, tm);
    painter.drawText(
                divXPos - 100,
                TB_TIME_SLIDER_HEIGHT + divHeight,
                200,
                100,
                Qt::AlignHCenter,
                sign);
    divSigned = true;
}
