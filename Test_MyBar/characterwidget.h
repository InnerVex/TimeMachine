#ifndef CHARACTERWIDGET_H
#define CHARACTERWIDGET_H

#include <QWidget>
#include <QKeyEvent>
#include <QPainter>
#include <QToolTip>

class CharacterWidget : public QWidget
{
    Q_OBJECT
public:
    CharacterWidget(QWidget *parent = 0);
    QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:
    void updateFont(const QFont &font);
    void updateSize(const QString &fontSize);
    void updateStyle(const QString &fontStyle);

signals:
    void characterSelected(const QString &character);

protected:
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QFont displayFont;
    int columns;
    int lastKey;
    int squareSize;
};

#endif // CHARACTERWIDGET_H
