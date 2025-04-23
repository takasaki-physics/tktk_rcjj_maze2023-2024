//重みづけ有りの右手法
#include <stdio.h>
#include <Arduino.h>
#include <Wire.h>
#include <SCServo.h> //SCServoを使うと宣言
SMS_STS sms_sts;

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

unsigned int SERVO_POS = 0; //変数SERVO_POS=0とする
byte ID[4]; //IDはそれぞれのモーターのID
s16 Position[4]; //Positionはモーターが回る角度(右一回転＝4095)
u16 Speed[4]; 
byte ACC[4];

bool black_count =0;
bool blue_count =0;
int8_t susumu_kaisuu= 10;
int8_t i=1;
int8_t n=1;
double kakudo;
int16_t kakudo_true;
double kakudo2;
int16_t kakudo_true2;
int16_t mawasu;
double katamuki;
int16_t katamuki_true;
bool saka = 0;//sloopがあるかどうか
int8_t count2 = 0;//進んだ回数＿10回でわかれている
bool chousei =0;
void tiziki_kaitenn()
{
  getYawPitchRoll();


  kakudo2 = yaw;
  kakudo_true2 = int(kakudo2);
  Serial.println(kakudo_true2);
}

//turn_hosei
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

void tiziki_2(){
  getYawPitchRoll();
  katamuki = pitch;
  katamuki_true = int(katamuki);
  Serial.println(katamuki_true); 
  
}



