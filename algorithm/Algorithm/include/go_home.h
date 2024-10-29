#pragma once
#include <stdio.h>
#include <cmath>
#include <iostream>

const int MAX = 30; // キュー配列の最大サイズ

class go_home
{
private:
    int st[MAX]; // スタックを表す配列
    int top = 0; // スタックの先頭を表すポインタ
    // スタックを初期化する
    void st_init() {
        top = 0; // スタックポインタを初期位置に
    }

    // スタックが空かどうかを判定する
    bool st_isEmpty() {
        return (top == 0); // スタックサイズが 0 かどうか
    }

    // スタックが満杯かどうかを判定する
    bool st_isFull() {
        return (top == MAX); // スタックサイズが MAX かどうか
    }

    // push (top を進めて要素を格納)
    void push(int v) {
        if (st_isFull()) {
            //cout << "error: stack is full." << endl;
            return;
        }
        st[top++] = v; // st[top] = v; と top++; をまとめてこのように表せます
    }

    // pop (top をデクリメントして、top の位置にある要素を返す)
    int pop() {
        if (st_isEmpty()) {
            //cout << "error: stack is empty." << endl;
            return -1;
        }
        return st[--top]; // --top; と return st[top]; をまとめてこのように表せます
    }

    int qu[MAX]; // キューを表す配列
    int tail = 0, head = 0; // キューの要素区間を表す変数

    // キューを初期化する
    void init() {
        head = tail = 0;
    }

    // キューが空かどうかを判定する
    bool isEmpty() {
        return (head == tail);
    }

    // スタックが満杯かどうかを判定する
    bool isFull() {
        return (head == (tail + 1) % MAX);
    }

    // enqueue (tail に要素を格納してインクリメント)
    void enqueue(int v) {
        if (isFull()) {
            //cout << "error: queue is full." << endl;
            return;
        }
        qu[tail++] = v;
        if (tail == MAX) tail = 0; // リングバッファの終端に来たら 0 に
    }

    // dequeue (head にある要素を返して head をインクリメント)
    int dequeue() {
        if (isEmpty()) {
            //cout << "error: stack is empty." << endl;
            return -1;
        }
        int res = qu[head];
        ++head;
        if (head == MAX) head = 0;
        return res;
    }

    int8_t kabe_zahyou[90][90];//0000 の4ビットに絶対方向の東8西4南2北1をそれぞれ割り当てるみたいな
    int8_t cost[90][90];//マス目のコスト
    bool reach_time[90][90];//到達の有無（帰還用）
    int8_t WhichWay(int a,int b){
        if(cost[a][b] - cost[a+1][b] == 1){
            return 1;
        }
        if(cost[a][b] - cost[a][b-1] == 1){
            return 2;
        }
        if(cost[a][b] - cost[a-1][b] == 1){
            return 3;
        }
        if(cost[a][b] - cost[a][b+1] == 1){
            return 4;
        }
        return 0;
    }

public:
    go_home();
    void BFS(int8_t x,int8_t y,int8_t i);
    void WriteDownWall(int8_t x,int8_t y,int8_t i,bool right_wall,bool front_wall,bool left_wall);
    void GoHome(int8_t x,int8_t y);
};

go_home::go_home()
{
    for (int t = 0; t < 90; t++) {
    for (int j = 0; j < 90; j++) {
        kabe_zahyou[t][j] = 100;
        reach_time[t][j] = 0;
        cost[t][j] = 0;
        }
    }
    init();
    st_init();
}


