
#include <stdio.h>
#include <Arduino.h>
#include <time.h>
#include <Algorithm\go_home.cpp>
#include <Algorithm\move.h>
#include <Algorithm\move.cpp>
#include <sensor/move_2024_2025.cpp>

move_2024_2025 Move;
go_home Gh;
move M;
long firstseconds;
uint8_t x = 50;
uint8_t y = 50;
uint8_t Direction = North;
uint8_t Status = 0;

void setup(){
    // 現在の時刻を取得
    time_t FirstTime;
    time(&FirstTime);  // 現在の時刻を取得して FirstTime に格納
    firstseconds = static_cast<long>(FirstTime); 
    
}

void loop(){

    switch (Status)
    {
    case 0://壁情報取得
        //get_tof_data();
        break;

    case 1://座標更新と探索
        Gh.WriteDownWall();//帰還用の記録
        M.MoveTo(M.judge());//拡張右手法で行く方法を決める,実際に移動して座標を変更+到達回数を加算

        // 現在の時刻を取得
        time_t NowTime;
        time(&NowTime);  // 現在の時刻を取得して NowTime に格納
        long now_seconds = static_cast<long>(NowTime); 

        //330秒（＝5分半）経ったら幅優先探索を始める
        if(now_seconds - firstseconds >= 330){
            Status = 2;//帰還開始
        }
        break;
    
    case 2://帰還(このとき探索に戻らないよう入れ子構造にする or ここだけは関数内に直接migi()とかを入れてwhile文)   
        Gh.BFS();
        Gh.GoHome();
        break;

    case 3://直進
        Move.susumu_heitan();
        break;

    case 4://右折
        Move.migi();
        Move.susumu_heitan();
        break;

    case 5://左折
        Move.hidari();
        Move.susumu_heitan();
        break;

    case 6://後進
        Move.migi();
        Move.migi();
        Move.susumu_heitan();
        break;
    }
}