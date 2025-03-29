#include <iostream>
#include <queue>
#include <stack>
#include <chrono>
#include <thread>
#include <cstdint>

using std::cout;
using std::endl;

std::stack<int> S;
std::queue<int> Q;

#define East 1
#define North 2
#define West 3
#define South 4

#define Right 1
#define Front 2
#define Left 3
#define Back 4

int x = 5;
int y = 2;
uint8_t Direction = East;
uint8_t NowDirection = 0;
uint8_t Status = 1;

bool StopFlag = false;

const uint8_t GoalX = 3;
const uint8_t GoalY = 2;

const uint8_t RealGoalX = 3;
const uint8_t RealGoalY = 1;

const uint8_t Xfactor = 7; // 迷路の大きさ
const uint8_t Yfactor = 5;

int cost[Xfactor][Yfactor]; // マス目のコスト
bool reach_time[Xfactor][Yfactor]; // 到達の有無
int kabe_zahyou[Xfactor][Yfactor] = {
    {16, 16, 16, 16, 16},
    {16, 5 , 14, 7 , 16},
    {16, 9 , 12, 2 , 16},
    {16, 7 , 13, 2 , 16},
    {16, 9 , 4 , 2 , 16},
    {16, 13, 10, 11, 16},
    {16, 16, 16, 16, 16}
};

/*int kabe_zahyou[Xfactor][Yfactor] = {
    {16, 16, 16, 16, 16},
    {16, 12, 12, 12, 16},
    {16, 16, 16, 16, 16},
    {16, 16, 16, 16, 16},
    {16, 16, 16, 16, 16},
    {16, 16, 10, 16, 16},
    {16, 16, 16, 16, 16}
};*/

void delay(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

int WhichWay(int a, int b) {
    if ((cost[a][b] - cost[a + 1][b] == 1) && !(kabe_zahyou[a][b] & 8)) {
        return East;
    }
    if ((cost[a][b] - cost[a][b - 1] == 1) && !(kabe_zahyou[a][b] & 1)) {
        return North;
    }
    if ((cost[a][b] - cost[a - 1][b] == 1) && !(kabe_zahyou[a][b] & 4)) {
        return West;
    }
    if ((cost[a][b] - cost[a][b + 1] == 1) && !(kabe_zahyou[a][b] & 2)) {
        return South;
    }
    return 0;
}

void BFS() {
    int a = x;
    int b = y;
    cost[a][b] = 1;
    cout << "GotoHome:" << endl;

    while (!(a == GoalX && b == GoalY)) {
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
    cout << "WalkMappingFinish" << endl;
    //スタックを使って逆探索
    a = GoalX;
    b = GoalY;
    NowDirection = Direction;
    Direction = North;

    /*デバッグ用*/
    /*if(WhichWay(a,b) == 0){
        migi();
    }*/

    S.push(4);//停止用
    cout << "a = " << a << ",b = " << b << endl;
    while(!((a == x) && (b == y))){
        delay(10);
        //cout << "a = " << a << ",b = " << b << ",Direction = " << Direction << endl;
        
        cout << "WhichWay = " << WhichWay(a,b) << endl;
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
                    case East:
                        Direction =North;
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
                        b += 1;/*こここここここ*/
                        break;
                    case North:
                        S.push(2);
                        S.push(2);
                        S.push(3);
                        Direction = South;
                        //Direction = East;
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
                        b += 1;
                        Direction = North;
                        break;
                    case West:
                        Direction =North;
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
                    case South:
                        Direction =North;
                        break;

                }
                break;

            /*デバッグ用*/
            default:
                //Serial.println("Error");
                break;
        }
        cout << "a = " << a << ",b = " << b << endl;
    }
    switch (NowDirection)//1：右折、2：左折、3：直進
    {
    case North:
        switch (Direction)
        {
        case East:
            S.push(1);
            break;

        case South:
            S.push(1);
            S.push(1);
            break;

        case West:
            S.push(2);
            break;
        }
        break;
    
    case East:
        switch (Direction)
        {
        case North:
            S.push(2);
            break;

        case South:
            S.push(1);
            break;

        case West:
            S.push(1);
            S.push(1);
            break;
        }
        break;

    case South:
        switch (Direction)
        {
        case North:
            S.push(1);
            S.push(1);
            break;
        
        case East:
            S.push(2);
            break;

        case West:
            S.push(1);
            break;
        }
        break;

    case West:
        switch (Direction)
        {
        case North:
            S.push(1);
            break;
        
        case East:
            S.push(1);
            S.push(1);
            break;

        case South:
            S.push(2);
            break;
        }
        break;
    }
}