void go_home::BFS(int8_t x,int8_t y,int8_t i)//現在地の座標を取得
{

    int a = x;
    int b = y;
    cost[a][b] = 1;//現在地のコストを1にする

    while(1){

        reach_time[a][b] = 1;//そのマスを訪問済みにする
        for(int n = 1; n <= 4; n++){//そのマスの周りのマスのコストを＋１する
            int result = static_cast<int>(pow(2, n));
            if(kabe_zahyou[a][b] % result == 0){//kabe_zahyou[][]は0000 の4ビットに絶対方向の東8西4南2北1をそれぞれ割り当てる
                switch(i){
                    case 1://北の壁がない
                        if((!reach_time[a][b-1])&&(kabe_zahyou[a][b-1] != 100)){//その先のマスが訪問済みでない&その先のマスが探索済み
                            cost[a][b-1] = cost[a][b] + 1;
                            //キューの末尾に入れる
                            enqueue(a);
                            enqueue(b-1);
                        }
                        break;

                    case 2://南の壁がない
                        if((!reach_time[a][b+1])&&(kabe_zahyou[a][b+1] != 100)){//その先のマスが訪問済みでない&その先のマスが探索済み
                            cost[a][b+1] = cost[a][b] + 1;
                            //キューの末尾に入れる
                            enqueue(a);
                            enqueue(b+1);
                        }
                        break;

                    case 3://西の壁がない
                        if((!reach_time[a-1][b])&&(kabe_zahyou[a-1][b] != 100)){//その先のマスが訪問済みでない&その先のマスが探索済み
                            cost[a-1][b] = cost[a][b] + 1;
                            //キューの末尾に入れる
                            enqueue(a-1);
                            enqueue(b);
                        }
                        break;

                    case 4://東の壁がない
                        if((!reach_time[a+1][b])&&(kabe_zahyou[a+1][b] != 100)){//その先のマスが訪問済みでない&その先のマスが探索済み
                            cost[a+1][b] = cost[a][b] + 1;
                            //キューの末尾に入れる
                            enqueue(a+1);
                            enqueue(b);
                        }
                        break;
                }

            }else{//その方向に壁があるとき
                kabe_zahyou[a][b] = kabe_zahyou[a][b] - result;
            }
        }
        //キューの先頭を取り出す
        a = dequeue();
        b = dequeue();

        if((a == -1)||(b == -1)){//error
            break;
        }

        if((a == 50)&&(b == 50)){
            break;
        }
    }
    //スタックを使って逆探索
    a = 50;
    b = 50;
    while(1){

        switch(i){//東西南北が1234
            case 1://east
                switch(WhichWay(a,b)){//前後左右のどこが最短になるか（ここは）１：右折、２：左折、３：直進

                    case 2://北マスからきたとき（ここのシグナルは探索時の曲がる→進むとは逆で、進む→曲がるじゃないとかも。pushの順番は曲がる、進む）
                        push(2);
                        push(3);
                        b += -1;
                        i = 4;

                    case 3://西マスからきたとき
                        push(3);
                        a += -1;

                    case 4://南マスからきたとき
                        push(1);
                        push(3);
                        b += 1;
                        i = 2;

                }

            case 2://north
                switch(WhichWay(a,b)){//前後左右のどこが最短になるか
                    case 1:
                        push(1);
                        push(3);
                        a += 1;
                        i = 3;

                    case 3:
                        push(2);
                        push(3);
                        a += -1;
                        i = 1;

                    case 4:
                        push(3);
                        b += -1;

                }

            case 3://west
                switch(WhichWay(a,b)){//前後左右のどこが最短になるか
                    case 1:
                        push(3);
                        a += 1;

                    case 2:
                        push(1);
                        push(3);
                        b += -1;
                        i = 4;

                    case 4:
                    push(2);
                    push(3);
                    i = 2;

                }

            case 4://south
                switch(WhichWay(a,b)){//前後左右のどこが最短になるか
                    case 1:
                        push(2);
                        push(3);
                        a += 1;
                        i = 3;

                    case 2:
                        push(3);
                        b += -1;

                    case 3:
                        push(1);
                        push(3);
                        a += -1;
                        i = 1;

                }
        }
        if((a == x)&&(b == y)){
            break;
        }
    }
    
    
}

void go_home::WriteDownWall(int8_t x,int8_t y,int8_t i,bool right_wall,bool front_wall,bool left_wall)
{
    //壁情報の記入(ここは帰還アルゴリズム用の関数)
    if(kabe_zahyou[x][y] == 100){//記録されていない場合（そうしないと延々と加算されちゃう）
        switch (i){
            case 1://east
                if(right_wall){
                    kabe_zahyou[x][y] += 2;
                }
                if(front_wall){
                    kabe_zahyou[x][y] += 8;
                }
                if(left_wall){
                    kabe_zahyou[x][y] += 1;
                }
                break;
            
            case 2://north
                if(right_wall){
                    kabe_zahyou[x][y] += 8;
                }
                if(front_wall){
                    kabe_zahyou[x][y] += 1;
                }
                if(left_wall){
                    kabe_zahyou[x][y] += 4;
                }
                break;

            case 3://west
                if(right_wall){
                    kabe_zahyou[x][y] += 1;
                }
                if(front_wall){
                    kabe_zahyou[x][y] += 4;
                }
                if(left_wall){
                    kabe_zahyou[x][y] += 2;
                }
                break;

            case 4://south
                if(right_wall){
                    kabe_zahyou[x][y] += 4;
                }
                if(front_wall){
                    kabe_zahyou[x][y] += 2;
                }
                if(left_wall){
                    kabe_zahyou[x][y] += 8;
                }
                break;
            }
            //対応
            //e n w s
            //8 4 2 1
        kabe_zahyou[x][y] += -100;
    }
}

void go_home::GoHome(int8_t x,int8_t y)
{
    int GoSignal = 0;
    while(1){
        //ここ以下を「相手から動き終わったという信号が送られたら」とかにしないとバババッて送られちゃうかも（RTOSだから大丈夫かも？）
        GoSignal = pop();
        switch(GoSignal){
            case 1:
                //TurnRight
                break;

            case 2:
                //TurnLeft
                break;

            case 3:
                //GoStraight
                break;
        }
        if ((x == 50)&&(y == 50)){
            break;
        }
    }
}