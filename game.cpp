#include "game.h"
#include "piece.h"
#include <QWidget>
#include <QPainter>
#include <QPen>
#include <Qcolor>
#include <algorithm>
#include "widget.h"

game::game(Widget* p) : QObject(), parent(p), square(8), line(600), size(line/square)
{

    connect(this, &game::GameOver, p, &Widget::gameOver);


    for(int r=0; r<square; r++){
        for(int c=0; c<square; c++){
            Map[r][c]=QRect(50 + (size*c), 50 + (size*r), size, size);
            PMAP[r][c]=nullptr;
        }
    }

    //말 생성 : 흑
    PMAP[0][0] = new piece(false, piece::BR, piece::BLACK);

    PMAP[0][1] = new piece(false, piece::BN, piece::BLACK);

    PMAP[0][2] = new piece(false, piece::BB, piece::BLACK);

    PMAP[0][3] = new piece(false, piece::BK, piece::BLACK);

    PMAP[0][4] = new piece(false, piece::BQ, piece::BLACK);

    PMAP[0][5] = new piece(false, piece::BB, piece::BLACK);

    PMAP[0][6] = new piece(false, piece::BN, piece::BLACK);

    PMAP[0][7] = new piece(false, piece::BR, piece::BLACK);

    for(int i=0; i<square; i++){
        PMAP[1][i] = new piece(false, piece::BP, piece::BLACK);
    }

// B < rook knight bishop king queen bs ni rk p p p p p p p p >

    //밀 생성 : 백
    PMAP[7][0] = new piece(false, piece::WR, piece::WHITE);

    PMAP[7][1] = new piece(false, piece::WN, piece::WHITE);

    PMAP[7][2] = new piece(false, piece::WB, piece::WHITE);

    PMAP[7][3] = new piece(false, piece::WK, piece::WHITE);

    PMAP[7][4] = new piece(false, piece::WQ, piece::WHITE);

    PMAP[7][5] = new piece(false, piece::WB, piece::WHITE);

    PMAP[7][6] = new piece(false, piece::WN, piece::WHITE);

    PMAP[7][7] = new piece(false, piece::WR, piece::WHITE);

    for(int i=0; i<square; i++){
        PMAP[6][i] = new piece(false, piece::WP, piece::WHITE);
    }
}

game::~game(){

    for (int x=0; x<square; x++)
    {
        for(int y=0; y<square; y++)
        {
            if(PMAP[x][y])
                delete PMAP[x][y];
        }
    }
}

void game::mouseDown(const QPoint &pt){

    for(int r=0; r<square; r++){
        for(int c=0; c<square; c++){
            auto m = Map[r][c];
            if( m.contains(pt.x(), pt.y()) ){

                //qDebug() << r << ',' << c;

                if(PMAP[r][c]){ // 현재 선택이 빈공간 X

                    selecting.row=r;
                    selecting.col=c;

                    selecting.selPMAP = PMAP[r][c];

                    selecting.selPMAP->team = PMAP[r][c]->team;


                    if(selected.selPMAP==nullptr){ // 전 선택 X

                        if(turn == PMAP[r][c]->team){
                            selected.row=r;
                            selected.col=c;

                            selected.selPMAP = PMAP[r][c];

                            selected.selPMAP->team = PMAP[r][c]->team;

                            way=moving(r, c, PMAP[r][c]->kind);
                        }

                        return;

                    }
                    else{ // 전 선택 O


                        if(selected.col==c && selected.row==r){ // 같은 기물 선택

                            way.clear();

                            selected.selPMAP=nullptr;

                            selected.col=-1;
                            selected.row=-1;

                        }
                        else if(selected.col!=c || selected.row!=r){ //다른 기물 선택

                            if(selected.selPMAP->team==selecting.selPMAP->team){

                                selected.selPMAP->team=PMAP[r][c]->team;

                                selected.col=c;
                                selected.row=r;

                                selected.selPMAP=PMAP[r][c];

                                way=moving(r, c, PMAP[r][c]->kind);
                            }
                            else{//상대 기물 먹기

                                bool a = test(way, r, c);

                                if(a){

                                    int rr = selected.row;
                                    int cc = selected.col;

                                    PMAP[rr][cc]=PMAP[r][c];
                                    PMAP[r][c]=selected.selPMAP;


                                    selected.col=-1;
                                    selected.row=-1;

                                    selected.selPMAP=nullptr;

                                    way.clear();

                                    if(turn==piece::BLACK)
                                        turn=piece::WHITE;
                                    else
                                        turn=piece::BLACK;

                                    if(PMAP[rr][cc]->kind==piece::BK){
                                        PMAP[rr][cc]=nullptr; // 먹은 기물 삭제
                                        emit GameOver( piece::BLACK );
                                    }else if(PMAP[rr][cc]->kind==piece::WK){
                                        PMAP[rr][cc]=nullptr; // 먹은 기물 삭제
                                        emit GameOver( piece::WHITE );
                                    }
                                    PMAP[rr][cc]=nullptr; // 먹은 기물 삭제
                                }
                            }
                        }

                    }

                }
                else if(PMAP[r][c]==nullptr && selected.selPMAP!=nullptr){ // 빈공간 선택

                    bool a = test(way, r, c);

                    if(a){

                        int rr = selected.row;
                        int cc = selected.col;

                        PMAP[r][c] = selected.selPMAP; //현재 선택 <-전 선택
                        PMAP[rr][cc] = nullptr; // 전 선택 비우기

                        selected.col=-1; // 전선택 초기화
                        selected.row=-1;

                        selected.selPMAP=nullptr;

                        way.clear();

                        if(turn==piece::BLACK)
                            turn=piece::WHITE;
                        else
                            turn=piece::BLACK;
                    }
                }
            }
        }
    }
}

