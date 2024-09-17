
int toutatu_zahyou[90][90];
int kabe_zahyou[90][90];
int x = 50;
int y = 50;
int right_weight = 0;
int front_weight = 0;
int left_weight = 0;
int go_to = 0;
int i = 2;//1:east 2:north 3:west 4:south

int right_reached = 0;//
int left_reached = 0;
int front_reached = 0;

bool right_wall = 0;
bool front_wall = 0;
bool left_wall = 0;

void write_down_wall(i){//壁情報の記入
 switch (i){
    case 1://east
        
        break;
    
    case 2://north

        break;

    case 3://west

        break;

    case 4://south

        break;
    }
}



void judge(i){//重みづけによる拡張右手法

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
    
    return go_to //go_to 1:right 2:front 3:left
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

}