#include <Algorithm\go_home.h>
#include <Algorithm\move.h>
#include <sensor/move_2024_2025.h>
#include <Algorithm\StackAndQue.h>

StackAndQue stq;

extern uint8_t x;
extern uint8_t y;
extern uint8_t Direction;
extern uint8_t Status;
extern move_2024_2025 S;

go_home::go_home()
{
    for (int t = 0; t < 90; t++) {
    for (int j = 0; j < 90; j++) {
        kabe_zahyou[t][j] = 100;
        reach_time[t][j] = 0;
        cost[t][j] = 0;
        }
    }
}

int8_t go_home::WhichWay(int a,int b)//前後左右のどこが最短になるか
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

void go_home::BFS()//現在地の座標を取得
{

    int a = x;
    int b = y;
    cost[a][b] = 1;//現在地のコストを1にする

    while(1){

        reach_time[a][b] = 1;//そのマスを訪問済みにする
        for(int n = 2; n <= 16; n *= 2){//そのマスの周りのマスのコストを＋１する
            //int result = static_cast<int>(pow(2, n));
            if(kabe_zahyou[a][b] % n == 0){//kabe_zahyou[][]は0000 の4ビットに絶対方向の東8西4南2北1をそれぞれ割り当てる
                switch(n){
                    case 2://北の壁がない
                        if((!reach_time[a][b-1])&&(kabe_zahyou[a][b-1] != 100)){//その先のマスが訪問済みでない&その先のマスが探索済み
                            cost[a][b-1] = cost[a][b] + 1;
                            //キューの末尾に入れる
                            stq.enqueue(a);
                            stq.enqueue(b-1);
                        }
                        break;

                    case 4://南の壁がない
                        if((!reach_time[a][b+1])&&(kabe_zahyou[a][b+1] != 100)){//その先のマスが訪問済みでない&その先のマスが探索済み
                            cost[a][b+1] = cost[a][b] + 1;
                            //キューの末尾に入れる
                            stq.enqueue(a);
                            stq.enqueue(b+1);
                        }
                        break;

                    case 8://西の壁がない
                        if((!reach_time[a-1][b])&&(kabe_zahyou[a-1][b] != 100)){//その先のマスが訪問済みでない&その先のマスが探索済み
                            cost[a-1][b] = cost[a][b] + 1;
                            //キューの末尾に入れる
                            stq.enqueue(a-1);
                            stq.enqueue(b);
                        }
                        break;

                    case 16://東の壁がない
                        if((!reach_time[a+1][b])&&(kabe_zahyou[a+1][b] != 100)){//その先のマスが訪問済みでない&その先のマスが探索済み
                            cost[a+1][b] = cost[a][b] + 1;
                            //キューの末尾に入れる
                            stq.enqueue(a+1);
                            stq.enqueue(b);
                        }
                        break;
                }

            }else{//その方向に壁があるとき
                kabe_zahyou[a][b] = kabe_zahyou[a][b] - (n/2);
            }
        }
        //キューの先頭を取り出す
        a = stq.dequeue();
        b = stq.dequeue();

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
    stq.push(4);//停止用
    while(1){

        switch(Direction){
            case East:
                switch(WhichWay(a,b)){//前後左右のどこが最短になるか１：右折、２：左折、３：直進

                    case North://北マスからきたとき（ここのシグナルは探索時の曲がる→進むとは逆で、進む→曲がるじゃないとかも。pushの順番は曲がる、進む(これpop?)）
                        stq.push(2);
                        stq.push(3);
                        b += -1;
                        Direction = South;

                    case West://西マスからきたとき
                        stq.push(3);
                        a += -1;

                    case South://南マスからきたとき
                        stq.push(1);
                        stq.push(3);
                        b += 1;
                        Direction = North;

                }

            case North:
                switch(WhichWay(a,b)){//前後左右のどこが最短になるか
                    case East:
                        stq.push(1);
                        stq.push(3);
                        a += 1;
                        Direction = West;

                    case West:
                        stq.push(2);
                        stq.push(3);
                        a += -1;
                        Direction = East;

                    case South:
                        stq.push(3);
                        b += -1;

                }

            case West:
                switch(WhichWay(a,b)){//前後左右のどこが最短になるか
                    case East:
                        stq.push(3);
                        a += 1;

                    case North:
                        stq.push(1);
                        stq.push(3);
                        b += -1;
                        Direction = South;

                    case South:
                        stq.push(2);
                        stq.push(3);
                        Direction = North;

                }

            case South:
                switch(WhichWay(a,b)){//前後左右のどこが最短になるか
                    case East:
                        stq.push(2);
                        stq.push(3);
                        a += 1;
                        Direction = West;

                    case North:
                        stq.push(3);
                        b += -1;

                    case West:
                        stq.push(1);
                        stq.push(3);
                        a += -1;
                        Direction = East;

                }
        }
        if((a == x)&&(b == y)){
            break;
        }
    }
    
    
}

void go_home::WriteDownWall()
{
    //壁情報の記入(ここは帰還アルゴリズム用の関数)
    if(kabe_zahyou[x][y] == 100){//記録されていない場合（そうしないと延々と加算されちゃう）
        switch (Direction){
            case East:
                if(S.right_wall){
                    kabe_zahyou[x][y] += 2;
                }
                if(S.front_wall){
                    kabe_zahyou[x][y] += 8;
                }
                if(S.left_wall){
                    kabe_zahyou[x][y] += 1;
                }
                break;
            
            case North:
                if(S.right_wall){
                    kabe_zahyou[x][y] += 8;
                }
                if(S.front_wall){
                    kabe_zahyou[x][y] += 1;
                }
                if(S.left_wall){
                    kabe_zahyou[x][y] += 4;
                }
                break;

            case West:
                if(S.right_wall){
                    kabe_zahyou[x][y] += 1;
                }
                if(S.front_wall){
                    kabe_zahyou[x][y] += 4;
                }
                if(S.left_wall){
                    kabe_zahyou[x][y] += 2;
                }
                break;

            case South:
                if(S.right_wall){
                    kabe_zahyou[x][y] += 4;
                }
                if(S.front_wall){
                    kabe_zahyou[x][y] += 2;
                }
                if(S.left_wall){
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

void go_home::GoHome()
{
        //ここ以下を「相手(モーター)から動き終わったという信号が送られたら」とかにしないとバババッて送られちゃうかも（RTOSだから大丈夫かも？）
    while(1){
        switch(stq.pop()){
            case 1:
                //TurnRight
                S.migi();
                break;

            case 2:
                //TurnLeft
                S.hidari();
                break;

            case 3:
                //GoStraight
                S.susumu_heitan();
                break;
            case 4:
                //Stop

                break;
        }
    }
    
}