void motor_setup() {
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
}
//turn right
void migi(){
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

// turn left
void hidari(){
  
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

//go straight
void susumu_heitan() {
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
     black_count=1;
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
            saka = 1;
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
            saka = 1;
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



void move_colororbump(){
    }
//collect_tof_data
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

/*
//get_victim_data
void serialEvent1(){
  if (Serial7.available() >0){
    int receivedData = Serial7.read();
    Serial7.flush();
    discover_hisaisha();
  }
}


*/
/* 
//get_victim_data
void serialEvent2(){
    if (Serial8.available() >0){
    int receivedData = Serial8.read();
    Serial8.flush();
    discover_hisaisha2();
  }
}
*/
//collect_color_and_bump_data
void serialEvent3(){
    if (Serial1.available() >0){
    int receivedData = Serial1.read();
    move_colororbump();
    }
}

void setup(){
    Serial.begin(115200);//using_serial_monitor
    Wire.begin();
    Serial2.begin(1000000);//using_servo_sts3032
    Serial3.begin(19200);//using_tof
    Serial1.begin(19200);//using_Sub_Kairo
    Serial7.begin(19200);//using_cam1
    Serial8.begin(19200);//using_cam2
    }

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
                    if(black_count){//黒タイルの信号が送られていた場合戻る
                        y += -1;
                        black_count = false;
                    }
                    if(saka){//坂の信号が送られていた場合座標を更に進める
                        y += 1;
                        saka = false;
                    }
                    break;
                
                case Front:
                    //send front moving signal
                    x += 1;
                    i = East;
                    if(black_count){//黒タイルの信号が送られていた場合戻る
                        x += -1;
                        black_count = false;
                    }
                    if(saka){//坂の信号が送られていた場合座標を更に進める
                        x += 1;
                        saka = false;
                    }
                    break;
                
                case Left:
                    //send left moving signal
                    y += -1;
                    i = North;
                    if(black_count){//黒タイルの信号が送られていた場合戻る
                        y += 1;
                        black_count = false;
                    }
                    if(saka){//坂の信号が送られていた場合座標を更に進める
                        y += -1;
                        saka = false;
                    }
                    break;

                case Back:
                    //send return moving signal
                    TileReach[x][y] += 100;//行き止まりだから効率化のため二度と行かないようにする
                    x += -1;
                    i = West;
                    if(black_count){//黒タイルの信号が送られていた場合戻る
                        x += 1;
                        black_count = false;
                    }
                    if(saka){//坂の信号が送られていた場合座標を更に進める
                        x += -1;
                        saka = false;
                    }
                    break;
                }
            break;
        
        case North:
            switch (GoTo){
                case Right:
                    //send right moving signal
                    x += 1;
                    i = East;
                    if(black_count){//黒タイルの信号が送られていた場合戻る
                        x += -1;
                        black_count = false;
                    }
                    if(saka){//坂の信号が送られていた場合座標を更に進める
                        x += 1;
                        saka = false;
                    }
                    break;
                
                case Front:
                    //send front moving signal
                    y += -1;
                    i = North;
                    if(black_count){//黒タイルの信号が送られていた場合戻る
                        y += 1;
                        black_count = false;
                    }
                    if(saka){//坂の信号が送られていた場合座標を更に進める
                        y += -1;
                        saka = false;
                    }
                    break;
                
                case Left:
                    //send left moving signal
                    x += -1;
                    i = West;
                    if(black_count){//黒タイルの信号が送られていた場合戻る
                        x += 1;
                        black_count = false;
                    }
                    if(saka){//坂の信号が送られていた場合座標を更に進める
                        x += -1;
                        saka = false;
                    }
                    break;

                case Back:
                    //send return moving signal
                    TileReach[x][y] += 100;
                    y += 1;
                    i = South;
                    if(black_count){//黒タイルの信号が送られていた場合戻る
                        y += -1;
                        black_count = false;
                    }
                    if(saka){//坂の信号が送られていた場合座標を更に進める
                        y += 1;
                        saka = false;
                    }
                    break;
                }
            break;
        
        case West:
            switch (GoTo){
                case Right:
                    //send right moving signal
                    y += -1;
                    i = North;
                    if(black_count){//黒タイルの信号が送られていた場合戻る
                        y += 1;
                        black_count = false;
                    }
                    if(saka){//坂の信号が送られていた場合座標を更に進める
                        y += -1;
                        saka = false;
                    }
                    break;
                
                case Front:
                    //send front moving signal
                    x += -1;
                    i = West;
                    if(black_count){//黒タイルの信号が送られていた場合戻る
                        x += 1;
                        black_count = false;
                    }
                    if(saka){//坂の信号が送られていた場合座標を更に進める
                        x += -1;
                        saka = false;
                    }
                    break;
                
                case Left:
                    //send left moving signal
                    y += 1;
                    i = South;
                    if(black_count){//黒タイルの信号が送られていた場合戻る
                        y += -1;
                        black_count = false;
                    }
                    if(saka){//坂の信号が送られていた場合座標を更に進める
                        y += 1;
                        saka = false;
                    }
                    break;
                
                case Back:
                    //send return moving signal
                    TileReach[x][y] += 100;
                    x += 1;
                    i = East;
                    if(black_count){//黒タイルの信号が送られていた場合戻る
                        x += -1;
                        black_count = false;
                    }
                    if(saka){//坂の信号が送られていた場合座標を更に進める
                        x += 1;
                        saka = false;
                    }
                    break;
                }
            break;
        
        case South:
            switch (GoTo){
                case Right:
                    //send right moving signal
                    x += -1;
                    i = West;
                    if(black_count){//黒タイルの信号が送られていた場合戻る
                        x += 1;
                        black_count = false;
                    }
                    if(saka){//坂の信号が送られていた場合座標を更に進める
                        x += -1;
                        saka = false;
                    }
                    break;
                
                case Front:
                    //send front moving signal
                    y += 1;
                    i = South;
                    if(black_count){//黒タイルの信号が送られていた場合戻る
                        y += -1;
                        black_count = false;
                    }
                    if(saka){//坂の信号が送られていた場合座標を更に進める
                        y += 1;
                        saka = false;
                    }
                    break;
                
                case Left:
                    //send left moving signal
                    x += 1;
                    i = East;
                    if(black_count){//黒タイルの信号が送られていた場合戻る
                        x += -1;
                        black_count = false;
                    }
                    if(saka){//坂の信号が送られていた場合座標を更に進める
                        x += 1;
                        saka = false;
                    }
                    break;
                
                case Back:
                    //send return moving signal
                    TileReach[x][y] += 100;
                    y += -1;
                    i = North;
                    if(black_count){//黒タイルの信号が送られていた場合戻る
                        y += 1;
                        black_count = false;
                    }
                    if(saka){//坂の信号が送られていた場合座標を更に進める
                        y += -1;
                        saka = false;
                    }
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