//重みづけ有りの右手法
#include <stdio.h>
#include <Arduino.h>

#define East 1
#define North 2
#define West 3
#define South 4

#define Right 1
#define Front 2
#define Left 3
#define Back 4

int8_t x = 50;
int8_t y = 50;
int8_t i = North;//向き
int8_t TileReach[90][90];//到達回数を格納

bool RightWall = false;
bool LeftWall = false;
bool FrontWall = false;

int8_t RightWeight = 0;
int8_t LeftWeight = 0;
int8_t FrontWeight = 0;
int8_t BackWeight = 0;


int16_t RightDis = 0;//壁までの距離
int16_t LeftDis = 0;
int16_t FrontDis = 0;

void WriteWall(){
    if(RightDis < 100){//壁までの距離の値が一定以下だったら（測定方法によって条件は変わると思う）
        RightWall = true;
    }
    if(LeftDis < 100){
        LeftWall = true;
    }
    if(FrontDis < 100){
        FrontWall = true;
    }
}


int8_t Judge(int8_t x,int8_t y,int8_t i){//どこに行くかを返す関数
    switch (i)//前後左右の重みを設定
    {
    case East:
        RightWeight = TileReach[x][y+1] * 5 + 1;
        FrontWeight = TileReach[x+1][y] * 5 + 2;
        LeftWeight = TileReach[x][y-1] * 5 + 3;
        break;
    
    case North:
        RightWeight = TileReach[x+1][y] * 5 + 1;
        FrontWeight = TileReach[x][y-1] * 5 + 2;
        LeftWeight = TileReach[x-1][y] * 5 + 3;
        break;

    case West:
        RightWeight = TileReach[x][y-1] * 5 + 1;
        FrontWeight = TileReach[x-1][y] * 5 + 2;
        LeftWeight = TileReach[x][y+1] * 5 + 3;
        break;

    case South:
        RightWeight = TileReach[x-1][y] * 5 + 1;
        FrontWeight = TileReach[x][y+1] * 5 + 2;
        LeftWeight = TileReach[x+1][y] * 5 + 3;
        break;
    }


    if(RightWall){//壁がある場合は更にプラス
        RightWeight += 100;
    }
    if(FrontWall){
        FrontWeight += 100;
    }
    if(LeftWall){
        LeftWeight += 100;
    }
    int8_t GoTo = 0;

    if((RightWeight > 100)&&(FrontWeight > 100)&&(LeftWeight > 100)){//全部壁（または行き止まり）のいずれかの場合下がる
        return Back;
    }

    if(RightWeight < FrontWeight){
        GoTo = Right;
    }else{
        GoTo = Front;
    }
    if(LeftWeight < min(RightWeight,FrontWeight)){
        GoTo = Left;
    }

    return GoTo;
}

void move(int8_t i,int8_t GoTo){//動く関数
    switch (i){//それぞれ向いている方向の場合分け
        case East:
            switch (GoTo){//それぞれ行く方向の場合分け
                case Right:
                    //send right moving signal
                    y += 1;
                    i = South;
                    break;
                
                case Front:
                    //send front moving signal
                    x += 1;
                    i = East;
                    break;
                
                case Left:
                    //send left moving signal
                    y += -1;
                    i = North;
                    break;

                case Back:
                    //send return moving signal
                    TileReach[x][y] += 100;//行き止まりだから効率化のため二度と行かないようにする
                    x += -1;
                    i = West;
                    break;
                }
            break;
        
        case North:
            switch (GoTo){
                case Right:
                    //send right moving signal
                    x += 1;
                    i = East;
                    break;
                
                case Front:
                    //send front moving signal
                    y += -1;
                    i = North;
                    break;
                
                case Left:
                    //send left moving signal
                    x += -1;
                    i = West;
                    break;

                case Back:
                    //send return moving signal
                    TileReach[x][y] += 100;
                    y += -1;
                    i = South;
                    break;
                }
            break;
        
        case West:
            switch (GoTo){
                case Right:
                    //send right moving signal
                    y += -1;
                    i = North;
                    break;
                
                case Front:
                    //send front moving signal
                    x += -1;
                    i = West;
                    break;
                
                case Left:
                    //send left moving signal
                    y += 1;
                    i = South;
                    break;
                
                case Back:
                    //send return moving signal
                    TileReach[x][y] += 100;
                    x += 1;
                    i = East;
                    break;
                }
            break;
        
        case South:
            switch (GoTo){
                case Right:
                    //send right moving signal
                    x += -1;
                    i = West;
                    break;
                
                case Front:
                    //send front moving signal
                    y += 1;
                    i = South;
                    break;
                
                case Left:
                    //send left moving signal
                    x += 1;
                    i = East;
                    break;
                
                case Back:
                    //send return moving signal
                    TileReach[x][y] += 100;
                    y += -1;
                    i = North;
                    break;
                }
            break;
    }
    TileReach[x][y] += 1;//移動先のマスの到達回数をプラスしている
}

void setup(){
  //距離センサーのピンとかを定義
  Serial.println("Start");

  for (int t = 0; t < 90; t++) {//到達回数を初期化
    for (int j = 0; j < 90; j++) {
        TileReach[t][j] = 0;
        }
    }
}

void loop(){
    WriteWall();
    move(i,Judge(x,y,i));
    //このあとに一定時間がたったら帰還するプログラムを入れる
}