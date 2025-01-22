/*

関数の編集時には記録を残してください

*/



#include <stdio.h>
#include <cstdint>
#include <Arduino.h>
#include <time.h>
#include <cstdint>
#include <SCServo.h> //SCServoを使うと宣言]
#include "MPU6050_6Axis_MotionApps20.h"

MPU6050 mpu;
SMS_STS sms_sts;

#define East 1
#define North 2
#define West 3
#define South 4

#define Right 1
#define Front 2
#define Left 3
#define Back 4



/*MPU変数*/
// MPU control/status vars
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint8_t fifoBuffer[64]; // FIFO storage buffer
// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
float ypr[3];           // [roll, pitch, yaw]   roll/pitch/yaw container and gravity vector
double pitch;
double roll;
double yaw;



/*******************************************************************************************/
/* ヨーピッチを取得                                                                            
/*処理：
/*
/*更新者：誰が作った
/*
/*******************************************************************************************/
void getYawPitchRoll()
{
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {// Get the Latest packet
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);

    yaw = ypr[0] * 180 / M_PI;
    pitch = ypr[1] * 180 / M_PI;
    roll = ypr[2] * 180 / M_PI;

    if (yaw < 0) {
        yaw = 360 + yaw;
      }
      yaw = 360 - yaw;

    // Output individual values to check if they are correctly obtained
    Serial.print(" X: ");
    Serial.print(gravity.x);
    Serial.print(" Y: ");
    Serial.print(gravity.y);
    Serial.print(" Z: ");
    Serial.print(gravity.z);
    Serial.print(" Yaw: ");
    Serial.print(yaw);
    Serial.print(" Pitch: ");
    Serial.print(pitch);
    Serial.print(" Roll: ");
    Serial.println(roll);
  }
}




/*センサー類変数*/
unsigned int SERVO_POS = 0; //変数SERVO_POS=0とする
byte ID[4]; //IDはそれぞれのモーターのID
s16 Position[4]; //Positionはモーターが回る角度(右一回転＝4095)
u16 Speed[4]; 
byte ACC[4]; 
int susumu_kaisuu= 10;
int i=1;
int n=1;
double kakudo;
int kakudo_true;
double kakudo2;
int kakudo_true2;
int mawasu;
double katamuki;
int katamuki_true;
int count2 = 0;//進んだ回数＿10回でわかれている
bool chousei =0;

bool right_wall = false;//ここもセンサーの値を取得するファイルとして分けたい
bool front_wall = false;
bool left_wall = false;
int8_t BlackTile =false;
int8_t blue_count =false;
bool Slope = false;//sloopがあるかどうか



/*******************************************************************************************/
/* 地磁気回転                                                                            
/*処理：
/*
/*更新者：誰が作った
/*
/*******************************************************************************************/
void tiziki_kaitenn()
{
  getYawPitchRoll();


  kakudo2 = yaw;
  kakudo_true2 = int(kakudo2);
  Serial.println(kakudo_true2);
}



