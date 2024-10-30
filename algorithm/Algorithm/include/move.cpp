#include <move.h>

move::move()
{
    for (int t = 0; t < 90; t++) {
    for (int j = 0; j < 90; j++) {
        toutatu_zahyou[t][j] = 0; 
        }
    }
}



void move::judge(int8_t x,int8_t y,int8_t i,bool right_wall,bool front_wall,bool left_wall)
{
    
    switch (i){//向きによって重みをつける
        case East:
            right_weight = toutatu_zahyou[x][y+1] * 5 + 1;
            front_weight = toutatu_zahyou[x+1][y] * 5 + 2;
            left_weight = toutatu_zahyou[x][y-1] * 5 + 3;
            break;

        case North:
            right_weight = toutatu_zahyou[x+1][y] * 5 + 1;
            front_weight = toutatu_zahyou[x][y-1] * 5 + 2;
            left_weight = toutatu_zahyou[x-1][y] * 5 + 3;
            break;

        case West:
            right_weight = toutatu_zahyou[x][y-1] * 5 + 1;
            front_weight = toutatu_zahyou[x-1][y] * 5 + 2;
            left_weight = toutatu_zahyou[x][y+1] * 5 + 3;
            break;

        case South:
            right_weight = toutatu_zahyou[x-1][y] * 5 + 1;
            front_weight = toutatu_zahyou[x][y+1] * 5 + 2;
            left_weight = toutatu_zahyou[x+1][y] * 5 + 3;
            break;

    }

    //壁がある場合更にプラスする
    if (right_wall){
        right_weight += 100;
    }
    if (front_wall){
        front_weight += 100;
    }
    if (left_wall){
        left_weight += 100;
    }


    //どこへ行くか決める
    int n = 0;
    if (right_weight < front_weight){
        n = right_weight;
        go_to = Right;
    }else{
        n = front_weight;
        go_to = Front;
    }
    if (left_weight < n){
        n = left_weight;
        go_to = Left;
    }

    if ((right_weight > 100) && (front_weight > 100) && (left_weight > 100)){//if all wall
        go_to = Back;
    }

    right_weight = 0;//怖いから初期化
    front_weight = 0;
    left_weight = 0;
    
    //return go_to;いらない？ //go_to 1:right 2:front 3:left 4:back
}

void move::MoveTo(int8_t x,int8_t y,int8_t i)
{
switch (i){
        case East:
            switch (go_to){
                case Right:
                    //send right moving signal
                    y += 1;
                    i = 4;
                    break;
                
                case Front:
                    //send front moving signal
                    x += 1;
                    i = 1;
                    break;
                
                case Left:
                    //send left moving signal
                    y += -1;
                    i = 2;
                    break;

                case Back:
                    //send return moving signal
                    toutatu_zahyou[x][y] += 100;//行き止まりだから効率化のため二度と行かないようにする
                    x += -1;
                    i = 3;
                    break;
                }
            break;
        
        case North:
            switch (go_to){
                case Right:
                    //send right moving signal
                    x += 1;
                    i = 1;
                    break;
                
                case Front:
                    //send front moving signal
                    y += -1;
                    i = 2;
                    break;
                
                case Left:
                    //send left moving signal
                    x += -1;
                    i = 3;
                    break;

                case Back:
                    //send return moving signal
                    toutatu_zahyou[x][y] += 100;//行き止まりだから効率化のため二度と行かないようにする
                    y += -1;
                    i = 4;
                    break;
                }
            break;
        
        case West:
            switch (go_to){
                case Right:
                    //send right moving signal
                    y += -1;
                    i = 2;
                    break;
                
                case Front:
                    //send front moving signal
                    x += -1;
                    i = 3;
                    break;
                
                case Left:
                    //send left moving signal
                    y += 1;
                    i = 4;
                    break;
                
                case Back:
                    //send return moving signal
                    toutatu_zahyou[x][y] += 100;//行き止まりだから効率化のため二度と行かないようにする
                    x += 1;
                    i = 1;
                    break;
                }
            break;
        
        case South:
            switch (go_to){
                case Right:
                    //send right moving signal
                    x += -1;
                    i = 3;
                    break;
                
                case Front:
                    //send front moving signal
                    y += 1;
                    i = 4;
                    break;
                
                case Left:
                    //send left moving signal
                    x += 1;
                    i = 1;
                    break;
                
                case Back:
                    //send return moving signal
                    toutatu_zahyou[x][y] += 100;//行き止まりだから効率化のため二度と行かないようにする
                    y += -1;
                    i = 2;
                    break;
                }
            break;
    }
    toutatu_zahyou[x][y] += 1;//移動先のマスの到達回数をプラスしている
    returnX = x;
    returnY = y;
    returnI = i;
    
}
