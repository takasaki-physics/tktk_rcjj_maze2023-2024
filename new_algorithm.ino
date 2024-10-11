
#include <stdio.h>
#include <cmath>
#include <iostream>
int toutatu_zahyou[90][90];
int kabe_zahyou[90][90];//0000 の4ビットに絶対方向の東8西4南2北1をそれぞれ割り当てるみたいな
int cost[90][90];//マス目のコスト
bool reach_time[90][90];//到達の有無（帰還用）
int x = 50;
int y = 50;
int right_weight = 0;
int front_weight = 0;
int left_weight = 0;
int go_to = 0;
int i = 2;//1:east 2:north 3:west 4:south

bool right_wall = 0;
bool front_wall = 0;
bool left_wall = 0;

const int MAX = 1000; // キュー配列の最大サイズ

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

void write_down_wall(){
    //壁情報の記入(ここは帰還アルゴリズム用の関数)
    if(kabe_zahyou[x][y] == 0){//記録されていない場合（そうしないと延々と加算されちゃう）
        switch (i){
            case 1://east
                if(right_wall == 1){
                    kabe_zahyou[x][y] += 2;
                }
                if(front_wall == 1){
                    kabe_zahyou[x][y] += 8;
                }
                if(left_wall == 1){
                    kabe_zahyou[x][y] += 1;
                }
                break;
            
            case 2://north
                if(right_wall == 1){
                    kabe_zahyou[x][y] += 8;
                }
                if(front_wall == 1){
                    kabe_zahyou[x][y] += 1;
                }
                if(left_wall == 1){
                    kabe_zahyou[x][y] += 4;
                }
                break;

            case 3://west
                if(right_wall == 1){
                    kabe_zahyou[x][y] += 1;
                }
                if(front_wall == 1){
                    kabe_zahyou[x][y] += 4;
                }
                if(left_wall == 1){
                    kabe_zahyou[x][y] += 2;
                }
                break;

            case 4://south
                if(right_wall == 1){
                    kabe_zahyou[x][y] += 4;
                }
                if(front_wall == 1){
                    kabe_zahyou[x][y] += 2;
                }
                if(left_wall == 1){
                    kabe_zahyou[x][y] += 8;
                }
                break;
            }
            //対応
            //e n w s
            //8 4 2 1
    }
}