/*******************************************************************************************/
/* 地磁気                                                                              
/*処理：
/*
/*更新者：誰が作った
/*
/*******************************************************************************************/
void tiziki()
{
  getYawPitchRoll();
  kakudo =yaw;
  kakudo_true = int(kakudo);
  Serial.println(kakudo_true);
  if (kakudo_true >= 0 && kakudo_true < 45) {
    for (int i =0; i < kakudo_true ;i++){
        Position[0] = -30; //右に回転
        Position[1] = -30;
        Position[2] = -30;
        Position[3] = -30;
        sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
        delay(18);
      
    }
  }else if (kakudo_true >= 45 && kakudo_true < 90) {
    for (int i =0; i < 90-kakudo_true ;i++){
        Position[0] = 30; //左に回転
        Position[1] = 30;
        Position[2] = 30;
        Position[3] = 30;
        sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
        delay(18);
      
    }
  }else if (kakudo_true >= 90 && kakudo_true < 135) {
    for (int i =0; i < kakudo_true-90 ;i++){
        Position[0] = -30; //右に回転
        Position[1] = -30;
        Position[2] = -30;
        Position[3] = -30;
        sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
        delay(18);
      
    }
  }else if (kakudo_true >= 135 && kakudo_true < 180) {
    for (int i =0; i < 180-kakudo_true ;i++){
        Position[0] = 30; //左に回転
        Position[1] = 30;
        Position[2] = 30;
        Position[3] = 30;
        sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
        delay(18);
      
    }
  }else if (kakudo_true >= 180 && kakudo_true < 225) {
    for (int i =0; i < kakudo_true-180 ;i++){
        Position[0] = -30; //右に回転
        Position[1] = -30;
        Position[2] = -30;
        Position[3] = -30;
        sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
        delay(18);
      
    }
  }else if (kakudo_true >= 225 && kakudo_true < 270) {
    for (int i =0; i < 270-kakudo_true ;i++){
        Position[0] = 30; //左に回転
        Position[1] = 30;
        Position[2] = 30;
        Position[3] = 30;
        sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
        delay(18);
      
    }
  }else if (kakudo_true >= 270 && kakudo_true < 315) {
    for (int i =0; i < kakudo_true-270 ;i++){
        Position[0] = -30; //右に回転
        Position[1] = -30;
        Position[2] = -30;
        Position[3] = -30;
        sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
        delay(18);
      
    }
  }else if (kakudo_true >= 315 && kakudo_true <= 360) {
    for (int i =0; i < 360-kakudo_true ;i++){
        Position[0] = 30; //左に回転
        Position[1] = 30;
        Position[2] = 30;
        Position[3] = 30;
        sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
        delay(18);
      
    }
  }
  delay(100);
}



/*******************************************************************************************/
/* 地磁気２                                                                              
/*処理：
/*
/*更新者：誰が作った
/*
/*******************************************************************************************/
void tiziki_2()
{
  getYawPitchRoll();
  katamuki = pitch;
  katamuki_true = int(katamuki);
  Serial.println(katamuki_true); 
  
}



/*******************************************************************************************/
/* 右回転                                                                              
/*処理：
/*
/*更新者：誰が作った
/*
/*******************************************************************************************/
void migi()
{
  tiziki_kaitenn();
  if (kakudo_true2 <=90){
    mawasu = kakudo_true2 +285;
    kakudo_true2 =kakudo_true2 +360;
    chousei =1;
  }else if (kakudo_true2 <= 360){
    mawasu = kakudo_true2-75;
  }
  while (kakudo_true2 > mawasu || chousei ==1){

    
  
  Position[0] = -220; //右に回転
  Position[1] = -220;
  Position[2] = -220;
  Position[3] = -220;
  sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
  tiziki_kaitenn();
  //serialEvent();
  //serialEvent2();
  if (chousei ==1){
    if (kakudo_true2 <=90){
      kakudo_true2 =kakudo_true2 +360;
      
    }else {
      chousei =0;
    }
  }
  delay(70);
  }
  mawasu =0;
  delay(200);
  tiziki();
  delay(200);
  
             
}



/*******************************************************************************************/
/* 左回転                                                                              
/*処理：
/*
/*更新者：誰が作った
/*
/*******************************************************************************************/
void hidari()
{
  
  tiziki_kaitenn();
  if (kakudo_true2 >= 270){
    mawasu = kakudo_true2 - 285; 
    kakudo_true2 = kakudo_true2 - 360;
    chousei = 1; 
  }else if (kakudo_true2 >= 0){
    mawasu = kakudo_true2 + 75;
  }
  while (kakudo_true2 < mawasu || chousei ==1){

    
  
  Position[0] = 220; //左に回転
  Position[1] = 220;
  Position[2] = 220;
  Position[3] = 220;
  sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
  tiziki_kaitenn();
  if (chousei ==1){
    if (kakudo_true2 >= 270){
      kakudo_true2 =kakudo_true2 - 360;
      
    }else {
      chousei =0;
    }
  }
  delay(70);
  }
  
  tiziki();
  delay(200);
              
}



