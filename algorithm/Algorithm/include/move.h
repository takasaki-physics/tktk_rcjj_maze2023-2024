#pragma once

#include <stdio.h>

class move
{
private:
    int8_t go_to = 0;
    int8_t toutatu_zahyou[90][90];//そのマスの到達回数
    int16_t right_weight = 0;
    int16_t front_weight = 0;
    int16_t left_weight = 0;
public:
    move();
    void judge(int8_t x,int8_t y,int8_t i,bool right_wall,bool front_wall,bool left_wall);
    void MoveTo(int8_t x,int8_t y,int8_t i);
    int8_t returnX;
    int8_t returnY;
    int8_t returnI;
};

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

void move::MoveTo(int8_t x,int8_t y,int8_t i)
{
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
    returnX = x;
    returnY = y;
    returnI = i;
    
}