bool game::test(const std::vector<POSITION>& v, int row, int col){

    std::vector<POSITION>::const_iterator itr;

    for(itr=v.begin(); itr!=v.end(); ++itr){
        if( itr->row == row && itr->col == col)
            return true;
    }

    return false;

}

void game::draw(QPainter &qp){
    //체스판 선 그리기
    for(int i=0; i<=square; i++){

        QPointF p1(50, size*i + 50);
        QPointF p2(line+50, size*i + 50);
        QPointF p3(size*i +50, 50);
        QPointF p4(size*i +50, line+50);

        qp.drawLine(p1, p2);
        qp.drawLine(p3, p4);

    }


    //칸 칠하기
    QBrush qb1;
    QColor c(70, 70, 70);
    qb1.setStyle(Qt::SolidPattern);
    qb1.setColor(c);

    QBrush qb2;
    QColor _c(255, 255, 255);
    qb2.setStyle(Qt::SolidPattern);
    qb2.setColor(_c);

    for(int x=0; x<square; x++){
        for(int y=0; y<square; y++){
            if((x+y)%2==1)
                qp.setBrush(qb1);
            else
                qp.setBrush(qb2);

            qp.drawRect(50+(size*y), 50+(size*x), size, size);
        }
    }

    if(!way.empty()){

        QPen p( QColor(0, 244, 0), 3);
        qp.setPen(p);

        qp.setBrush(QBrush(Qt::DiagCrossPattern));

        for(int i=0; i<way.size(); i++){
            qp.drawRect(Map[way[i].row][way[i].col]);
        }
    }

    // 선택된 기물 테두리 그리기
    if(selected.selPMAP)
    {
        int r=selected.row;
        int c=selected.col;

        QPen p( QColor(244, 0, 0), 3);
        qp.setPen(p);

        // 널브러쉬
        qp.setBrush(QBrush(Qt::NoBrush));

        qp.drawRect(Map[r][c]);
    }

    //말 그리기
    for(int r=0; r<square; r++){
        for(int c=0; c<square; c++){
            if(PMAP[r][c]!=nullptr){
                qp.drawPixmap(Map[r][c], PMAP[r][c]->img);
            }
        }
    }

}