/*******************************************************************************************/
/* 直進                                                                              
/*処理：
/*
/*更新者：誰が作った
/*
/*******************************************************************************************/
void susumu_heitan()
{
    Serial3.flush();
  while (count2  <susumu_kaisuu) {
    Position[0] = 576; //go
    Position[1] = -576;
    Position[2] = -576;
    Position[3] = 576;
    sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
    //serialEvent3();_color_or_loadcell
    //serialEvent1();_victim_camera1
    //serialEvent2();_victim_camera2
    count2++;
  }
  if (count2 != susumu_kaisuu){
    while (count2 !=0){
    Position[0] = -576; //back
    Position[1] = 576;
    Position[2] = 576;
    Position[3] = -576;
    sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
    delay(320);
     count2--;
    BlackTile=1;
    }
  
  delay(300);

  }else {
    
    tiziki();

  }

    tiziki_2();
    delay(50);
    if (katamuki_true >17){
        Serial.println("Saka_Ue");
        while(katamuki_true >17){
            Serial.println("sakaue");
            Position[0] = 200; //前に進む
            Position[1] = -200;
            Position[2] = -200;
            Position[3] = 200;
            sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
            tiziki_2();
            delay(10);
            Slope = 1;
        }
    }else if (katamuki_true <-17){
        while(katamuki_true < -17){
            Position[0] = 200; //前に進む
            Position[1] = -200;
            Position[2] = -200;
            Position[3] = 200;
            sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
            tiziki_2();
            delay(10);
            Slope = 1;
        }
    }else if(katamuki_true <-5&& katamuki_true >=9){
        Position[0] = 1000; //前に進む
        Position[1] = -1000;
        Position[2] = -1000;
        Position[3] = 1000;
        sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
        delay(270);

    }

  count2 =0;

    
}



/*******************************************************************************************/
/*色タイルまたはバンプ                                                                              
/*処理：
/*
/*更新者：誰が作った
/*
/*******************************************************************************************/
void move_colororbump(){
    }



/*******************************************************************************************/
/* collect_tof_data                                                                              
/*処理：
/*
/*更新者：誰が作った
/*
/*******************************************************************************************/
void serialEvent(){
    Serial3.flush();
    while (Serial3.available()== 0){
        delay(100);
        }
    if (Serial3.available() >0){
        int receivedData = Serial3.read();
        //get_data_json
        }
    
    }



/*******************************************************************************************/
/* get_victim_data                                                                              
/*処理：
/*
/*更新者：誰が作った
/*
/*******************************************************************************************/
/*
void serialEvent1(){
  if (Serial7.available() >0){
    int receivedData = Serial7.read();
    Serial7.flush();
    discover_hisaisha();
  }
}
*/



/*******************************************************************************************/
/* get_victim_data                                                                              
/*処理：
/*
/*更新者：誰が作った
/*
/*******************************************************************************************/
/* 
void serialEvent2(){
    if (Serial8.available() >0){
    int receivedData = Serial8.read();
    Serial8.flush();
    discover_hisaisha2();
  }
}
*/



/*******************************************************************************************/
/* collect_color_and_bump_data                                                                              
/*処理：
/*
/*更新者：誰が作った
/*
/*******************************************************************************************/
void serialEvent3(){
    if (Serial1.available() >0){
    int receivedData = Serial1.read();
    move_colororbump();
    }
}



/*スタック・キュー用変数*/
const int MAX = 30; // キュー配列の最大サイズ



/*******************************************************************************************/
/* スタック・キュー用の関数                                                                             
/*処理：インターネットに転がってるものを借用。ググってくれ！
/*
/*更新者：吉ノ薗2025/01/22「既存のライブラリだとリングバッファがないからつくった」
/*
/*******************************************************************************************/
int st[MAX]; // スタックを表す配列
int top = 0; // スタックの先頭を表すポインタ
// スタックを初期化する
void st_init() {
    top = 0; // スタックポインタを初期位置に
}

// スタックが空かどうかを判定する
bool st_isEmpty() {
    return (top == 0); // スタックサイズが 0 かどうか
}

// スタックが満杯かどうかを判定する
bool st_isFull() {
    return (top == MAX); // スタックサイズが MAX かどうか
}

int qu[MAX]; // キューを表す配列
int tail = 0, head = 0; // キューの要素区間を表す変数

// キューを初期化する
void init() {
    head = tail = 0;
}