void ForBFSLeftGo(){
    //WriteDownWall
    int BFSWallZahyou = kabe_zahyou[x][y];
    //壁情報の記入(ここは帰還アルゴリズム用の関数)
    /*switch (Direction){
    case East:
      if(right_wall){
        //BFSWallZahyou |= 2;
      }
      if(front_wall){
        BFSWallZahyou += 8;
      }
      if(left_wall){
        BFSWallZahyou += 1;
      }
      break;
            
    case North:
      if(right_wall){
        BFSWallZahyou += 8;
      }
      if(front_wall){
        BFSWallZahyou += 1;
      }
      if(left_wall){
        BFSWallZahyou += 4;
      }
      break;

    case West:
      if(right_wall){
        BFSWallZahyou += 1;
      }
      if(front_wall){
        BFSWallZahyou += 4;
      }
      if(left_wall){
        //BFSWallZahyou |= 2;
      }
      break;

    case South:
      if(right_wall){
        BFSWallZahyou += 4;
      }
      if(front_wall){
        //BFSWallZahyou |= 2;
      }
      if(left_wall){
        BFSWallZahyou += 8;
      }
      break;
    }*/
    //南の壁情報をなくす
    BFSWallZahyou &= ~2;
    kabe_zahyou[RealGoalX][RealGoalY] &= ~2;
    cout << "BFSWallZahyou = " << BFSWallZahyou << endl;
    
    if(BFSWallZahyou == kabe_zahyou[RealGoalX][RealGoalY]){
      StopFlag = true;
      cout << "Finish" << endl;
      return;
    }
    
    //左手法
    uint8_t GoTo = 0;
    bool left_wall = false;
    bool front_wall = false;
    bool right_wall = false;
    switch (Direction)
    {
    case North:
        if(kabe_zahyou[x][y] & 4) {left_wall = true;}
        if(kabe_zahyou[x][y] & 1) {front_wall = true;}
        if(kabe_zahyou[x][y] & 8) {right_wall = true;}
        break;
    
    case East:
        if(kabe_zahyou[x][y] & 1) {left_wall = true;}
        if(kabe_zahyou[x][y] & 8) {front_wall = true;}
        if(kabe_zahyou[x][y] & 2) {right_wall = true;}
        break;

    case South:
        if(kabe_zahyou[x][y] & 8) {left_wall = true;}
        if(kabe_zahyou[x][y] & 2) {front_wall = true;}
        if(kabe_zahyou[x][y] & 4) {right_wall = true;}
        break;

    case West:
        if(kabe_zahyou[x][y] & 2) {left_wall = true;}
        if(kabe_zahyou[x][y] & 4) {front_wall = true;}
        if(kabe_zahyou[x][y] & 1) {right_wall = true;}
        break;
    }


    if     (!left_wall) {GoTo = Left ;}
    else if(!front_wall){GoTo = Front;}
    else if(!right_wall){GoTo = Right;}
    else                {GoTo = Back ;}

    switch (GoTo)
    {
    case Right:
      Status = 4;
      break;

    case Front:
      Status = 2;
      break;

    case Left:
      Status = 3;
      break;

    case Back:
      Status = 5;
      break;
      }
      
}



