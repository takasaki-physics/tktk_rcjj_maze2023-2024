
#include <stdio.h>
#include <arduino.h>
#include <time.h>
#include <go_home.cpp>
#include <move.h>
#include <move.cpp>

go_home Gh;
move M;
int8_t x = 50;
int8_t y = 50;
int8_t i = North ;//1:east 2:north 3:west 4:south
long firstseconds;

bool right_wall = false;//ここもセンサーの値を取得するファイルとして分けたい
bool front_wall = false;
bool left_wall = false;



void setup(){
    // 現在の時刻を取得
    time_t FirstTime;
    time(&FirstTime);  // 現在の時刻を取得して FirstTime に格納
    firstseconds = static_cast<long>(FirstTime); 
    
}

void loop(){
    //壁情報を取得
    M.judge(x,y,i,right_wall,front_wall,left_wall);//拡張右手法で行く方法を決める
    Gh.WriteDownWall(x,y,i,right_wall,front_wall,left_wall);//帰還用の記録
    M.MoveTo(x,y,i);//実際に移動して座標を変更+到達回数を加算
    x = M.returnX;
    y = M.returnY;
    i = M.returnI;


    // 現在の時刻を取得
    time_t NowTime;
    time(&NowTime);  // 現在の時刻を取得して NowTime に格納
    long now_seconds = static_cast<long>(NowTime); 
    //330秒（＝5分半）経ったら幅優先探索を始める
    if(now_seconds - firstseconds >= 330){
        Gh.BFS(x,y,i);
        Gh.GoHome(x,y);
        //停止
    }
}