std::vector<POSITION> game::moving(int r, int c, piece::KIND k){

    std::vector<POSITION> v;

    switch(k){
    case piece::BP:
        v=findPawnWay(r, c, piece::BLACK);
        break;

    case piece::WP:
        v=findPawnWay(r, c, piece::WHITE);
        break;


    case piece::BR:
        v=findRookWay(r, c, piece::BLACK);
        break;

    case piece::WR:
        v=findRookWay(r, c, piece::WHITE);
        break;


    case piece::BB:
        v=findBishopWay(r, c, piece::BLACK);
        break;

    case piece::WB:
        v=findBishopWay(r, c, piece::WHITE);
        break;


    case piece::BN:
        v=findKnightWay(r, c, piece::BLACK);
        break;

    case piece::WN:
        v=findKnightWay(r, c, piece::WHITE);
        break;

    case piece::BQ:
        v=findQueenWay(r, c, piece::BLACK);
        break;

    case piece::WQ:
        v=findQueenWay(r, c, piece::WHITE);
        break;

    case piece::BK:
        v=findKingWay(r, c, piece::BLACK);
        break;

    case piece::WK:
        v=findKingWay(r, c, piece::WHITE);
        break;

    default:
        break;

    }

    return v;

}

std::vector<POSITION> game::findPawnWay(int row, int col, piece::TEAM teamT){

    std::vector<POSITION> v;



    if(teamT==piece::BLACK){
        if(PMAP[row+1][col]==nullptr){
            POSITION P(row+1, col);
            v.push_back(P);
        }
        if(row==1){
            if(!PMAP[row+2][col]){
                POSITION P(row+2, col);
                v.push_back(P);
            }
        }
    }
    else{
        if(PMAP[row-1][col]==nullptr){
            POSITION P(row-1, col);
            v.push_back(P);
        }
        if(row==6){
            if(!PMAP[row-2][col]){
                POSITION P(row-2, col);
                v.push_back(P);
            }
        }
    }

    if(teamT==piece::BLACK){
        if(PMAP[row+1][col-1] && PMAP[row+1][col-1]->team!=teamT){
            POSITION P(row+1, col-1);
            v.push_back(P);
        }
        if(PMAP[row+1][col+1] && PMAP[row+1][col+1]->team!=teamT){
            POSITION P(row+1, col+1);
            v.push_back(P);
        }
    }
    else if(teamT==piece::WHITE){
        if(PMAP[row-1][col+1] && PMAP[row-1][col+1]->team!=teamT){
            POSITION P(row-1, col+1);
            v.push_back(P);
        }
        if(PMAP[row-1][col-1] && PMAP[row-1][col-1]->team!=teamT){
            POSITION P(row-1, col-1);
            v.push_back(P);
        }
    }



    for( int i=0; i<v.size(); i++){
        qDebug() << '[' << v[0].row << ',' << v[0].col << ']';
    }

    return v;
}

std::vector<POSITION> game::findKnightWay(int row, int col, piece::TEAM teamT){

    std::vector<POSITION> v;


    if(row+2 < 8){
        if(col-1>=0){
            if(!PMAP[row+2][col-1] || PMAP[row+2][col-1]->team!=teamT)
                v.push_back( POSITION( row+2, col-1) );
        }
        if(col+1<8){
            if(!PMAP[row+2][col+1] || PMAP[row+2][col+1]->team!=teamT)
                v.push_back( POSITION( row+2, col+1) );
        }
    }
    if(row-2 >=0 ){
        if(col-1>=0){
            if(!PMAP[row-2][col-1] || PMAP[row-2][col-1]->team!=teamT)
                v.push_back( POSITION( row-2, col-1) );
        }
        if(col+1<8){
            if(!PMAP[row-2][col+1] || PMAP[row-2][col+1]->team!=teamT)
                v.push_back( POSITION( row-2, col+1) );
        }
    }

    if(col+2 < 8){
        if(row-1>=0){
            if(!PMAP[row-1][col+2] || PMAP[row-1][col+2]->team!=teamT)
                v.push_back( POSITION( row-1, col+2) );
        }
        if(row+1<8){
            if(!PMAP[row+1][col+2] || PMAP[row+1][col+2]->team!=teamT)
                v.push_back( POSITION( row+1, col+2) );
        }
    }

    if(col-2 >=0){
        if(row-1>=0){
            if(!PMAP[row-1][col-2] || PMAP[row-1][col-2]->team!=teamT)
               v.push_back( POSITION( row-1, col-2) );
        }
        if(row+1<8){
            if(!PMAP[row+1][col-2] || PMAP[row+1][col-2]->team!=teamT)
                v.push_back( POSITION( row+1, col-2) );
        }
    }

    return v;
}

