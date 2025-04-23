/*帰還用変数*/
#include <stdio.h>
#include <cstdint>
#include <Arduino.h>
#include <time.h>
#include <cstdint>
#include <queue>
#include <stack>
using namespace std;

std::stack<uint8_t> S;
std::queue<uint8_t> Q;

#define East 1
#define North 2
#define West 3
#define South 4

#define Right 1
#define Front 2
#define Left 3
#define Back 4

uint8_t x = 4;
uint8_t y = 1;
uint8_t Direction = South;

const uint8_t Xfactor = 7; // 迷路の大きさ
const uint8_t Yfactor = 5;

int cost[Xfactor][Yfactor]; // マス目のコスト
bool reach_time[Xfactor][Yfactor]; // 到達の有無
int kabe_zahyou[Xfactor][Yfactor] = {
    {16, 16, 16, 16, 16},
    {16, 5 , 14, 7 , 16},
    {16, 9 , 12, 2 , 16},
    {16, 7 , 16, 2 , 16},
    {16, 9 , 4 , 2 , 16},
    {16, 13, 10, 11, 16},
    {16, 16, 16, 16, 16}
};
long now_seconds;
long firstseconds;

/*******************************************************************************************/
/* 最短経路の方位                                                                              
/*処理：現在のマスのコストの、-1のコストのマスを探す
/*更新者：吉ノ薗2025/01/22
/*
/*******************************************************************************************/
int WhichWay(uint8_t a,uint8_t b)
{
    if(cost[a][b] - cost[a+1][b] == 1){
        return East;
       }
    if(cost[a][b] - cost[a][b-1] == 1){
        return North;
    }
    if(cost[a][b] - cost[a-1][b] == 1){
        return West;
    }
    if(cost[a][b] - cost[a][b+1] == 1){
        return South;
    }
    return 0;
}


