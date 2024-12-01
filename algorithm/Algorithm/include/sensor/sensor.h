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
};

sensor::sensor()
{
}

