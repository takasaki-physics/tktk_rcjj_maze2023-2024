
#include <stdio.h>
#include <Arduino.h>
#include <time.h>
#include <Algorithm\go_home.cpp>
#include <Algorithm\move.h>
#include <Algorithm\move.cpp>

go_home Gh;
move M;
long firstseconds;
uint8_t x = 50;
uint8_t y = 50;
uint8_t Direction = North;

void setup(){
    // 現在の時刻を取得
    time_t FirstTime;
    time(&FirstTime);  // 現在の時刻を取得して FirstTime に格納
    firstseconds = static_cast<long>(FirstTime); 
    
}

void loop(){
    //壁情報を取得
    Gh.WriteDownWall();//帰還用の記録
    M.MoveTo(M.judge());//拡張右手法で行く方法を決める,実際に移動して座標を変更+到達回数を加


    // 現在の時刻を取得
    time_t NowTime;
    time(&NowTime);  // 現在の時刻を取得して NowTime に格納
    long now_seconds = static_cast<long>(NowTime); 
    //330秒（＝5分半）経ったら幅優先探索を始める
    if(now_seconds - firstseconds >= 330){
        Gh.BFS();
        Gh.GoHome();
        //停止
    }
}