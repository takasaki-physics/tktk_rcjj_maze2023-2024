#include <iostream>
#include <queue>
#include <stack>
#include <chrono>
#include <thread>
#include <cstdint>

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

void delay(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

int WhichWay(uint8_t a, uint8_t b) {
    if (cost[a][b] - cost[a + 1][b] == 1) {
        return East;
    }
    if (cost[a][b] - cost[a][b - 1] == 1) {
        return North;
    }
    if (cost[a][b] - cost[a - 1][b] == 1) {
        return West;
    }
    if (cost[a][b] - cost[a][b + 1] == 1) {
        return South;
    }
    return 0;
}

void BFS() {
    uint8_t a = x;
    uint8_t b = y;
    cost[a][b] = 1;
    cout << "GotoHome:" << endl;

    while (!(a == 1 && b == 3)) {
        reach_time[a][b] = 1;
        cout << "a = " << a << ", b = " << b << endl;
        cout << " kabe_zahyou[a][b] = " << kabe_zahyou[a][b] << ", cost[a][b] = " << cost[a][b] << endl;
        for (int n = 1; n <= 8; n *= 2) {
            cout << "n = " << n <<  endl;
            //delay(300);

            if (!(kabe_zahyou[a][b] & n)) {
                switch (n) {
                    case 1:
                        if (!reach_time[a][b - 1] && !(kabe_zahyou[a][b - 1] & 16)) {
                            cost[a][b - 1] = cost[a][b] + 1;
                            Q.push(a);
                            Q.push(b - 1);
                        }
                        break;
                    case 2:
                        if (!reach_time[a][b + 1] && !(kabe_zahyou[a][b + 1] & 16)) {
                            cost[a][b + 1] = cost[a][b] + 1;
                            Q.push(a);
                            Q.push(b + 1);
                        }
                        break;
                    case 4:
                        if (!reach_time[a - 1][b] && !(kabe_zahyou[a - 1][b] & 16)) {
                            cost[a - 1][b] = cost[a][b] + 1;
                            Q.push(a - 1);
                            Q.push(b);
                        }
                        break;
                    case 8:
                        if (!reach_time[a + 1][b] && !(kabe_zahyou[a + 1][b] & 16)) {
                            cost[a + 1][b] = cost[a][b] + 1;
                            Q.push(a + 1);
                            Q.push(b);
                        }
                        break;
                }
            }
        }
        if (Q.empty()) {cout << "ERROR: Queue is empty before popping!" << endl;}

        if (Q.size() < 2) break;
        a = Q.front(); Q.pop();
        b = Q.front(); Q.pop();
    }
    //スタックを使って逆探索
    a = 1;
    b = 3;
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
              cout << "no way" << endl;
        }
        /*if((a == x)&&(b == y)){
            break;
        }*/
    }
}

void GoHome() {
    while (!S.empty()) {
        switch (S.top()) {
            case 1:
                cout << "migi" << endl;
                delay(300);
                break;
            case 2:
                cout << "hidari" << endl;
                delay(300);
                break;
            case 3:
                cout << "susumu_heitan" << endl;
                delay(300);
                break;
            case 4:
                cout << "Stop" << endl;
                delay(20000);
                break;
        }
        S.pop();
    }
}

int main() {
    for (int t = 0; t < Xfactor; t++) {
        for (int j = 0; j < Yfactor; j++) {
            //kabe_zahyou[t][j] = 0;
            reach_time[t][j] = 0;
            cost[t][j] = 0;
        }
    }

    BFS();
    cout << "BFS end" << endl;
    GoHome();

    return 0;
}
