#pragma once
#include <stdio.h>
#include <cstdint>
//#include <cmath>
//#include <iostream> 後でオンにする


class go_home
{
private:

    int8_t kabe_zahyou[90][90];//0000 の4ビットに絶対方向の東8西4南2北1をそれぞれ割り当てるみたいな
    int8_t cost[90][90];//マス目のコスト
    bool reach_time[90][90];//到達の有無（帰還用）
    int8_t WhichWay(int a,int b);

public:
    go_home();
    void BFS();
    void WriteDownWall();
    void GoHome();
};