// キューが空かどうかを判定する
bool isEmpty() {
    return (head == tail);
}

// スタックが満杯かどうかを判定する
bool isFull() {
    return (head == (tail + 1) % MAX);
}

// push (top を進めて要素を格納)
void push(int v) {
    if (st_isFull()) {
        //cout << "error: stack is full." << endl;
        return;
    }
    st[top++] = v; // st[top] = v; と top++; をまとめてこのように表せます
}

// pop (top をデクリメントして、top の位置にある要素を返す)
int pop() {
    if (st_isEmpty()) {
        //cout << "error: stack is empty." << endl;
        return -1;
    }
    return st[--top]; // --top; と return st[top]; をまとめてこのように表せます
}

// enqueue (tail に要素を格納してインクリメント)
void enqueue(int v) {
    if (isFull()) {
        //cout << "error: queue is full." << endl;
        return;
    }
    qu[tail++] = v;
    if (tail == MAX) tail = 0; // リングバッファの終端に来たら 0 に
}

// dequeue (head にある要素を返して head をインクリメント)
int dequeue() {
    if (isEmpty()) {
        //cout << "error: stack is empty." << endl;
        return -1;
    }
    int res = qu[head];
    ++head;
    if (head == MAX) head = 0;
    return res;
}



/*アルゴリズム用変数*/
uint8_t x = 50;
uint8_t y = 50;
uint8_t Direction = North;
uint8_t Status = 0;
int8_t toutatu_zahyou[90][90];//そのマスの到達回数
int16_t RightWeight = 0;
int16_t FrontWeight = 0;
int16_t LeftWeight = 0;



/*******************************************************************************************/
/* ジャッジ                                                                            
/*処理：前左右を重みづけして、行く方向を決定
/*      右+1、前+2、左+3することで基本的に右に行く拡張右手法を実現
/*      壁があったらさらにプラスする
/*
/*更新者：吉ノ薗2025/01/22
/*
/*******************************************************************************************/
int8_t judge(){
    
    switch (Direction){//向きによって重みをつける
        case East:
            RightWeight = toutatu_zahyou[x][y+1] * 5 + 1;
            FrontWeight = toutatu_zahyou[x+1][y] * 5 + 2;
            LeftWeight = toutatu_zahyou[x][y-1] * 5 + 3;
            break;

        case North:
            RightWeight = toutatu_zahyou[x+1][y] * 5 + 1;
            FrontWeight = toutatu_zahyou[x][y-1] * 5 + 2;
            LeftWeight = toutatu_zahyou[x-1][y] * 5 + 3;
            break;

        case West:
            RightWeight = toutatu_zahyou[x][y-1] * 5 + 1;
            FrontWeight = toutatu_zahyou[x-1][y] * 5 + 2;
            LeftWeight = toutatu_zahyou[x][y+1] * 5 + 3;
            break;

        case South:
            RightWeight = toutatu_zahyou[x-1][y] * 5 + 1;
            FrontWeight = toutatu_zahyou[x][y+1] * 5 + 2;
            LeftWeight = toutatu_zahyou[x+1][y] * 5 + 3;
            break;

    }

    //壁がある場合更にプラスする
    if (right_wall){
        RightWeight += 100;
    }
    if (front_wall){
        FrontWeight += 100;
    }
    if (left_wall){
        LeftWeight += 100;
    }

    int8_t GoTo = 0;

    //どこへ行くか決める
    if(RightWeight < FrontWeight){
        GoTo = Right;
    }else{
        GoTo = Front;
    }
    if(LeftWeight < min(RightWeight,FrontWeight)){
        GoTo = Left;
    }

    

    if ((RightWeight > 100) && (FrontWeight > 100) && (LeftWeight > 100)){//if all wall
        GoTo = Back;
    }

    RightWeight = 0;//怖いから初期化
    FrontWeight = 0;
    LeftWeight = 0;

    return GoTo;
}