std::vector<POSITION> game::findRookWay(int row, int col, piece::TEAM teamT){

    std::vector<POSITION> v;

    int r=row-1;
    while(0 <= r){
        if(!PMAP[r][col])
            v.push_back ( POSITION(r, col) );
        else if(PMAP[r][col]->team==teamT)
            break;
        else{
            v.push_back ( POSITION(r, col) );
            break;
        }
        r-=1;
    }

    r=row+1;
    while(r< 8){
        if(!PMAP[r][col])
            v.push_back( POSITION(r, col) );
        else if(PMAP[r][col]->team==teamT)
            break;
        else{
            v.push_back ( POSITION(r, col) );
            break;
        }

        r+=1;
    }

    int c = col-1;
    while(0 <= c){
        if(!PMAP[row][c])
            v.push_back( POSITION(row, c) );
        else if(PMAP[row][c]->team==teamT)
            break;
        else{
            v.push_back( POSITION(row, c) );
            break;
        }

        c-=1;
    }

    c=col+1;
    while(c < 8){
        if(!PMAP[row][c])
            v.push_back( POSITION(row, c) );
        else if(PMAP[row][c]->team==teamT)
            break;
        else{
            v.push_back( POSITION(row, c) );
            break;
        }

        c+=1;
    }

    return v;
}

std::vector<POSITION> game::findBishopWay(int row, int col, piece::TEAM teamT){

    std::vector<POSITION> v;

    int r=row+1;
    int c=col+1;
    while(r < 8 && c < 8){
        if(!PMAP[r][c])
            v.push_back( POSITION(r, c) );
        else if(PMAP[r][c]->team==teamT)
            break;
        else{
            v.push_back( POSITION(r, c) );
            break;
        }
        r++;
        c++;
    }

    r=row+1;
    c=col-1;
    while(r <8 && c>=0) {
        if(!PMAP[r][c])
            v.push_back( POSITION(r, c) );
        else if(PMAP[r][c]->team==teamT)
            break;
        else{
            v.push_back( POSITION(r, c) );
            break;
        }
        r++;
        c--;
    }

    r=row-1;
    c=col+1;
    while(r >=0 && c<8){
        if(!PMAP[r][c])
            v.push_back( POSITION(r, c) );
        else if(PMAP[r][c]->team==teamT)
            break;
        else{
            v.push_back( POSITION(r, c) );
            break;
        }
        r--;
        c++;
    }

    r=row-1;
    c=col-1;
    while(r >=0 && c >=0){
        if(!PMAP[r][c])
            v.push_back( POSITION(r, c) );
        else if(PMAP[r][c]->team==teamT)
            break;
        else{
            v.push_back( POSITION(r, c) );
            break;
        }
        r--;
        c--;
    }

    return v;
}

std::vector<POSITION> game::findKingWay(int row, int col, piece::TEAM teamT){

    std::vector<POSITION> v;

    int r=row+1;
    if(r<8){
        if(!PMAP[r][col])
            v.push_back( POSITION(r, col) );
        if(col+1<8 && !PMAP[r][col+1])
            v.push_back( POSITION(r, col+1) );
        if(col-1>=0 && !PMAP[r][col-1])
            v.push_back( POSITION(r, col-1) );
    }

    r=row-1;
    if(r>=0){
        if(!PMAP[r][col])
            v.push_back( POSITION(r, col) );
        if(col+1<8 && !PMAP[r][col+1])
            v.push_back( POSITION(r, col+1) );
        if(col-1>=0 && !PMAP[r][col-1])
            v.push_back( POSITION(r, col-1) );
    }

    r=row;
    if(col+1<8 && !PMAP[r][col+1])
        v.push_back( POSITION(r, col+1) );
    if(col-1>=0 && !PMAP[r][col+1])
        v.push_back( POSITION(r, col-1) );


    return v;

}

std::vector<POSITION> game::findQueenWay(int row, int col, piece::TEAM teamT){

    std::vector<POSITION> v;
    std::vector<POSITION> v1;
    std::vector<POSITION> v2;

    v1 = findBishopWay(row, col, teamT);
    v2 = findRookWay(row, col, teamT);

    for(int i=0; i<v1.size(); i++){
        v.push_back(v1[i]);
    }
    for(int i=0; i<v2.size(); i++){
        v.push_back(v2[i]);
    }

    return v;

}

