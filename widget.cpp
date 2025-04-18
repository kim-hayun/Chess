#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>

#include "game.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget), pG(nullptr)
{
    if(pG==nullptr)
        pG=new game(this);

    ui->setupUi(this);
    setFixedSize(pG->line+100, pG->line+100);

}

Widget::~Widget()
{
    delete ui;
    if(pG)
        delete pG;
}

void Widget::paintEvent(QPaintEvent *event){

    QPainter qp;

    qp.begin(this);
    pG->draw(qp);
    qp.end();
}


void Widget::mousePressEvent(QMouseEvent *e){
    QPoint pt = e->pos();
    pG->mouseDown(pt);
    update();
}

void Widget::gameOver( piece::TEAM t ){
    QMessageBox::StandardButton result=QMessageBox::NoButton;

    if(t==piece::BLACK){
        result = QMessageBox::information(this, "GAME OVER. BLACK WIN!!!", "Retry(Y), Exit(N)", QMessageBox::Yes | QMessageBox::No);
    }
    else if(t==piece::WHITE){
        result = QMessageBox::information(this, "GAME OVER. WHITE WIN!!!", "Retry(Y), Exit(N)", QMessageBox::Yes | QMessageBox::No);
    }
    else if(t==piece::none){
        result = QMessageBox::information(this, "GAME OVER. DRAW!!!", "Retry(Y), Exit(N)", QMessageBox::Yes | QMessageBox::No);
    }

    if( result == QMessageBox::Yes){
        if(pG){
            delete pG;
            pG=nullptr;
        }
        pG=new game(this);
    }
    else
        close();
}