/*******************************************************************************************/
/* 動く方向                                                                              
/*処理：それぞれの方位のときに行く方向によって座標を移動・方位を変更する
/*      後進のときは全部壁のときだから、現在の重みを+100する
/*
/*更新者：吉ノ薗2025/01/22
/*
/*******************************************************************************************/
void MoveTo(int8_t GoTo)
{
switch (Direction){
        case East:
            switch (GoTo){
                case Right:
                    Status = 4;
                    y += 1;
                    Direction = South;
                    break;
                
                case Front:
                    Status = 3;
                    x += 1;
                    Direction = East;
                    break;
                
                case Left:
                    Status = 5;
                    y += -1;
                    Direction = North;
                    break;

                case Back:
                    Status = 6;
                    toutatu_zahyou[x][y] += 100;//行き止まりだから効率化のため二度と行かないようにする
                    x += -1;
                    Direction = West;
                    break;
                }
            break;
        
        case North:
            switch (GoTo){
                case Right:
                    Status = 4;
                    x += 1;
                    Direction = East;
                    break;
                
                case Front:
                    Status = 3;
                    y += -1;
                    Direction = North;
                    break;
                
                case Left:
                    Status = 5;
                    x += -1;
                    Direction = West;
                    break;

                case Back:
                    Status = 6;
                    toutatu_zahyou[x][y] += 100;//行き止まりだから効率化のため二度と行かないようにする
                    y += -1;
                    Direction = South;
                    break;
                }
            break;
        
        case West:
            switch (GoTo){
                case Right:
                    Status = 4;
                    y += -1;
                    Direction = North;
                    break;
                
                case Front:
                    Status = 3;
                    x += -1;
                    Direction = West;
                    break;
                
                case Left:
                    Status = 5;
                    y += 1;
                    Direction = South;
                    break;
                
                case Back:
                    Status = 6;
                    toutatu_zahyou[x][y] += 100;//行き止まりだから効率化のため二度と行かないようにする
                    x += 1;
                    Direction = East;
                    
                    break;
                }
            break;
        
        case South:
            switch (GoTo){
                case Right:
                    Status = 4;
                    x += -1;
                    Direction = West;
                    break;
                
                case Front:
                    Status = 3;
                    y += 1;
                    Direction = South;
                    break;
                
                case Left:
                    Status = 5;
                    x += 1;
                    Direction = East;
                    break;
                
                case Back:
                    Status = 6;
                    toutatu_zahyou[x][y] += 100;//行き止まりだから効率化のため二度と行かないようにする
                    y += -1;
                    Direction = North;
                    break;
                }
            break;
    }
    toutatu_zahyou[x][y] += 1;//移動先のマスの到達回数をプラスしている
    
}



/*******************************************************************************************/
/* タイルの色                                                                              
/*処理：それぞれの方向について、
/*      黒タイルの信号が送られていたら一マス戻る
/*      坂の信号が送られていたら一マス進む
/*      黒タイルと坂信号の初期化
/*
/*更新者：吉ノ薗2025/01/22
/*
/*******************************************************************************************/
void TileColor()
{
    switch (Direction)
    {
    case North:
        if(BlackTile){//黒タイルの信号が送られていた場合戻る
            y += 1;
        }
        if(Slope){//坂の信号が送られていた場合座標を更に進める
            y += -1;
        }
        break;

    case East:
        if(BlackTile){//黒タイルの信号が送られていた場合戻る
            x += -1;
        }
        if(Slope){//坂の信号が送られていた場合座標を更に進める
            x += 1;
        }
        break;
    
    case West:
        if(BlackTile){//黒タイルの信号が送られていた場合戻る
            x += 1;
        }
        if(Slope){//坂の信号が送られていた場合座標を更に進める
            x += -1;
        }
        break;

    case South:
        if(BlackTile){//黒タイルの信号が送られていた場合戻る
            y += -1;
        }
        if(Slope){//坂の信号が送られていた場合座標を更に進める
            y += 1;
        }
        break;
    
    
    }
    BlackTile = false;
    Slope = false;
}




/*帰還用変数*/
int8_t kabe_zahyou[90][90];//0000 の4ビットに絶対方向の東8西4南2北1をそれぞれ割り当てるみたいな
int8_t cost[90][90];//マス目のコスト
bool reach_time[90][90];//到達の有無（帰還用）
long now_seconds;
long firstseconds;



