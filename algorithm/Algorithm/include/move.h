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

