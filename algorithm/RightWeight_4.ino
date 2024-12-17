//重みづけ有りの右手法
#include "MPU6050_6Axis_MotionApps20.h"
#include <ArduinoJson.h>
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
int count4 =0;
MPU6050 mpu;
int distance = 0;
double pitch;
double roll;
double yaw;

uint8_t mpuIntStatus;
uint8_t devStatus;
uint16_t packetSize;
uint8_t fifoBuffer[64];

Quaternion q;
VectorFloat gravity;
VectorInt16 aa;
VectorInt16 aaReal;
float ypr[3];

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




unsigned int SERVO_POS = 0; //変数SERVO_POS=0とする
byte ID[4]; //IDはそれぞれのモーターのID
s16 Position[4]; //Positionはモーターが回る角度(右一回転＝4095)
u16 Speed[4]; 
byte ACC[4];

bool black_count =false;
bool blue_count =0;
int8_t susumu_kaisuu= 10;
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
int receivedData2 =0;
bool while_out = 0;

void setupMPU() {
  Wire.begin();
  Wire.setClock(400000);
  mpu.initialize();
  devStatus = mpu.dmpInitialize();

  mpu.setXAccelOffset(-3550);
  mpu.setYAccelOffset(-1727);
  mpu.setZAccelOffset(1087);
  mpu.setXGyroOffset(25);
  mpu.setYGyroOffset(-29);
  mpu.setZGyroOffset(7);

  if (devStatus == 0) {
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.setDMPEnabled(true);
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    Serial.print("DMP Initialization failed.");
  }
}