/*******************************************************************************************/
/* 最短経路の方位                                                                              
/*処理：現在のマスのコストの、-1のコストのマスを探す
/*更新者：吉ノ薗2025/01/22
/*
/*******************************************************************************************/
int8_t WhichWay(int a,int b)
{
    if(cost[a][b] - cost[a+1][b] == 1){
        return East;
       }
    if(cost[a][b] - cost[a][b-1] == 1){
        return North;
    }
    if(cost[a][b] - cost[a-1][b] == 1){
        return West;
    }
    if(cost[a][b] - cost[a][b+1] == 1){
        return South;
    }
    return 0;
}



/*******************************************************************************************/
/* 幅優先探索                                                                              
/*処理：現在のマスから空いている隣接マスのコストを、現在のマスのコスト+1していくのを繰り返す
/*      kabe_zahyou[][]に0000 の4ビットに絶対方向の東8西4南2北1をそれぞれ割り当てているので、割っていって余り0のときその先の壁が無いと判定する
/*      ゴールにたどり着いたらゴールから逆に辿っていきつつ右折、左折、直進を逆からスタックに入れていく
/*
/*
/*
/*更新者：吉ノ薗2025/01/22
/*
/*******************************************************************************************/
void BFS()
{

    int a = x;
    int b = y;
    cost[a][b] = 1;//現在地のコストを1にする

    while(1){

        reach_time[a][b] = 1;//そのマスを訪問済みにする
        for(int n = 2; n <= 16; n *= 2){//そのマスの周りのマスのコストを＋１する
            //int result = static_cast<int>(pow(2, n));
            if(kabe_zahyou[a][b] % n == 0){//kabe_zahyou[][]は0000 の4ビットに絶対方向の東8西4南2北1をそれぞれ割り当てる
                switch(n){
                    case 2://北の壁がない
                        if((!reach_time[a][b-1])&&(kabe_zahyou[a][b-1] != 100)){//その先のマスが訪問済みでない&その先のマスが探索済み
                            cost[a][b-1] = cost[a][b] + 1;
                            //キューの末尾に入れる
                            enqueue(a);
                            enqueue(b-1);
                        }
                        break;

                    case 4://南の壁がない
                        if((!reach_time[a][b+1])&&(kabe_zahyou[a][b+1] != 100)){//その先のマスが訪問済みでない&その先のマスが探索済み
                            cost[a][b+1] = cost[a][b] + 1;
                            //キューの末尾に入れる
                            enqueue(a);
                            enqueue(b+1);
                        }
                        break;

                    case 8://西の壁がない
                        if((!reach_time[a-1][b])&&(kabe_zahyou[a-1][b] != 100)){//その先のマスが訪問済みでない&その先のマスが探索済み
                            cost[a-1][b] = cost[a][b] + 1;
                            //キューの末尾に入れる
                            enqueue(a-1);
                            enqueue(b);
                        }
                        break;

                    case 16://東の壁がない
                        if((!reach_time[a+1][b])&&(kabe_zahyou[a+1][b] != 100)){//その先のマスが訪問済みでない&その先のマスが探索済み
                            cost[a+1][b] = cost[a][b] + 1;
                            //キューの末尾に入れる
                            enqueue(a+1);
                            enqueue(b);
                        }
                        break;
                }

            }else{//その方向に壁があるとき
                kabe_zahyou[a][b] = kabe_zahyou[a][b] - (n/2);
            }
        }
        //キューの先頭を取り出す
        a = dequeue();
        b = dequeue();

        if((a == -1)||(b == -1)){//error
            break;
        }

        if((a == 50)&&(b == 50)){
            break;
        }
    }
    //スタックを使って逆探索
    a = 50;
    b = 50;
    push(4);//停止用
    while(1){

        switch(Direction){
            case East:
                switch(WhichWay(a,b)){//前後左右のどこが最短になるか１：右折、２：左折、３：直進

                    case North://北マスからきたとき（ここのシグナルは探索時の曲がる→進むとは逆で、進む→曲がるじゃないとかも。pushの順番は曲がる、進む(これpop?)）
                        push(2);
                        push(3);
                        b += -1;
                        Direction = South;

                    case West://西マスからきたとき
                        push(3);
                        a += -1;

                    case South://南マスからきたとき
                        push(1);
                        push(3);
                        b += 1;
                        Direction = North;

                }

            case North:
                switch(WhichWay(a,b)){//前後左右のどこが最短になるか
                    case East:
                        push(1);
                        push(3);
                        a += 1;
                        Direction = West;

                    case West:
                        push(2);
                        push(3);
                        a += -1;
                        Direction = East;

                    case South:
                        push(3);
                        b += -1;

                }

            case West:
                switch(WhichWay(a,b)){//前後左右のどこが最短になるか
                    case East:
                        push(3);
                        a += 1;

                    case North:
                        push(1);
                        push(3);
                        b += -1;
                        Direction = South;

                    case South:
                        push(2);
                        push(3);
                        Direction = North;

                }

            case South:
                switch(WhichWay(a,b)){//前後左右のどこが最短になるか
                    case East:
                        push(2);
                        push(3);
                        a += 1;
                        Direction = West;

                    case North:
                        push(3);
                        b += -1;

                    case West:
                        push(1);
                        push(3);
                        a += -1;
                        Direction = East;

                }
        }
        if((a == x)&&(b == y)){
            break;
        }
    }
    
    
}