void BFS()
{
    //hidari();//デバッグ用
    uint8_t a = x;
    uint8_t b = y;
    cost[a][b] = 1;//現在地のコストを1にする
    Serial.print("%d GotoHome:");

    while(!(a == 50 && b == 50)){

        reach_time[a][b] = 1;//そのマスを訪問済みにする
        Serial.print("%d a =");
        Serial.print(a);
        Serial.print("%d b =");
        Serial.print(b);

        Serial.print("%d kabe_zahyou[a][b] ==");
        Serial.print(kabe_zahyou[a][b]);

        Serial.print("%d cost[a][b] == ");
        Serial.print(cost[a][b]);
        
        for(int n = 1; n <= 8; n *= 2){//そのマスの周りのマスのコストを＋１する
            //int result = static_cast<int>(pow(2, n));
            Serial.print("%d n =");
            Serial.print(n);

            if(!(kabe_zahyou[a][b] & n)) {//kabe_zahyou[][]は0000 の4ビットに絶対方向の東8西4南2北1をそれぞれ割り当てる

                switch(n){
                    case 1://北の壁がない
                        if((!reach_time[a][b-1]) && !(kabe_zahyou[a][b-1] & 16)){//その先のマスが訪問済みでない&その先のマスが探索済み
                            cost[a][b-1] = cost[a][b] + 1;
                            //キューの末尾に入れる
                            Q.push(a);
                            Q.push(b-1);
                        }
                        break;

                    case 2://南の壁がない
                        if((!reach_time[a][b+1]) && !(kabe_zahyou[a][b+1] & 16)){//その先のマスが訪問済みでない&その先のマスが探索済み
                            cost[a][b+1] = cost[a][b] + 1;
                            //キューの末尾に入れる
                            Q.push(a);
                            Q.push(b+1);
                        }
                        break;

                    case 4://西の壁がない
                        if((!reach_time[a-1][b]) && !(kabe_zahyou[a-1][b] & 16)){//その先のマスが訪問済みでない&その先のマスが探索済み
                            cost[a-1][b] = cost[a][b] + 1;
                            //キューの末尾に入れる
                            Q.push(a-1);
                            Q.push(b);
                        }
                        break;

                    case 8://東の壁がない
                        if((!reach_time[a+1][b]) && !(kabe_zahyou[a+1][b] & 16)) {//その先のマスが訪問済みでない&その先のマスが探索済み
                            cost[a+1][b] = cost[a][b] + 1;
                            //キューの末尾に入れる
                            Q.push(a+1);
                            Q.push(b);
                        }
                        break;
                }

            }
            /*else{//その方向に壁があるとき
                kabe_zahyou[a][b] &= ~n;  // n のビットを 0 にする（壁を削除）これビットだしいらない気がする
            }*/
        }
        //キューの先頭を取り出す
        if (Q.size() < 2) break;  // キューの要素が足りない場合は終了

        a = Q.front(); Q.pop();
        b = Q.front(); Q.pop();

        //if (Q.empty()) break;

        /*if((a == 50)&&(b == 50)){
            break;
        }*/
    }



    //スタックを使って逆探索
    a = 50;
    b = 50;
    Direction = North;

    /*デバッグ用*/
    /*if(WhichWay(a,b) == 0){
        migi();
    }*/

    S.push(4);//停止用
    while(!((a == x) && (b == y))){

        switch(Direction){
            case East:
                switch(WhichWay(a,b)){//前後左右のどこが最短になるか

                    case North://北マスからきたとき（ここのシグナルは探索時の曲がる→進むとは逆で、進む→曲がるじゃないと。）
                        S.push(2);//１：右折、２：左折、３：直進
                        S.push(3);
                        b += -1;
                        Direction = South;
                        break;
                    case West://西マスからきたとき
                        S.push(3);
                        a += -1;
                        break;

                    case South://南マスからきたとき
                        S.push(1);
                        S.push(3);
                        b += 1;
                        Direction = North;
                        break;

                }
                break;

            case North:
                switch(WhichWay(a,b)){//前後左右のどこが最短になるか
                    case East:
                        S.push(1);
                        S.push(3);
                        a += 1;
                        Direction = West;
                        break;

                    case West:
                        S.push(2);
                        S.push(3);
                        a += -1;
                        Direction = East;
                        break;

                    case South:
                        S.push(3);
                        b += -1;
                        break;

                }
                break;

            case West:
                switch(WhichWay(a,b)){//前後左右のどこが最短になるか
                    case East:
                        S.push(3);
                        a += 1;
                        break;

                    case North:
                        S.push(1);
                        S.push(3);
                        b += -1;
                        Direction = South;
                        break;

                    case South:
                        S.push(2);
                        S.push(3);
                        Direction = North;
                        break;

                }
                break;

            case South:
                switch(WhichWay(a,b)){//前後左右のどこが最短になるか
                    case East:
                        S.push(2);
                        S.push(3);
                        a += 1;
                        Direction = West;
                        break;

                    case North:
                        S.push(3);
                        b += -1;
                        break;

                    case West:
                        S.push(1);
                        S.push(3);
                        a += -1;
                        Direction = East;
                        break;

                }
                break;

            /*デバッグ用*/
            default:
                Serial.println("Error");
                break;
        }
        /*if((a == x)&&(b == y)){
            break;
        }*/
    }
}

/*******************************************************************************************/
/* 帰還                                                                             
/*処理：スタックから値をpopして順番に動いていく
/*      全て終わる、つまりpopした値が一番最初に入れた"4"になっていたら停止（まだ停止部分はつくってない）
/*
/*更新者：吉ノ薗2025/01/22
/*　　　　吉ノ薗2025/01/29：20秒停止するようにした
/*       吉ノ薗2025/03/26 whileの条件を「スタックが空でなかった場合」に変更
/*
/*******************************************************************************************/
void GoHome()
{
        //ここ以下を「相手(モーター)から動き終わったという信号が送られたら」とかにしないとバババッて送られちゃうかも
    while(!S.empty()){
        switch(S.top()){
            case 1:
                //TurnRight
                migi();
                delay(300);
                break;

            case 2:
                //TurnLeft
                hidari();
                delay(300);
                break;

            case 3:
                //GoStraight
                susumu_heitan();
                delay(300);
                break;
            case 4:
                //Stop
                /*ここに壁情報が合ってないとうろうろさせるコード入れる*/
                /*壁情報取得　→　kabe_zahyou[50][50]と照合　→　会ってなかったら(x,y)=(51,50),(51,51),(50,51),(49,51),(49.50),(49,49),(50,49),(51,49)をそれぞれBFSで行って照合（いけなかったらどうするん）*/
                delay(20000);
                break;
        }
        S.pop();//要素の削除
    }
    
}

void setup(){
    for (int t = 0; t < Xfactor; t++) {
        for (int j = 0; j < Yfactor; j++) {
            //kabe_zahyou[t][j] = 0;
            reach_time[t][j] = 0;
            cost[t][j] = 0;
        }
    }
}

void loop(){
    BFS();
    GoHome();
}