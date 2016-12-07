#include "timebar.h"

#include <QPainter>
#include <QDebug>
#include <QToolTip>
#include <QMouseEvent>
#include <QWheelEvent>

#include "select.h"

TimeBar::TimeBar(QDateTimeEdit *_fvtEdit, QWidget *parent) :
    QWidget(parent), fvtEdit(_fvtEdit)
{
    setMouseTracking(true);

    firstVisibleTime = fvtEdit->dateTime().toTime_t();
    minimumAvailableTime = 946659600;
    canRequestTime = true;
    cursorHover = false;
    drawSlider = false;
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
    //Отрисовываем полоску под курсором
    if(canRequestTime && cursorPos.y() > 0 && cursorPos.y() <= TB_TIME_SLIDER_HEIGHT)
    {
        cursorX = cursorPos.x();
        cursorHover = true;
    }
    else
    {
        cursorHover = false;
    }

    if(cursorPos.y() > 0 && cursorPos.y() <= TB_TIME_BAR_HEIGHT)
    {
        hoverTime = firstVisibleTime + ((float)cursorPos.x() / mDivWidth) * divValue;
        tooltipPoint = QPoint(event->globalPos().x() - TB_TOOLTIP_OFFSET_X,
                              QWidget::mapToGlobal(this->pos()).y() - TB_TOOLTIP_OFFSET_Y);
        QToolTip::showText(tooltipPoint,
                               QDateTime::fromTime_t(hoverTime).toString("dd'.'MM'.'yyyy' 'hh':'mm':'ss"),
                               this, rect() );
    }
    else
    {
        QToolTip::hideText();
    }

    repaint();
}

void TimeBar::mousePressEvent(QMouseEvent *event)
{
    QPoint cursorPos = event->pos();
    //Запрашиваем время по клику на полосе проигрывания
    if(canRequestTime && cursorPos.y() <= TB_TIME_SLIDER_HEIGHT)
    {
        hoverTime = firstVisibleTime + ((float)cursorPos.x() / mDivWidth) * divValue;
        emit setPlayTime(hoverTime);
    }
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
        if(firstVisibleTime < minimumAvailableTime) firstVisibleTime = minimumAvailableTime;
        emit sendMessageToStatusBar("FirstVisibleTime: " + QString::number(firstVisibleTime));
    }
    else
    {
        firstVisibleTime -= (event->delta()/120) * divValue;
        firstVisibleTime = (firstVisibleTime < 0) ? 0 : firstVisibleTime;
        if(firstVisibleTime < minimumAvailableTime) firstVisibleTime = minimumAvailableTime;
        emit sendMessageToStatusBar("FirstVisibleTime: " + QString::number(firstVisibleTime));
    }

    updateFVT();
    repaint();
}

void TimeBar::setFVT(const QDateTime &datetime)
{
    firstVisibleTime = datetime.toTime_t();
    repaint();
}