void getYawPitchRoll() {
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    yaw = ypr[0] * 180 / M_PI;
    pitch = ypr[1] * 180 / M_PI;
    roll = ypr[2] * 180 / M_PI;

    if (yaw < 0) {
      yaw = 360 + yaw;
    }
    yaw = 360 - yaw;

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

void discover_hisaisha() {//追加分
  while ((Serial7.available() == 0)&&(count4 <50)){
    delay(200);
    count4++;
  Serial.println(count4);
  }
  Serial.println("count4");
  if (Serial7.available() > 0) {
   int receivedData = Serial7.read();
   Serial.println(receivedData);
   Serial.print("count");
   Serial.println(count4);
   delay(2000);
   if (count4 < 25){
    Serial.println("NONE");
    count4 = 0;
   }else if (count4 < 30){
    Serial.println("red|| H");
    //SCServo();
    //SCServo();
    count4 =0;
   }else if(count4 < 35){
    Serial.println("yellow||S");
    //SCServo();
    count4 = 0;
   }else if(count4 < 50){
    Serial.println("green||U");
    count4 =0;
   }else{
    count4 =0;
    
   }
  }else{
    count4=0;
  }
  
  
}
void discover_hisaisha2() {//追加分
  while ((Serial8.available() == 0)&&(count4 <50)){
    delay(200);
    count4++;
    Serial.println(count4);
  }
  Serial.println("count4");
  if (Serial8.available() > 0) {
   int receivedData = Serial8.read();
   Serial.println(receivedData);
   Serial.print("count");
   Serial.println(count4);
   delay(2000);
   if (count4 < 25){
    Serial.println("NONE");
    count4 = 0;
   }else if (count4 < 30){
    Serial.println("red|| H");
    //SCServo();
    //SCServo();
    count4 =0;
   }else if(count4 < 35){
    Serial.println("yellow||S");
    //SCServo();
    count4 = 0;
   }else if(count4 < 50){
    Serial.println("green||U");
    count4 =0;
   }else{
    count4 =0;
    
   }
  }else{
    count4=0;
  }
  
  
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
    Serial1.flush();
  while (count2  <susumu_kaisuu) {
    Position[0] = 576; //go
    Position[1] = -576;
    Position[2] = -576;
    Position[3] = 576;
    sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
    delay(320);
    serialEvent3();//_color_or_loadcell
    //serialEvent1();//_victim_camera1
    //serialEvent2();//_victim_camera2
    count2++;
    if (while_out ==1){
      Serial.println("Black_tile");
      break;
    }

  }
  if (blue_count ==true){
    Serial.println("wait");
    delay(5500);
    blue_count =false;
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
     black_count=true;
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

void get_tof_data(){
  /*static char jsonBuffer[256];
  static size_t index = 0;

  // シリアルデータの読み取り
  while (Serial3.available() > 0) {
    char incomingChar = Serial3.read();

    // 改行（\n または \r\n）でJSONメッセージの終端と判断
    if (incomingChar == '\n' || incomingChar == '\r') {
      if (index > 0) {
        jsonBuffer[index] = '\0'; // 終端文字を追加
        Serial.print("Received data: ");
        Serial.println(jsonBuffer); // デバッグ用

        // JSONを解析
        StaticJsonDocument<256> jsonDoc;
        DeserializationError error = deserializeJson(jsonDoc, jsonBuffer);

        if (error) {
          Serial.print("JSON parse failed: ");
          Serial.println(error.c_str());
        } else {
          for (int i = 1; i <= 6; i++) {
            String sensorKey = "sensor" + String(i);
            if (jsonDoc.containsKey(sensorKey)) {
              distance = jsonDoc[sensorKey];
              Serial.print(sensorKey);
              Serial.print(": ");
              Serial.print(distance);
              Serial.println(" mm");
            }
            if (distance <= 150){
                if (i == 1 || i== 4){
                    FrontWall = true;
                    Serial.println("frontwall");
                    }
                else if (i== 2 || i == 3){
                    LeftWall = true;
                    Serial.println("leftwall");
                }
                else if (i== 5 || i==6){
                    RightWall = true;
                    Serial.println("rightwall");
                }
                
            }
            else {
                if (i == 1 || i== 4){
                    FrontWall = false;
                    Serial.println("no_frontwall");
                    }
                else if (i== 2 || i == 3){
                    LeftWall = false;
                    Serial.println("no_leftwall");
                }
                else if (i== 5 || i==6){
                    RightWall = false;
                    Serial.println("no_rightwall");
                }
            }        
          }
          Serial.println(); // データ区切り
        }
        index = 0; // バッファをリセット
      }
    break;//tofデータが収集し終わりwhileから抜け出す
    } else {
      if (index < sizeof(jsonBuffer) - 1) {
        jsonBuffer[index++] = incomingChar;
      } else {
        // バッファオーバーフロー時の処理
        Serial.println("Buffer overflow detected. Clearing buffer.");
        index = 0;
      }
    }
  }
    Serial.println("Data OK.");
    */
    int received_data = Serial3.read();
    Serial.println(received_data);
    if (received_data == '1') {
      FrontWall = true;
      RightWall = true;
      LeftWall = true;
    }
    if (received_data == '2') {
      FrontWall = false;
      RightWall = true;
      LeftWall = true;
    }
    if (received_data == '3') {
      FrontWall = true;
      RightWall = false;
      LeftWall = true;
    }
    if (received_data == '4') {
      FrontWall = true;
      RightWall = true;
      LeftWall = false;
    }
    if (received_data == '5') {
      FrontWall = true;
      RightWall = false;
      LeftWall = false;
    }
    if (received_data == '6') {
      FrontWall = false;
      RightWall = false;
      LeftWall = true;
    }
    if (received_data == '7') {
      FrontWall = false;
      RightWall = true;
      LeftWall = false;
    }
    if (received_data == '8') {
      FrontWall = false;
      RightWall = false;
      LeftWall = false;
    }
    }

void move_colororbump(){
  if (receivedData2 == 'B'){
    while_out = 1;
  }
  else if (receivedData2 == 'A'){
    Serial.println("Blue_tile");
    blue_count = true;
  }
  else if(receivedData2 == 'L'){
    Serial.println("bump_left");
      delay(100);
      Position[0] = 341; //左に回転
      Position[1] = 341;
      Position[2] = 341;
      Position[3] = 341;
      sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
      delay(210);
      delay(150);
      Position[0] = -620; //前に進む
      Position[1] = 620;
      Position[2] = 620;
      Position[3] = -620;
      sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
      delay(420);
      Position[0] = -341; //右に回転
      Position[1] = -341;
      Position[2] = -341;
      Position[3] = -341;
      sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
      delay(210);
      delay(150);
      count2--;
  }
  else if(receivedData2 == 'R'){
    Serial.println("bump_right");
      delay(100);
      Position[0] = -341; //左に回転
      Position[1] = -341;
      Position[2] = -341;
      Position[3] = -341;
      sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
      delay(210);
      delay(150);
      Position[0] = -620; //前に進む
      Position[1] = 620;
      Position[2] = 620;
      Position[3] = -620;
      sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
      delay(420);
      Position[0] = 341; //右に回転
      Position[1] = 341;
      Position[2] = 341;
      Position[3] = 341;
      sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
      delay(210);
      delay(150);
      count2--;
  }

    }



/*
//get_victim_data
void serialEvent1(){
  if (Serial7.available() >0){
    int receivedData7 = Serial7.read();
    Serial7.flush();
    discover_hisaisha();
  }
}


*/
/* 
//get_victim_data
void serialEvent2(){
    if (Serial8.available() >0){
    int receivedData8 = Serial8.read();
    Serial8.flush();
    discover_hisaisha2();
  }
}
*/
//collect_color_and_bump_data
void serialEvent(){
    Serial3.flush();
    while (Serial3.available()== 0){
      Serial.println("WAIT");
        delay(100);
        }
    if (Serial3.available() >0){
        get_tof_data();
        }
    
    }

  void serialEvent3(){
    if (Serial1.available() >0){
    receivedData2 = Serial1.read();
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
    Serial.println("Start");
    delay(10000);

  for (int t = 0; t < 90; t++) {//到達回数を初期化
    for (int j = 0; j < 90; j++) {
        TileReach[t][j] = 0;
        }
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
                    migi();
                    susumu_heitan();//send right moving signal
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
                    susumu_heitan();//send front moving signal
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
                    hidari();
                    susumu_heitan();//send left moving signal
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
                    migi();
                    migi();
                    susumu_heitan();//send return moving signal
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
                    migi();
                    susumu_heitan();//send right moving signal
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
                    susumu_heitan();//send front moving signal
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
                    hidari();
                    susumu_heitan();//send left moving signal
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
                    migi();
                    migi();
                    susumu_heitan();//send return moving signal
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
                    migi();
                    susumu_heitan();//send right moving signal
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
                    susumu_heitan();//send front moving signal
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
                    hidari();//send left moving signal
                    susumu_heitan();
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
                    migi();
                    migi();
                    susumu_heitan();//send return moving signal
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
                    migi();
                    susumu_heitan();//send right moving signal
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
                    susumu_heitan();//send front moving signal
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
                    hidari();
                    susumu_heitan();//send left moving signal
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
                    migi();
                    migi();
                    susumu_heitan();//send return moving signal
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


void loop(){

    serialEvent();
    move(i,Judge(x,y,i));
    //このあとに一定時間がたったら帰還するプログラムを入れる
}