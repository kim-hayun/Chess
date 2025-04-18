#ifndef GAME_H
#define GAME_H
#include "piece.h"
#include "mydata.h"
#include <vector>
#include <QObject>


class Widget;

struct SELECT{

    SELECT(int r=-1, int c=-1, piece *K =nullptr) : row(r), col(c), selPMAP(K) {}

    int row;
    int col;

    piece *selPMAP;
};

class game : public QObject
{
    Q_OBJECT

public:
    game(Widget*);
    ~game();


public:

    Widget* parent;

    SELECT selected;
    SELECT selecting;

    const int square;
    const int line;
    const float size;

    QRect Map[8][8];
    piece *PMAP[8][8];

    std::vector<POSITION> way;

    piece::TEAM turn= piece::BLACK;

public:
    void mouseDown(const QPoint& pt);
    void draw(QPainter& qp);

    bool test(const std::vector<POSITION>& v, int row, int col);

    std::vector<POSITION> moving(int r, int c, piece::KIND k);

    std::vector<POSITION> findPawnWay(int row, int col, piece::TEAM teamT);
    std::vector<POSITION> findKnightWay(int row, int col, piece::TEAM teamT);
    std::vector<POSITION> findRookWay(int row, int col, piece::TEAM teamT);
    std::vector<POSITION> findBishopWay(int row, int col, piece::TEAM teamT);
    std::vector<POSITION> findKingWay(int row, int col, piece::TEAM teamT);
    std::vector<POSITION> findQueenWay(int row, int col, piece::TEAM teamT);

signals:
    void GameOver(piece::TEAM);

};


#endif // GAME_H