void TimeBar::setSliderPosition(const qint32 _playTime)
{
    if(_playTime == -1)
    {
        drawSlider = false;
    }
    else
    {
        playTime = _playTime;
        drawSlider = true;
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
    while(firstVisibleTime < minimumAvailableTime)
    {
        firstVisibleTime += divValue;
    }
    updateFVT();

    //Отрисовываем деления
    QPen divPen(Qt::black);

    for(int idiv = 1; idiv <= divAmount; idiv++)
    {
        divPen.setWidth(1);

        divXPos = mDivWidth * idiv;
        divHeight = TB_DIVISION_HEIGHT_MINOR;

        //Соответствующее делению время
        unixtime = firstVisibleTime + divValue * idiv;;

        //Отрисовка подписи под делением
        painter.setFont(QFont("Sans-serif", 10));
        divSigned = false;

        tm = QDateTime::fromTime_t(unixtime);
        if (tm.time().second() == 0)
        {
            if (tm.time().minute() == 0)
            {
                if (tm.time().hour() == 0)
                {
                    if (tm.date().day() == 1)
                    {
                        //Деление соответствует началу месяца
                        divPen.setWidth(2);
                        divHeight = TB_DIVISION_HEIGHT_BIG;
                        drawDivSign(painter, "MM'.'yyyy");
                    }
                    else
                    {
                        //Деление соответствует началу дня
                        divPen.setWidth(2);
                        divHeight = TB_DIVISION_HEIGHT_BIG;
                        if(minDivSignPeriod <= SECONDS_IN_DAY)
                            drawDivSign(painter, "dd'.'MM'.'yyyy");
                    }
                }
                else
                {
                    //Деление соответствует началу часа
                    divPen.setWidth(2);
                    divHeight = TB_DIVISION_HEIGHT_MEDIUM;
                    if(minDivSignPeriod <= SECONDS_IN_HOUR)
                        drawDivSign(painter, "hh':'mm':'ss");
                }
            }
            else
            {
                //Деление соответствует началу минуты
                divHeight = TB_DIVISION_HEIGHT_MEDIUM;
                if(minDivSignPeriod <= SECONDS_IN_MINUTE)
                    drawDivSign(painter, "hh':'mm':'ss");
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
                drawDivSign(painter, "dd'.'MM'.'yyyy");
            }
            else if (minDivSignPeriod >= 3600)
            {
                //Кратно часам
                divPen.setWidth(2);
                divHeight = TB_DIVISION_HEIGHT_MEDIUM;
                drawDivSign(painter, "hh':'mm':'ss");
            }
            else if (minDivSignPeriod >= 60)
            {
                //Кратно минутам
                divHeight = TB_DIVISION_HEIGHT_MEDIUM;
                drawDivSign(painter, "hh':'mm':'ss");
            }
            else
            {
                drawDivSign(painter, "hh':'mm':'ss");
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

    lastVisibleTime = firstVisibleTime + ((float)widgetWidth / mDivWidth) * divValue;

    //Отрисовка областей доступности
    updateAvailability();
    int prevRight = 0;
    for (int iarea = 0; iarea < availabilityVector.size(); iarea++)
    {
        int left = widgetWidth *
                (float)(availabilityVector[iarea].first - firstVisibleTime) / (lastVisibleTime - firstVisibleTime);
        int right = widgetWidth *
                (float)(availabilityVector[iarea].second - firstVisibleTime) / (lastVisibleTime - firstVisibleTime);

        if(left == 0) left += 1;
        if(right == widgetWidth) right -= 1;

        divPen.setStyle(Qt::NoPen);
        painter.setPen(divPen);
        painter.setBrush(QBrush(Qt::green));
        painter.drawRect(QRect(left, 1, (right - left), TB_TIME_SLIDER_HEIGHT - 1));

        if(prevRight != left)
        {
            painter.setBrush(QBrush(Qt::red));
            painter.drawRect(QRect(prevRight, 1, (left - prevRight), TB_TIME_SLIDER_HEIGHT - 1));
        }

        prevRight = right;
    }
    currentTime = QDateTime::currentDateTimeUtc().toTime_t();
    if(availabilityVector.last().second < lastVisibleTime)
    {
        int right = widgetWidth;
        if(currentTime < lastVisibleTime)
        {
            right = widgetWidth *
                    (float)(currentTime - firstVisibleTime) / (lastVisibleTime - firstVisibleTime);

        }
        if(right == widgetWidth) right -= 1;
        painter.setBrush(QBrush(Qt::red));
        painter.drawRect(QRect(prevRight, 1, (right - prevRight), TB_TIME_SLIDER_HEIGHT - 1));
    }
    divPen.setStyle(Qt::SolidLine);

    //Отрисовка подсказывающей полосочки под курсором
    if(cursorHover)
    {
        divPen.setStyle(Qt::SolidLine);
        divPen.setWidth(2);
        painter.setPen(divPen);
        painter.drawLine(
                    QPoint(cursorX, 0),
                    QPoint(cursorX, TB_TIME_SLIDER_HEIGHT));
    }

    //Отрисовка кирпича
    if(drawSlider)
    {
        if(playTime >= firstVisibleTime && playTime <= lastVisibleTime)
        {
            int xSPos = ((float)(playTime - firstVisibleTime) / (lastVisibleTime - firstVisibleTime)) * widgetWidth;
            QRect target(xSPos, 0, TB_TIME_SLIDER_WIDTH, TB_TIME_SLIDER_HEIGHT);
            QRect source(0, 0, 0, 0);
            QPixmap pixmap(":/player/images/scroller");
            painter.drawPixmap(target, pixmap, source);
        }
    }
}

void TimeBar::drawDivSign(QPainter &painter, QString format)
{
    painter.drawText(
                divXPos - 100,
                TB_TIME_SLIDER_HEIGHT + divHeight,
                200,
                100,
                Qt::AlignHCenter,
                tm.toString(format));
    divSigned = true;
}

void TimeBar::updateFVT()
{
    fvtEdit->blockSignals(true);
    fvtEdit->setDateTime(QDateTime::fromTime_t(firstVisibleTime));
    fvtEdit->blockSignals(false);
}

void TimeBar::updateAvailability()
{
    availabilityVector.clear();

    QString filename = Select::selectFile(firstVisibleTime);
    qint32 fileStartTime = Select::selectDateTime(filename);
    qint32 fileEndTime = fileStartTime + Select::selectDuration(fileStartTime) * 0.001;

    if(fileEndTime > firstVisibleTime)
    {
        availabilityVector.append(QPair<qint32, qint32>(firstVisibleTime, fileEndTime));
    }

    while(true)
    {
        filename = Select::selectNextFile(filename);
        if(QString::compare(filename, "", Qt::CaseInsensitive) == 0)
        {
            break;
        }

        fileStartTime = Select::selectDateTime(filename);
        if(fileStartTime >= lastVisibleTime)
        {
            break;
        }

        fileEndTime = fileStartTime + Select::selectDuration(fileStartTime) * 0.001;
        if(fileEndTime >= lastVisibleTime)
        {
            fileEndTime = lastVisibleTime;
        }
        availabilityVector.append(QPair<qint32, qint32>(fileStartTime, fileEndTime));
    }
}
