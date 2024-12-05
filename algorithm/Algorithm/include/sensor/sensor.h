#pragma once
#include <stdio.h>

class sensor
{
private:
    
public:
    sensor();
    bool right_wall = false;//ここもセンサーの値を取得するファイルとして分けたい
    bool front_wall = false;
    bool left_wall = false;
    bool BlackTile = false;
    bool BlueTile = false;
    bool Slope = false;
};

sensor::sensor()
{
}

