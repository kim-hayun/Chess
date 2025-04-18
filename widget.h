#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include "piece.h"
#include "game.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;

public:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *e);

    game* pG;

public slots:
    void gameOver( piece::TEAM t );
};
#endif // WIDGET_H
