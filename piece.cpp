#include "piece.h"
#include <QPixmap>
#include <QApplication>
#include <QDir>
#include <QPainter>

piece::piece(bool _death, KIND _kind, TEAM _team=none)
{

    kind=_kind;
    team=_team;
    death=_death;

    //auto path = QCoreApplication::applicationDirPath();
    QDir appDir(QCoreApplication::applicationDirPath());
    appDir.cdUp();
    appDir.cdUp();
    appDir.cdUp();

    QString path=appDir.path();
    //qDebug() << path +"/img/b.king.png";

    switch(kind){
    case BK:
        img.load(path+"/img/b.king.png");
        break;

    case BQ:
        img.load(path+"/img/b.queen.png");
        break;

    case BN:
    img.load(path+"/img/b.knight.png");
        break;

    case BR:
        img.load(path+"/img/b.rook.png");
        break;

    case BB:
        img.load(path+"/img/b.bishop.png");
        break;

    case BP:
        img.load(path+"/img/b.pawn.png");
        break;

    case WK:
        img.load(path+"/img/w.king.png");
        break;

    case WQ:
        img.load(path+"/img/w.queen.png");
        break;

    case WN:
        img.load(path+"/img/w.knight.png");
        break;

    case WR:
        img.load(path+"/img/w.rook.png");
        break;

    case WB:
        img.load(path+"/img/w.bishop.png");
        break;

    case WP:
        img.load(path+"/img/w.pawn.png");
        break;

    case NONE:
        break;

    }

}



piece::~piece()
{

}
