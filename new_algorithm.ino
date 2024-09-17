
int toutatu_zahyou[90][90];
int kabe_zahyou[90][90];//0000 に絶対値の東8西4南2北1をそれぞれ割り当てる
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

void write_down_wall(){//壁情報の記入(ここは帰還アルゴリズム用の関数)

//
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
        right_weight += 99;
    }
    if (front_wall == 1){
        front_weight += 99;
    }
    if (left_wall == 1){
        left_weight += 99;
    }


    //どこへ行くか決める
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
    
    //return go_to //go_to 1:right 2:front 3:left
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
        }
    }
}

void loop(){
    //壁情報を取得
    judge();//拡張右手法で行く方法を決める
    move();//実際に移動して座標を変更+到達回数を加算
}