#pragma once
#include <stdio.h>
#include <sensor/move_2024_2025.cpp>
move_2024_2025 Move;
#define East 1
#define North 2
#define West 3
#define South 4

#define Right 1
#define Front 2
#define Left 3
#define Back 4
class move
{
private:
    int8_t toutatu_zahyou[90][90];//そのマスの到達回数
    int16_t RightWeight = 0;
    int16_t FrontWeight = 0;
    int16_t LeftWeight = 0;
public:
    int8_t returnX;
    int8_t returnY;
    int8_t returnI;
    move();
    int8_t judge(int8_t x,int8_t y,int8_t i);
    void MoveTo(int8_t x,int8_t y,int8_t i,int8_t GoTo);
};