void GoHome() {
    cout << " " << endl;
    Direction = NowDirection;
    while (!S.empty()) {
        switch (S.top()) {
            case 1:
                cout << "migi" << endl;
                switch (Direction)
                {
                case North:
                  Direction = East;
                  break;
                
                case East:
                  Direction = South;
                  break;

                case South:
                  Direction = West;
                  break;

                case West:
                  Direction = North;
                  break;
                }
                delay(300);
                break;
            case 2:
                cout << "hidari" << endl;
                switch (Direction)
                {
                case North:
                  Direction = West;
                  break;
                
                case West:
                  Direction = South;
                  break;

                case South:
                  Direction = East;
                  break;

                case East:
                  Direction = North;
                  break;
                }
                delay(300);
                break;
            case 3:
                cout << "susumu_heitan" << endl;
                switch (Direction)
                {
                case North:
                  y += -1;
                  break;
                
                case East:
                  x += 1;
                  break;

                case South:
                  y += 1;
                  break;

                case West:
                  x += -1;
                  break;
                }
                delay(300);
                break;
            case 4:
                cout << "Stop" << endl;
                cout << Direction << endl;
                cout << "kabe_zahyou[RealGoalX][RealGoalY] = " << kabe_zahyou[RealGoalX][RealGoalY] << endl;

                //Direction = North;
                while(!StopFlag)
                {
                  switch (Status)
                  {
                  case 1:
                    //WriteDownWall
                    
                    cout << "x = " << x << ",y = " << y << endl;
                    ForBFSLeftGo();
                    break;

                  case 2://直進
                     cout << "susumu_heitan" << endl;
                     switch (Direction)
                     {
                     case North:
                       y += -1;
                       break;
                     
                     case East:
                       x += 1;
                       break;
     
                     case South:
                       y += 1;
                       break;
     
                     case West:
                       x += -1;
                       break;
                     }
                    delay(200);
                    Status = 1;
                    break;
                  
                  case 3://左折
                    cout << "hidari" << endl;
                    switch (Direction)
                    {
                    case North:
                      Direction = West;
                      break;
                    
                    case East:
                      Direction = North;
                      break;
    
                    case South:
                      Direction = East;
                      break;
    
                    case West:
                      Direction = South;
                      break;
                    }
                    delay(500);
                    cout << "susumu_heitan" << endl;
                    switch (Direction)
                    {
                    case North:
                      y += -1;
                      break;
                    
                    case East:
                      x += 1;
                      break;
    
                    case South:
                      y += 1;//
                      break;
    
                    case West:
                      x += -1;
                      break;
                    }
                    delay(200);
                    Status = 1;
                    break;
                  
                  case 4://右折
                    cout << "migi" << endl;
                    switch (Direction)
                    {
                    case North:
                      Direction = East;
                      break;
                    
                    case East:
                      Direction = South;
                      break;
    
                    case South:
                      Direction = West;
                      break;
    
                    case West:
                      Direction = North;
                      break;
                    }
                    delay(500);
                    cout << "susumu_heitan" << endl;
                    switch (Direction)
                    {
                    case North:
                      y += -1;
                      break;
                    
                    case East:
                      x += 1;
                      break;
    
                    case South:
                      y += 1;
                      break;
    
                    case West:
                      x += -1;
                      break;
                    }
                    delay(200);
                    Status = 1;
                    break;
                  
                  case 5://後進
                    cout << "migi" << endl;
                    switch (Direction)
                    {
                    case North:
                      Direction = East;
                      break;
                    
                    case East:
                      Direction = South;
                      break;
    
                    case South:
                      Direction = West;
                      break;
    
                    case West:
                      Direction = North;
                      break;
                    }
                    delay(500);
                    cout << "migi" << endl;
                    switch (Direction)
                    {
                    case North:
                      Direction = East;
                      break;
                    
                    case East:
                      Direction = South;
                      break;
    
                    case South:
                      Direction = West;
                      break;
    
                    case West:
                      Direction = North;
                      break;
                    }
                    delay(500);
                    cout << "susumu_heitan" << endl;
                    switch (Direction)
                    {
                    case North:
                      y += -1;
                      break;
                    
                    case East:
                      x += 1;
                      break;
    
                    case South:
                      y += 1;
                      break;
    
                    case West:
                      x += -1;
                      break;
                    }
                    delay(200);
                    Status = 1;
                    break;
                  }

                }
                
                delay(20000);
                break;
                
        }
        cout << "x = " << x << ",y = " << y << endl;
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