/*******************************************************************************************/
/* 現在のマスの壁情報を記入                                                                              
/*処理：kabe_zahyou[][]は情報未記入の場合100にしてあるので、100とき記録する
/*    kabe_zahyou[][]に0000 の4ビットに絶対方向の東8西4南2北1をそれぞれ割り当てて、値を加算
/*    kabe_zahyou[][]を-100して記録済みに
/*
/*更新者：吉ノ薗2025/01/22
/*
/*******************************************************************************************/
void WriteDownWall()
{
    //壁情報の記入(ここは帰還アルゴリズム用の関数)
    if(kabe_zahyou[x][y] == 100){//記録されていない場合（そうしないと延々と加算されちゃう）
        switch (Direction){
            case East:
                if(right_wall){
                    kabe_zahyou[x][y] += 2;
                }
                if(front_wall){
                    kabe_zahyou[x][y] += 8;
                }
                if(left_wall){
                    kabe_zahyou[x][y] += 1;
                }
                break;
            
            case North:
                if(right_wall){
                    kabe_zahyou[x][y] += 8;
                }
                if(front_wall){
                    kabe_zahyou[x][y] += 1;
                }
                if(left_wall){
                    kabe_zahyou[x][y] += 4;
                }
                break;

            case West:
                if(right_wall){
                    kabe_zahyou[x][y] += 1;
                }
                if(front_wall){
                    kabe_zahyou[x][y] += 4;
                }
                if(left_wall){
                    kabe_zahyou[x][y] += 2;
                }
                break;

            case South:
                if(right_wall){
                    kabe_zahyou[x][y] += 4;
                }
                if(front_wall){
                    kabe_zahyou[x][y] += 2;
                }
                if(left_wall){
                    kabe_zahyou[x][y] += 8;
                }
                break;
            }
            //対応
            //e n w s
            //8 4 2 1
        kabe_zahyou[x][y] += -100;//4ビットの情報のみが残る
    }
}



/*******************************************************************************************/
/* 帰還                                                                             
/*処理：スタックから値をpopして順番に動いていく
/*      全て終わる、つまりpopした値が一番最初に入れた"4"になっていたら停止（まだ停止部分はつくってない）
/*
/*更新者：吉ノ薗2025/01/22
/*
/*******************************************************************************************/
void GoHome()
{
        //ここ以下を「相手(モーター)から動き終わったという信号が送られたら」とかにしないとバババッて送られちゃうかも
    while(1){
        switch(pop()){
            case 1:
                //TurnRight
                migi();
                break;

            case 2:
                //TurnLeft
                hidari();
                break;

            case 3:
                //GoStraight
                susumu_heitan();
                break;
            case 4:
                //Stop

                break;
        }
    }
    
}



