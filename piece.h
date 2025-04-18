#ifndef PIECE_H
#define PIECE_H
#include <QPixmap>

class piece
{
public:
    enum KIND{BK, BQ, BB, BR, BN, BP,
              WK, WQ, WB, WR, WN, WP,
              NONE};

    enum TEAM{BLACK, WHITE, none};


    piece(bool _death, KIND _kind, TEAM _team);
    ~piece();

public:

    KIND kind;
    TEAM team;
    bool death;


public:
    void move();
    QPixmap img;

};

#endif // PIECE_H