void BFS(){
    int a = x;
    int b = y;
    cost[a][b] = 1;//現在地のコストを1にする

    while(1){

        reach_time[a][b] = 1;//そのマスを訪問済みにする

        for(int i = 1; i <= 4; i++){//そのマスの周りのマスのコストを＋１する
            int result = static_cast<int>(pow(2, i));
            if(kabe_zahyou[a][b] % result == 0){//kabe_zahyou[][]は0000 の4ビットに絶対方向の東8西4南2北1をそれぞれ割り当てる
                switch(i){
                    case 1://北の壁がない
                        if(!reach_time[a][b-1]){//その先のマスが訪問済みでない
                            cost[a][b-1] = cost[a][b] + 1;
                            //キューの末尾に入れる
                            enqueue(a);
                            enqueue(b-1);
                        }
                        break;

                    case 2://南の壁がない
                        if(!reach_time[a][b+1]){//その先のマスが訪問済みでない
                            cost[a][b+1] = cost[a][b] + 1;
                            //キューの末尾に入れる
                            enqueue(a);
                            enqueue(b+1);
                        }
                        break;

                    case 3://西の壁がない
                        if(!reach_time[a-1][b]){//その先のマスが訪問済みでない
                            cost[a-1][b] = cost[a][b] + 1;
                            //キューの末尾に入れる
                            enqueue(a-1);
                            enqueue(b);
                        }
                        break;

                    case 4://東の壁がない
                        if(!reach_time[a+1][b]){//その先のマスが訪問済みでない
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
    //右のマスのコストとの差が１の場合
    if(cost[a][b] - cost[a+1][b] == 1){
        push(2);//曲がるについてどうすればいいかわかんね～～
    }
    
    
}


void judge(){//重みづけによる拡張右手法

    switch (i){//向きによって重みをつける
        case 1://east
            right_weight = toutatu_zahyou[x][y+1] * 5 + 1;
            front_weight = toutatu_zahyou[x+1][y] * 5 + 2;
            left_weight = toutatu_zahyou[x][y-1] * 5 + 3;
            break;

        case 2://north
            right_weight = toutatu_zahyou[x+1][y] * 5 + 1;
            front_weight = toutatu_zahyou[x][y-1] * 5 + 2;
            left_weight = toutatu_zahyou[x-1][y] * 5 + 3;
            break;

        case 3://west
            right_weight = toutatu_zahyou[x][y-1] * 5 + 1;
            front_weight = toutatu_zahyou[x-1][y] * 5 + 2;
            left_weight = toutatu_zahyou[x][y+1] * 5 + 3;
            break;

        case 4://south
            right_weight = toutatu_zahyou[x-1][y] * 5 + 1;
            front_weight = toutatu_zahyou[x][y+1] * 5 + 2;
            left_weight = toutatu_zahyou[x+1][y] * 5 + 3;
            break;

    }

    //壁がある場合更にプラスする
    if (right_wall == 1){
        right_weight += 100;
    }
    if (front_wall == 1){
        front_weight += 100;
    }
    if (left_wall == 1){
        left_weight += 100;
    }


    //どこへ行くか決める
    int n = 0;
    if (right_weight < front_weight){
        n = right_weight;
        go_to = 1;
    }else{
        n = front_weight;
        go_to = 2;
    }
    if (left_weight < n){
        n = left_weight;
        go_to = 3;
    }

    if ((right_weight > 100) && (front_weight > 100) && (left_weight > 100)){//if all wall
        go_to = 4;
    }

    right_weight = 0;//怖いから初期化
    front_weight = 0;
    left_weight = 0;
    
    //return go_to //go_to 1:right 2:front 3:left 4:back
}

void move(){
    switch (i){
        case 1://east
            switch (go_to){
                case 1://right
                    //send right moving signal
                    y += 1;
                    i = 4;
                    break;
                
                case 2://front
                    //send front moving signal
                    x += 1;
                    i = 1;
                    break;
                
                case 3://left
                    //send left moving signal
                    y += -1;
                    i = 2;
                    break;

                case 4://back
                    //send return moving signal
                    toutatu_zahyou[x][y] += 100;//行き止まりだから効率化のため二度と行かないようにする
                    x += -1;
                    i = 3;
                    break;
                }
            break;
        
        case 2://north
            switch (go_to){
                case 1://right
                    //send right moving signal
                    x += 1;
                    i = 1;
                    break;
                
                case 2://front
                    //send front moving signal
                    y += -1;
                    i = 2;
                    break;
                
                case 3://left
                    //send left moving signal
                    x += -1;
                    i = 3;
                    break;

                case 4://back
                    //send return moving signal
                    toutatu_zahyou[x][y] += 100;//行き止まりだから効率化のため二度と行かないようにする
                    y += -1;
                    i = 4;
                    break;
                }
            break;
        
        case 3://west
            switch (go_to){
                case 1://right
                    //send right moving signal
                    y += -1;
                    i = 2;
                    break;
                
                case 2://front
                    //send front moving signal
                    x += -1;
                    i = 3;
                    break;
                
                case 3://left
                    //send left moving signal
                    y += 1;
                    i = 4;
                    break;
                
                case 4://back
                    //send return moving signal
                    toutatu_zahyou[x][y] += 100;//行き止まりだから効率化のため二度と行かないようにする
                    x += 1;
                    i = 1;
                    break;
                }
            break;
        
        case 4://south
            switch (go_to){
                case 1://right
                    //send right moving signal
                    x += -1;
                    i = 3;
                    break;
                
                case 2://front
                    //send front moving signal
                    y += 1;
                    i = 4;
                    break;
                
                case 3://left
                    //send left moving signal
                    x += 1;
                    i = 1;
                    break;
                
                case 4://back
                    //send return moving signal
                    toutatu_zahyou[x][y] += 100;//行き止まりだから効率化のため二度と行かないようにする
                    y += -1;
                    i = 2;
                    break;
                }
            break;
    }
    toutatu_zahyou[x][y] += 1;//移動先のマスの到達回数をプラスしている
}




void setup(){
    for (int t = 0; t < 90; t++) {
    for (int j = 0; j < 90; j++) {
        toutatu_zahyou[t][j] = 0;  //make it to begining 
        kabe_zahyou[t][j] = 0;
        reach_time[t][j] = 0;
        cost[t][j] = 0;
        }
    }
    init();
    st_init();
}

void loop(){
    //壁情報を取得
    judge();//拡張右手法で行く方法を決める
    move();//実際に移動して座標を変更+到達回数を加算

    if((x == 50)&&(y == 50)){//動いた後に判定だから大丈夫なはず
        //stop
    }
}