/*******************************************************************************************/
/* セットアップ                                                                              
/*処理：MPU、センサー、モーター、座標の初期化
/*
/*更新者：吉ノ薗2025/01/22
/*
/*******************************************************************************************/
void setup(){

  /*MPUのセットアップ***********************************************************************************************************/
  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
  mpu.initialize();
  devStatus = mpu.dmpInitialize();
  // supply your own gyro offsets here, scaled for min sensitivity

  mpu.setXAccelOffset(-3550);
  mpu.setYAccelOffset(-1727);
  mpu.setZAccelOffset(1087);
  mpu.setXGyroOffset(25);
  mpu.setYGyroOffset(-29);
  mpu.setZGyroOffset(7);
  
  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
    // Calibration Time: generate offsets and calibrate our MPU6050
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.setDMPEnabled(true);
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    Serial.print("DMP Initialization failed.");
  }



  /*センサーのセットアップ***********************************************************************************************************/
  Serial.begin(115200);//using_serial_monitor
  Wire.begin();
  Serial2.begin(1000000);//using_servo_sts3032
  Serial3.begin(19200);//using_tof
  Serial1.begin(19200);//using_Sub_Kairo
  Serial7.begin(19200);//using_cam1
  Serial8.begin(19200);//using_cam2



  /*モーターのセットアップ***********************************************************************************************************/
  sms_sts.pSerial = &Serial2;
  delay(1000);
  ID[0] = 1; //右前のモーターのID
  ID[1] = 2; //左前
  ID[2] = 3; //右後
  ID[3] = 4; //左後
  Speed[0] = 3400; //変数Speed[0]=0とする
  Speed[1] = 3400;
  Speed[2] = 3400;
  Speed[3] = 3400;
  ACC[0] = 50; //変数ACC[0]=0とする
  ACC[1] = 50;
  ACC[2] = 50;
  ACC[3] = 50;


  /*スタック・キューのセットアップ***********************************************************************************************************/
  init();
  st_init();



  /*アルゴリズムのセットアップ***********************************************************************************************************/
  for (int t = 0; t < 90; t++) {
    for (int j = 0; j < 90; j++) {
        kabe_zahyou[t][j] = 100;
        reach_time[t][j] = 0;
        cost[t][j] = 0;
        toutatu_zahyou[t][j] = 0; 
        }
    }
    // 現在の時刻を取得します
    time_t FirstTime;
    time(&FirstTime);  // 現在の時刻を取得して FirstTime に格納
    firstseconds = static_cast<long>(FirstTime); 
    
}

/*******************************************************************************************/
/* メインループ                                                                              
/*処理：statusの値によって壁情報の取得・座標更新と探索・帰還・前後左右への移動を行う
/*      status1のとき壁情報を記録 → 拡張右手法て行く方向を決定 → 現在の時刻とスタートの時刻を照合して、330秒経っていたらstatusを2にするかどうか決定
/*      status2のとき幅優先探索 → 実際に動いて停止
/*      status3,4,5,6のとき前後左右に移動 → 黒タイルや坂だった場合に合わせて座標を更新
/*
/*更新者：吉ノ薗2025/01/22
/*
/*******************************************************************************************/
void loop(){

    switch (Status)
    {
    case 0://壁情報取得
        //get_tof_data();
        break;

    case 1://座標更新と探索
        WriteDownWall();//帰還用の記録
        MoveTo(judge());//拡張右手法で行く方法を決める,実際に移動して座標を変更+到達回数を加算

        // 現在の時刻を取得
        time_t NowTime;
        time(&NowTime);  // 現在の時刻を取得して NowTime に格納
        now_seconds = static_cast<long>(NowTime); 

        //330秒（＝5分半）経ったら幅優先探索を始める
        if(now_seconds - firstseconds >= 330){
            Status = 2;//帰還開始
        }
        break;
    
    case 2://帰還(このとき探索に戻らないよう入れ子構造にする or ここだけは関数内に直接migi()とかを入れてwhile文)   
        BFS();
        GoHome();
        break;

    case 3://直進
        susumu_heitan();
        TileColor();
        break;

    case 4://右折
        migi();
        susumu_heitan();
        TileColor();
        break;

    case 5://左折
        hidari();
        susumu_heitan();
        TileColor();
        break;

    case 6://後進
        migi();
        migi();
        susumu_heitan();
        TileColor();
        break;
    }
}