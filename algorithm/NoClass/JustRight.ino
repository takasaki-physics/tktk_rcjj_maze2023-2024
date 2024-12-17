//マジでただの右手法（マッピングもなし）
//このとき無限ループの可能性が出る

#include <stdio.h>
#include <Arduino.h>

#define Right 1
#define Front 2
#define Left 3
#define Back 4

bool RightWall = false;//壁の有無
bool LeftWall = false;
bool FrontWall = false;

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


int8_t Judge(){//どこに行くかを返す関数
    if(!RightWall){//右の壁が空いていたら
            return Right;
        }

    if(!FrontWall){//前の壁が空いていたら
            return Front;
        }

    if(!LeftWall){//左の壁が空いていたら
            return Left;
        }

        return Back;

}

void move(int8_t GoTo){//動く関数
  switch (GoTo)
  {
  case Right://ここにそれぞれの場合の動く関数を入れる

    break;

  case Left:

    break;
  
  case Front:

    break;
  
  case Back:

    break;
  }
}

void setup(){
  //距離センサーのピンとかを定義
  Serial.println("Start");
}

void loop(){
    WriteWall();
    move(Judge());

}