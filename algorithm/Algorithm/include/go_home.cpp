#include<go_home.h>
#include <StackAndQue.h>
StackAndQue stq;

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

int8_t go_home::WhichWay(int a,int b)
{
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
                            stq.enqueue(a);
                            stq.enqueue(b-1);
                        }
                        break;

                    case 2://南の壁がない
                        if((!reach_time[a][b+1])&&(kabe_zahyou[a][b+1] != 100)){//その先のマスが訪問済みでない&その先のマスが探索済み
                            cost[a][b+1] = cost[a][b] + 1;
                            //キューの末尾に入れる
                            stq.enqueue(a);
                            stq.enqueue(b+1);
                        }
                        break;

                    case 3://西の壁がない
                        if((!reach_time[a-1][b])&&(kabe_zahyou[a-1][b] != 100)){//その先のマスが訪問済みでない&その先のマスが探索済み
                            cost[a-1][b] = cost[a][b] + 1;
                            //キューの末尾に入れる
                            stq.enqueue(a-1);
                            stq.enqueue(b);
                        }
                        break;

                    case 4://東の壁がない
                        if((!reach_time[a+1][b])&&(kabe_zahyou[a+1][b] != 100)){//その先のマスが訪問済みでない&その先のマスが探索済み
                            cost[a+1][b] = cost[a][b] + 1;
                            //キューの末尾に入れる
                            stq.enqueue(a+1);
                            stq.enqueue(b);
                        }
                        break;
                }

            }else{//その方向に壁があるとき
                kabe_zahyou[a][b] = kabe_zahyou[a][b] - result;
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
    while(1){

        switch(i){//東西南北が1234
            case 1://east
                switch(WhichWay(a,b)){//前後左右のどこが最短になるか（ここは）１：右折、２：左折、３：直進

                    case 2://北マスからきたとき（ここのシグナルは探索時の曲がる→進むとは逆で、進む→曲がるじゃないとかも。pushの順番は曲がる、進む）
                        stq.push(2);
                        stq.push(3);
                        b += -1;
                        i = 4;

                    case 3://西マスからきたとき
                        stq.push(3);
                        a += -1;

                    case 4://南マスからきたとき
                        stq.push(1);
                        stq.push(3);
                        b += 1;
                        i = 2;

                }

            case 2://north
                switch(WhichWay(a,b)){//前後左右のどこが最短になるか
                    case 1:
                        stq.push(1);
                        stq.push(3);
                        a += 1;
                        i = 3;

                    case 3:
                        stq.push(2);
                        stq.push(3);
                        a += -1;
                        i = 1;

                    case 4:
                        stq.push(3);
                        b += -1;

                }

            case 3://west
                switch(WhichWay(a,b)){//前後左右のどこが最短になるか
                    case 1:
                        stq.push(3);
                        a += 1;

                    case 2:
                        stq.push(1);
                        stq.push(3);
                        b += -1;
                        i = 4;

                    case 4:
                        stq.push(2);
                        stq.push(3);
                        i = 2;

                }

            case 4://south
                switch(WhichWay(a,b)){//前後左右のどこが最短になるか
                    case 1:
                        stq.push(2);
                        stq.push(3);
                        a += 1;
                        i = 3;

                    case 2:
                        stq.push(3);
                        b += -1;

                    case 3:
                        stq.push(1);
                        stq.push(3);
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
        GoSignal = stq.pop();
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