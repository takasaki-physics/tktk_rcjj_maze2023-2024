/*

関数の編集時には記録を残してください

*/



#include <stdio.h>
#include <cstdint>
#include <Arduino.h>
#include <time.h>
#include <cstdint>
#include <SCServo.h> //SCServoを使うと宣言]
#include <SoftwareSerial.h> //SoftwareSerial(複数機器とシリアル通信)を使うと宣言
#include "MPU6050_6Axis_MotionApps20.h"
#include <queue>
#include <stack>

std::stack <uint8_t> S;
std::queue<uint8_t> Q;
MPU6050 mpu;
SMS_STS sms_sts;
SoftwareSerial softSerial(36,37);

#define East 1
#define North 2
#define West 3
#define South 4

#define Right 1
#define Front 2
#define Left 3
#define Back 4

/*デバッグ用*/
uint8_t Homecount = 0;


uint8_t x = 50;
uint8_t y = 50;
uint8_t Direction = North;
uint8_t Status = 0;

uint8_t CheckX = 0;
uint8_t CheckY = 0;
uint8_t CheckD = 0;

/*MPU変数*/
// MPU control/status vars
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


/*******************************************************************************************/
/* Yaw・Pitch・Roll角の取得                                                                            
/*処理：MPU6050を用いて回転や坂検知のためのRoll・Yaw角の取得
/*
/*更新者：清田侑希　2025/1/25
/*
/*******************************************************************************************/
void getYawPitchRoll() {
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);

    yaw = ypr[0] * 180 / M_PI;
    pitch = ypr[1] * 180 / M_PI;
    roll = ypr[2] * 180 / M_PI;
    //yaw角のデータを常に正にする
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

/*.....tof受信用の変数.......................................*/

const byte HEADER = 255;  // ヘッダー (送信側と一致すること)
bool headerDetected = false;
byte receivedData[6];  // 受信データ格納用
int receivedIndex = 0; // データ格納位置
/*センサー類変数*/
unsigned int SERVO_POS = 0; //変数SERVO_POS=0とする
byte ID[4]; //IDはそれぞれのモーターのID
s16 Position[4]; //Positionはモーターが回る角度(右一回転＝4095)
u16 Speed[4]; 
byte ACC[4]; 
int susumu_kaisuu= 40;
int i=1;
int n=1;
double kakudo; //tiziki()でのyawの代入
int kakudo_true; //tiziki()でyawを整数化したあとの変数
double kakudo2; //tiziki_kaitenn()でのyawの代入
int kakudo_true2; //tiziki_kaitenn()でyawを整数化したあとの変数
int mawasu; //どこまで機体を回すかの変数
bool chousei =0; //MPUが0度と360度になったときの分岐
double katamuki; //tiziki2()でのrollの代入
int katamuki_true; //tiziki2()でrollを整数化したあとの変数
int count2 = 0;//進んだ回数＿40回でわかれている
int bump_giveup_count = 0; //障害物に当たった時あきらめる変数

bool right_wall = false;//ここもセンサーの値を取得するファイルとして分けたい
bool front_wall = false;
bool left_wall = false;
int8_t BlackTile =false;
int8_t blue_count =false;
bool Slope = false;//sloopがあるかどうか
bool Gap = false;



/*******************************************************************************************/
/* 地磁気回転                                                                            
/*処理：回転の際に用いるYaw角のデータを整数化
/*
/*更新者：清田侑希 2025/1/25
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
/*処理：回転後に発生する機体の方向の誤差をMPU6050によって補正
/* 変更点：モーターにギアを導入したのでモーターの回転方向が逆になってます
/*更新者：清田侑希 2025/1/25
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
        Position[0] = 60; //右に回転
        Position[1] = 60;
        Position[2] = 60;
        Position[3] = 60;
        sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
        delay(18);
      
    }
  }else if (kakudo_true >= 45 && kakudo_true < 90) {
    for (int i =0; i < 90-kakudo_true ;i++){
        Position[0] = -60; //左に回転
        Position[1] = -60;
        Position[2] = -60;
        Position[3] = -60;
        sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
        delay(18);
      
    }
  }else if (kakudo_true >= 90 && kakudo_true < 135) {
    for (int i =0; i < kakudo_true-90 ;i++){
        Position[0] = 60; //右に回転
        Position[1] = 60;
        Position[2] = 60;
        Position[3] = 60;
        sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
        delay(18);
      
    }
  }else if (kakudo_true >= 135 && kakudo_true < 180) {
    for (int i =0; i < 180-kakudo_true ;i++){
        Position[0] = -60; //左に回転
        Position[1] = -60;
        Position[2] = -60;
        Position[3] = -60;
        sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
        delay(18);
      
    }
  }else if (kakudo_true >= 180 && kakudo_true < 225) {
    for (int i =0; i < kakudo_true-180 ;i++){
        Position[0] = 60; //右に回転
        Position[1] = 60;
        Position[2] = 60;
        Position[3] = 60;
        sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
        delay(18);
      
    }
  }else if (kakudo_true >= 225 && kakudo_true < 270) {
    for (int i =0; i < 270-kakudo_true ;i++){
        Position[0] = -60; //左に回転
        Position[1] = -60;
        Position[2] = -60;
        Position[3] = -60;
        sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
        delay(18);
      
    }
  }else if (kakudo_true >= 270 && kakudo_true < 315) {
    for (int i =0; i < kakudo_true-270 ;i++){
        Position[0] = 60; //右に回転
        Position[1] = 60;
        Position[2] = 60;
        Position[3] = 60;
        sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
        delay(18);
      
    }
  }else if (kakudo_true >= 315 && kakudo_true <= 360) {
    for (int i =0; i < 360-kakudo_true ;i++){
        Position[0] = -60; //左に回転
        Position[1] = -60;
        Position[2] = -60;
        Position[3] = -60;
        sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
        delay(18);
      
    }
  }
  Serial.println("kakudo_hosei");
  delay(100);
}



/*******************************************************************************************/
/* 地磁気２                                                                              
/*処理：坂の検知に用いるRoll角の整数化
/*変更点：MPU6050の設置方向の関係からPitch角からRoll角に仕様変更
/*更新者：清田侑希 2025/1/25
/*
/*******************************************************************************************/
void tiziki_2(){
  getYawPitchRoll();
  katamuki = roll;
  katamuki_true = int(katamuki);
  Serial.println(katamuki_true); 
  
}

/*******************************************************************************************/
/* collect_tof_data                                                                              
/*処理： ヘッダーが来たら6バイト距離のデータを取得するこれによって各壁の検知を行う
/*
/*更新者：清田侑希　2025/1/26
/*清田侑希　2025/2/23 変更点：壁の有無を各方向の2つのセンサーがともに壁を検知したときのみ壁とみなす
/*******************************************************************************************/
void get_tof_data() {
    while (Serial3.available() > 0) {
      byte incomingByte = Serial3.read(); // Serial3から1バイト読み取り
      if (!headerDetected) {
        if (incomingByte == HEADER) {
          headerDetected = true;  // ヘッダー検出
          receivedIndex = 0;      // データ格納位置をリセット
          Serial.println("Header detected!"); // デバッグ用
        }
      } else {
        receivedData[receivedIndex++] = incomingByte; // データを格納
        if (receivedIndex == 6) {
          // 6バイト受信完了
          headerDetected = false;
          // 受信したデータを表示
        for (int i = 0; i < receivedIndex; i++) {
            Serial.print("Sensor ");
            Serial.print(i);
            Serial.print(": ");
            Serial.println(receivedData[i], BIN); // 受信データを2進数で表示

        }

        //壁の有無を変数に代入
        if (receivedData[0] == 1 && receivedData[3] == 1) {
            front_wall = true;
            Serial.println("front_wall");
        } else {
            front_wall = false;
        }
        
        if (receivedData[1] == 1 && receivedData[2] == 1) {
            left_wall = true;
            Serial.println("left_wall");
        } else {
            left_wall = false;
        }
        
        if (receivedData[4] == 1 && receivedData[5] == 1) {
            right_wall = true;
            Serial.println("right_wall");
        } else {
            right_wall = false;
        }

        Serial.println("All data received");
        //座標更新のフェーズに移行
        Status = 1;
   
        }
      }
    }
}


/*******************************************************************************************/
/* get_victim_data                                                                              
/*処理：OpenMVによって被災者の発見
/*
/*更新者：清田侑希 2025/2/23
/*
/*******************************************************************************************/
//右側のカメラ
void discover_hisaisha() {//追加分
  int count4 = 0; //待つ時間によって判定
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

//get_victim_data_右
void Cam1_serial(){
  if (Serial7.available() >0){
    int receivedData7 = Serial7.read(); 
    discover_hisaisha();
    Serial7.flush();
  }
}

/*******************************************************************************************/
/* get_victim_data2                                                                              
/*処理：OpenMVによって被災者を発見する
/*
/*更新者：清田侑希　2025/2/23
/*
/*******************************************************************************************/
//左側のカメラ
void discover_hisaisha2() {//追加分
  int count4 = 0; //待つ時間によって判定
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

//get_victim_data_左
void Cam2_serial(){
  if (Serial8.available() >0){
    int receivedData7 = Serial8.read(); 
    discover_hisaisha2();
    Serial8.flush();
  }
}

/*******************************************************************************************/
/* 右回転                                                                              
/*処理：MPU6050で取得したデータをもとに90度右回転するまでモーターを回し続ける
/*変更点：ギア搭載による回転方向の反転と適切なdelayの実装
/*更新者：清田侑希　2025/1/25
/*       清田侑希　2025/3/7 変更点：回転方法の変更
/*******************************************************************************************/
void migi(){
  tiziki_kaitenn();
  if (kakudo_true2 <=90){
    mawasu = kakudo_true2 +280;
    kakudo_true2 =kakudo_true2 +360;
    chousei =1;
  }else if (kakudo_true2 <= 360){
    mawasu = kakudo_true2-80;
  }
/*
  //90度回転させる
  Position[0] = 8190;
  Position[1] = 8190;
  Position[2] = 8190;
  Position[3] = 8190;
  delay(1884);
  delay(200);
  //現在の角度を取得する
  tiziki_kaitenn(); 
  if (kakudo_true2 - mawasu >= 300){
    mawasu = 360;
  }
    */
  //仮にバンプなどで90度回らなかったら90度回るまで補正し続ける
  while (kakudo_true2 > mawasu){
  Position[0] = 440; //右に回転
  Position[1] = 440;
  Position[2] = 440;
  Position[3] = 440;
  sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
  delay(100);
  tiziki_kaitenn();
  Cam1_serial();
  //Cam2_serial();
  if (chousei ==1){
    if (kakudo_true2 <=90){
      kakudo_true2 =kakudo_true2 +360;
    }else {
      chousei =0;
      }
    }
    delay(50);
  }
  mawasu =0;
  delay(200);
  tiziki(); //角度補正            
}




/*******************************************************************************************/
/* 左回転                                                                              
/*処理：MPU6050で取得したデータをもとに90度左回転するまでモーターを回し続ける
/*変更点：ギア搭載による回転方向の反転と適切なdelayの実装
/*更新者：清田侑希　2025/1/25
/*　清田侑希　2025/3/12 変更点：90度モーターのみで回転し、90度回らなかったら補正を行う
/*******************************************************************************************/
void hidari(){
  //どこまで回すかの数値を処理
  tiziki_kaitenn();
  if (kakudo_true2 >= 270){
    mawasu = kakudo_true2 - 280; 
    kakudo_true2 = kakudo_true2 - 360;
    chousei = 1; 
  }else if (kakudo_true2 >= 0){
    mawasu = kakudo_true2 + 80;
  }
/*
    Position[0] = -8190;
    Position[1] = -8190;
    Position[2] = -8190;
    Position[3] = -8190;
    if(kakudo_true2 <= -300){
        mawasu = 0;
    }
    */
    //90度回転しなかったときに角度の補正を行う
  while (kakudo_true2 < mawasu || chousei ==1){
  Position[0] = -440; //左に回転
  Position[1] = -440;
  Position[2] = -440;
  Position[3] = -440;
  sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
  delay(100);
  tiziki_kaitenn();
  Cam1_serial();
  //Cam2_serial();
  if (chousei ==1){
    if (kakudo_true2 >= 270){
      kakudo_true2 =kakudo_true2 - 360;
      
    }else {
      chousei =0;
    }
  }
  delay(50);
  }
  delay(200);
  tiziki(); //角度補正         
}

/*******************************************************************************************/
/*色タイルまたはバンプ                                                                              
/*処理：Serial1にデータが来たら1バイト読み込んで青、黒、ロードセルを検知する
/*Data 1:黒タイル　2：青タイル　3：左の障害物　4：右の障害物　5：前の壁　6：銀のタイル
/*更新者：清田侑希　2025/1/26
/*清田侑希　2025/2/23 変更点：障害物を検知して最初の動作の時に壁との距離を測るようにした
/*******************************************************************************************/
void get_color_bump() {
  if (Serial1.available()>0){
  int receivedData2 = Serial1.read();
  Serial.println(receivedData2);
  delay(50);
  if(receivedData2 == 1){
    BlackTile = true;
    while (count2 >0){
      Position[0] = -303; //back
      Position[1] = 303;
      Position[2] = 303;
      Position[3] = -303;
      sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
      Serial.println("Backing...");
      delay(100);
      count2--;
    }
    count2 = 40; //進むwhile文から出る
  }else if(receivedData2 == 2){
    Serial.println("blue_tile");
    delay(50);
    blue_count = true;
  }else if (bump_giveup_count == 0){
        receivedIndex = 0; //tofデータを取得するためにバイト数を初期化する
        front_wall = false; //怖いので初期化
        while (receivedIndex != 6){
            get_tof_data();
        }
        if (front_wall == true){
            Position[0] = -2025; //後ろに下がる
            Position[1] = 2025;
            Position[2] = 2025;
            Position[3] = -2025;
            sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
            Serial.println("Backing...");
            delay(1000);
            if (count2 < 20 ){
                //座標の変更をもとに戻す
                Gap = true;
            }
            count2 = 40;            
        }
        bump_giveup_count++;     
    
  }else if(receivedData2 == 5 || bump_giveup_count > 3){
    if(receivedData2== 5 || bump_giveup_count > 3){
        Position[0] = -2025; //後ろに下がる
        Position[1] = 2025;
        Position[2] = 2025;
        Position[3] = -2025;
        sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
        Serial.println("Backing...");
        delay(1000);
        if (count2 < 20 && bump_giveup_count > 1){
            //座標の変更をもとに戻す
            Gap = true;
        }
        count2 = 40;
    }

  }else if(receivedData2 == 3 ){
    Serial.println("left");
    Position[0] = -1023; //左に回転
    Position[1] = -1023;
    Position[2] = -1023;
    Position[3] = -1023;
    sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
    delay(314);
    delay(500);
    Position[0] = -1240; //後ろに下がる
    Position[1] = 1240;
    Position[2] = 1240;
    Position[3] = -1240;
    sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
    delay(420);
    delay(500);
    Position[0] = 1534; //右に回転
    Position[1] = 1534;
    Position[2] = 1534;
    Position[3] = 1534;
    sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
    delay(471);
    delay(500);
    count2= count2-4;
    bump_giveup_count++;
  }else if(receivedData2 == 4){
    Serial.println("right");
    Position[0] = 1023; //右に回転
    Position[1] = 1023;
    Position[2] = 1023;
    Position[3] = 1023;
    sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
    delay(314);
    delay(500);
    Position[0] = -1240; //後ろに下がる
    Position[1] = 1240;
    Position[2] = 1240;
    Position[3] = -1240;
    sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
    delay(420);
    delay(500);
    Position[0] = -1534; //左に回転
    Position[1] = -1534;
    Position[2] = -1534;
    Position[3] = -1534;
    sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
    delay(471);
    delay(500);
    count2= count2-4;
    bump_giveup_count++;
  }else if(receivedData2 == 6){
        Serial.println("Check_Point");
        //現在の座標をチェックポイントの座標として代入する
        CheckX = x;
        CheckY = y;
        CheckD = Direction;

  }else{
    Serial.println("No sensors");
    delay(100);
  }

  }

}

/*******************************************************************************************/
/* 直進                                                                              
/*処理：モーターを回しながら直進この時カラーセンサーとロードセル、カメラの割り込みと坂の検知が入る
/*
/*更新者：清田侑希　2025/1/26
/*
/*******************************************************************************************/
void susumu_heitan() {
    while (Serial1.available() > 0) {
      char receivedChar = Serial1.read(); // データを読み取る
      // 必要に応じて受信データを処理する
    }
  while (count2  <susumu_kaisuu) {
    Serial.println("Going...");
    Position[0] = 303; //go
    Position[1] = -303;
    Position[2] = -303;
    Position[3] = 303;
    sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
    delay(90);
    get_color_bump(); //color_load
    Cam1_serial();//_victim_camera1
    Cam2_serial();//_victim_camera2
    count2++;
  }

  /*............青タイルがある場合の待機............*/

  if (blue_count == true){
    Serial.println("Waiting...");
    delay(5500);
  }
    
    /*...........坂があるかどうか..............*/
    tiziki_2(); //get_roll_data
    delay(50);
    if (katamuki_true <-8){
        Serial.println("Slope_Ue");
        while(katamuki_true <-8){
            Serial.println("Climbing...");
            Position[0] = 400; //前に進む
            Position[1] = -400;
            Position[2] = -400;
            Position[3] = 400;
            sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
            delay(100);
            tiziki_2(); //get_roll_data
            delay(100);
            Slope = true;
        }
        Position[0] = 8190; //前に進む
        Position[1] = -8190;
        Position[2] = -8190;
        Position[3] = 8190;
        sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
        delay(2500);
    }else if (katamuki_true >8){
        while(katamuki_true > 8){
            Serial.println("Down Hill...");
            Position[0] = 400; //前に進む
            Position[1] = -400;
            Position[2] = -400;
            Position[3] = 400;
            sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
            delay(100);
            tiziki_2(); //get_roll_data
            delay(100);
            Slope = true;
        }
        Position[0] = 8190; //前に進む
        Position[1] = -8190;
        Position[2] = -8190;
        Position[3] = 8190;
        sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
        delay(2500);
    }
    while (Serial3.available() > 0) {
        char receivedChar = Serial3.read(); // データを読み取る
        // 必要に応じて受信データを処理する
    }
    tiziki();
    delay(300);
  /*..............変数初期化.................................*/
  count2 =0;
  bump_giveup_count =0;
  Status = 0;
}











/*スタック・キュー用変数
const int MAX = 100; // キュー配列の最大サイズ
*/

/*******************************************************************************************/
/* スタック・キュー用の関数                                                                             
/*処理：インターネットに転がってるものを借用。ググってくれ！
/*
/*更新者：吉ノ薗2025/01/22「既存のライブラリだとリングバッファがないからつくった」
/*　　　　吉ノ薗2025/02/01　ライブラリに移行
/*
/*******************************************************************************************/

/*int st[MAX]; // スタックを表す配列
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
*/

/*
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
*/


/*アルゴリズム用変数*/
int8_t toutatu_zahyou[90][90];//そのマスの到達回数
int RightWeight = 0;
int FrontWeight = 0;
int LeftWeight = 0;

bool start_Gohome = false; //SSD1306に帰還アルゴリズムに入っているかどうかを送信するかの変数

/*帰還用変数*/
int8_t kabe_zahyou[90][90];//0000 の4ビットに絶対方向の東8西4南2北1をそれぞれ割り当てるみたいな
int8_t cost[90][90];//マス目のコスト
bool reach_time[90][90];//到達の有無（帰還用）
long now_seconds;
long firstseconds;

/*******************************************************************************************/
/* 座標などのデータ送信                                                                             
/*処理：Serial1を用いて帰還しているかどうか、右、前、左の重み、X,Y座標、方向のデータをSSD1306に送信する
/*
/*更新者：清田侑希　2025/1/30
/*　　　　
/*
/*******************************************************************************************/

void send_display(){
  // 各センサーの距離を取得し壁の有無を判定


  byte data_for_send[7];
  data_for_send[0] = (start_Gohome) ? 0x01 : 0x00;
  data_for_send[1] = (RightWeight);
  data_for_send[2] = (LeftWeight);
  data_for_send[3] = (FrontWeight);
  data_for_send[4] = (x);
  data_for_send[5] = (y);
  data_for_send[6] = (Direction);

  Serial3.write(255);
  Serial.println("Sent header: 255");
  delay(50);//wait
  // 判定データ送信
  for (int i = 0; i < 7; i++) {
    Serial3.write(data_for_send[i]);
  }
    
  

    Serial.print("Sent data:");//デバック用にシリアルモニタに送信
    for (int i = 0; i < 7; i++) {
      Serial.print(data_for_send[i], BIN);
      if (i < 6) Serial.print(", ");
    }
    Serial.println();


  delay(200); // 必要に応じて調整
}

/*******************************************************************************************/
/*行き止まりの検索                                                                              
/*処理：全てのマスについて、
/*      前後左右の三か所以上が行き止まりor壁のときそのマスの到達回数を+100する（マスの到達回数が100未満のとき）
/*
/*更新者：吉ノ薗2025/02/02いったん凍結
/*
/*******************************************************************************************/
/*void EffectiveDeadEnd(){

    int NorthReached = 0;
    int EastReached = 0;
    int WestReached = 0;
    int SouthReached = 0;

    for (int t = 1; t < 89; t++) {
        for (int j = 1; j < 89; j++) {

            int DeadEndCount = 0;

            NorthReached = toutatu_zahyou[t][j-1];
            EastReached  = toutatu_zahyou[t+1][j];
            WestReached  = toutatu_zahyou[t-1][j];
            SouthReached = toutatu_zahyou[t][j+1];
            
            if(toutatu_zahyou[t][j] < 100){

                if((NorthReached >= 100) || (kabe_zahyou[t][j] & 1)){DeadEndCount++;}//北に壁がある
                if((SouthReached >= 100) || (kabe_zahyou[t][j] & 2)){DeadEndCount++;}//南に壁がある
                if((WestReached >= 100) || (kabe_zahyou[t][j] & 4)){DeadEndCount++;}//西に壁がある
                if((EastReached >= 100) || (kabe_zahyou[t][j] & 8)){DeadEndCount++;}//東に壁がある

                //前後左右の三か所以上が行き止まりor壁のとき、そのマスの到達回数を+100する
                if(DeadEndCount >= 3){
                    toutatu_zahyou[t][j] += 100;
                }

            }
        }
    }
}*/

/*******************************************************************************************/
/* ジャッジ                                                                            
/*処理：前左右を重みづけして、行く方向を決定
/*      右+1、前+2、左+3することで基本的に右に行く拡張右手法を実現
/*      壁があったらさらにプラスする
/*
/*更新者：吉ノ薗2025/01/22
/*       清田侑希　2025/1/30    座標などのデータ更新
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

    //壁がある場合１０００にする
    if (right_wall){
        RightWeight = 1000;
    }
    if (front_wall){
        FrontWeight = 1000;
    }
    if (left_wall){
        LeftWeight = 1000;
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

    if(toutatu_zahyou[x][y] > 100){toutatu_zahyou[x][y] = 100;}//捕捉：走行中に右の重みが94っていう出るはずのない値がでたためオーバーフローを疑いこの関数を導入。対処療法であるため根本的な解決には至っていない

    if ((RightWeight == 1000) && (FrontWeight == 1000) && (LeftWeight == 1000)){//if all wall
        GoTo = Back;
        toutatu_zahyou[x][y] = 50;//行き止まりだから効率化のため二度と行かないようにする
    }

    send_display();
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
    //EffectiveDeadEnd();//より効率的な行き止まりの検索

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
                        y += -1;
                        Direction = North;
                        break;
                    }
                break;
        }
    toutatu_zahyou[x][y] += 1;//移動先のマスの到達回数をプラスしている
    
}



/*******************************************************************************************/
/* タイルの状態                                                                              
/*処理：それぞれの方向について、
/*      黒タイルまたは"少しずれている"の信号が送られていたらそのマスの重みを100にして一マス戻る
/*      坂の信号が送られていたら一マス進む
/*      黒タイルと坂信号の初期化
/*
/*更新者：吉ノ薗2025/01/22
/*
/*******************************************************************************************/
void TileStatus()
{
    if(BlackTile){
        toutatu_zahyou[x][y] += 100;
    }

    switch (Direction)
    {
    case North:
        if(BlackTile || Gap){//黒タイルまたはずれの信号が送られていた場合戻る
            y += 1;
        }
        if(Slope){//坂の信号が送られていた場合座標を更に進める
            y += -1;
        }
        break;

    case East:
        if(BlackTile || Gap){//黒タイルの信号が送られていた場合戻る
            x += -1;
        }
        if(Slope){//坂の信号が送られていた場合座標を更に進める
            x += 1;
        }
        break;
    
    case West:
        if(BlackTile || Gap){//黒タイルの信号が送られていた場合戻る
            x += 1;
        }
        if(Slope){//坂の信号が送られていた場合座標を更に進める
            x += -1;
        }
        break;

    case South:
        if(BlackTile || Gap){//黒タイルの信号が送られていた場合戻る
            y += -1;
        }
        if(Slope){//坂の信号が送られていた場合座標を更に進める
            y += 1;
        }
        break;
    
    
    }
    BlackTile = false;
    Slope = false;
    Gap = false;
}








/*******************************************************************************************/
/* 最短経路の方位                                                                              
/*処理：現在のマスのコストの、-1のコストのマスを探す
/*更新者：吉ノ薗2025/01/22
/*
/*******************************************************************************************/
int WhichWay(uint8_t a,uint8_t b)
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
/*　　　　吉ノ薗 2025/02/01　変更点：スタックとキューをライブラリにして座標の計算をビット演算に変更
/*
/*******************************************************************************************/
void BFS()
{
    //hidari();//デバッグ用


    uint8_t a = x;
    uint8_t b = y;
    cost[a][b] = 1;//現在地のコストを1にする
    Serial.print("/n GotoHome:");

    while(!(a == 50 && b == 50)){

        reach_time[a][b] = 1;//そのマスを訪問済みにする
        for(int n = 1; n <= 8; n *= 2){//そのマスの周りのマスのコストを＋１する
            //int result = static_cast<int>(pow(2, n));
            Serial.print("/n n =");
            Serial.print(n);

            Serial.print("/n kabe_zahyou[a][b] ==");
            Serial.print(kabe_zahyou[a][b]);

            Serial.print("/n cost[a][b] == ");
            Serial.print(cost[a][b]);

            delay(300);

            if(!(kabe_zahyou[a][b] & n)) {//kabe_zahyou[][]は0000 の4ビットに絶対方向の東8西4南2北1をそれぞれ割り当てる

                switch(n){
                    case 1://北の壁がない
                        if((!reach_time[a][b-1]) && !(kabe_zahyou[a][b-1] & 16)){//その先のマスが訪問済みでない&その先のマスが探索済み
                            cost[a][b-1] = cost[a][b] + 1;
                            //キューの末尾に入れる
                            Q.push(a);
                            Q.push(b-1);
                        }
                        break;

                    case 2://南の壁がない
                        if((!reach_time[a][b+1]) && !(kabe_zahyou[a][b+1] & 16)){//その先のマスが訪問済みでない&その先のマスが探索済み
                            cost[a][b+1] = cost[a][b] + 1;
                            //キューの末尾に入れる
                            Q.push(a);
                            Q.push(b+1);
                        }
                        break;

                    case 4://西の壁がない
                        if((!reach_time[a-1][b]) && !(kabe_zahyou[a-1][b] & 16)){//その先のマスが訪問済みでない&その先のマスが探索済み
                            cost[a-1][b] = cost[a][b] + 1;
                            //キューの末尾に入れる
                            Q.push(a-1);
                            Q.push(b);
                        }
                        break;

                    case 8://東の壁がない
                        if((!reach_time[a+1][b]) && !(kabe_zahyou[a+1][b] & 16)) {//その先のマスが訪問済みでない&その先のマスが探索済み
                            cost[a+1][b] = cost[a][b] + 1;
                            //キューの末尾に入れる
                            Q.push(a+1);
                            Q.push(b);
                        }
                        break;
                }

            }
            /*else{//その方向に壁があるとき
                kabe_zahyou[a][b] &= ~n;  // n のビットを 0 にする（壁を削除）これビットだしいらない気がする
            }*/
        }
        //キューの先頭を取り出す
        if (Q.size() < 2) break;  // キューの要素が足りない場合は終了

        a = Q.front(); 
        Serial.print("/n QueX =");
        Serial.print(a);
        Q.pop();

        b = Q.front(); 
        Serial.print("/n QueY =");
        Serial.print(b);
        Q.pop();

        //if (Q.empty()) break;

        /*if((a == 50)&&(b == 50)){
            break;
        }*/
    }



    //スタックを使って逆探索
    a = 50;
    b = 50;

    /*デバッグ用*/
    /*if(WhichWay(a,b) == 0){
        migi();
    }*/

    S.push(4);//停止用
    while(!((a == x) && (b == y))){

        switch(Direction){
            case East:
                switch(WhichWay(a,b)){//前後左右のどこが最短になるか

                    case North://北マスからきたとき（ここのシグナルは探索時の曲がる→進むとは逆で、進む→曲がるじゃないと。）
                        S.push(2);//１：右折、２：左折、３：直進
                        S.push(3);
                        b += -1;
                        Direction = South;
                        break;

                    case West://西マスからきたとき
                        S.push(3);
                        a += -1;
                        break;

                    case South://南マスからきたとき
                        S.push(1);
                        S.push(3);
                        b += 1;
                        Direction = North;
                        break;

                }
                break;

            case North:
                switch(WhichWay(a,b)){//前後左右のどこが最短になるか
                    case East:
                        S.push(1);
                        S.push(3);
                        a += 1;
                        Direction = West;
                        break;

                    case West:
                        S.push(2);
                        S.push(3);
                        a += -1;
                        Direction = East;
                        break;

                    case South:
                        S.push(3);
                        b += -1;
                        break;

                }
                break;

            case West:
                switch(WhichWay(a,b)){//前後左右のどこが最短になるか
                    case East:
                        S.push(3);
                        a += 1;
                        break;

                    case North:
                        S.push(1);
                        S.push(3);
                        b += -1;
                        Direction = South;
                        break;

                    case South:
                        S.push(2);
                        S.push(3);
                        Direction = North;
                        break;

                }
                break;

            case South:
                switch(WhichWay(a,b)){//前後左右のどこが最短になるか
                    case East:
                        S.push(2);
                        S.push(3);
                        a += 1;
                        Direction = West;
                        break;

                    case North:
                        S.push(3);
                        b += -1;
                        break;

                    case West:
                        S.push(1);
                        S.push(3);
                        a += -1;
                        Direction = East;
                        break;

                }
                break;

            /*デバッグ用*/
            default:
                hidari();
        }
        /*if((a == x)&&(b == y)){
            break;
        }*/
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
    if(kabe_zahyou[x][y] & 16){//記録されていない場合（そうしないと延々と加算されちゃう）
        switch (Direction){
            case East:
                if(right_wall){
                    kabe_zahyou[x][y] |= 2;
                }
                if(front_wall){
                    kabe_zahyou[x][y] |= 8;
                }
                if(left_wall){
                    kabe_zahyou[x][y] |= 1;
                }
                break;
            
            case North:
                if(right_wall){
                    kabe_zahyou[x][y] |= 8;
                }
                if(front_wall){
                    kabe_zahyou[x][y] |= 1;
                }
                if(left_wall){
                    kabe_zahyou[x][y] |= 4;
                }
                break;

            case West:
                if(right_wall){
                    kabe_zahyou[x][y] |= 1;
                }
                if(front_wall){
                    kabe_zahyou[x][y] |= 4;
                }
                if(left_wall){
                    kabe_zahyou[x][y] |= 2;
                }
                break;

            case South:
                if(right_wall){
                    kabe_zahyou[x][y] |= 4;
                }
                if(front_wall){
                    kabe_zahyou[x][y] |= 2;
                }
                if(left_wall){
                    kabe_zahyou[x][y] |= 8;
                }
                break;
            }
            //対応
            //e n w s
            //8 4 2 1
        kabe_zahyou[x][y] &= ~16;//4ビットの情報のみが残る
    }
}



/*******************************************************************************************/
/* 帰還                                                                             
/*処理：スタックから値をpopして順番に動いていく
/*      全て終わる、つまりpopした値が一番最初に入れた"4"になっていたら停止（まだ停止部分はつくってない）
/*
/*更新者：吉ノ薗2025/01/22
/*　　　　吉ノ薗2025/01/29：20秒停止するようにした
/*
/*******************************************************************************************/
void GoHome()
{

        //ここ以下を「相手(モーター)から動き終わったという信号が送られたら」とかにしないとバババッて送られちゃうかも
    while(1){
        
        switch(S.top()){
            case 1:
                //TurnRight
                migi();
                delay(300);
                break;

            case 2:
                //TurnLeft
                hidari();
                delay(300);
                break;

            case 3:
                //GoStraight
                susumu_heitan();
                delay(300);
                break;
            case 4:
                //Stop
                delay(20000);
                break;
            /*default:
                hidari();//デバッグ用*/
        }
        S.pop();//要素の削除
    }
    
}



/*******************************************************************************************/
/* セットアップ                                                                              
/*処理：MPU、センサー、モーター、座標の初期化
/*変更点：センサーのセットアップの部分を最初に持ってくる/Serialの初期化のための適切な待機/サーボモーターの速度設定を2倍にする
/*更新者：吉ノ薗陽向　2025/01/22
/*       清田侑希　2025/01/25
/*　　　　吉ノ薗陽向　2025/01/31　変更点：初期位置の到達回数を+1した
/*
/*******************************************************************************************/
void setup(){
/*センサーのセットアップ***********************************************************************************************************/
  Serial.begin(9600);   // デバッグ用
  Serial1.begin(19200); //using_color_load
  Serial2.begin(1000000);//using_servo_sts3032
  Serial3.begin(9600);  // using_tof
  Serial7.begin(19200);//using_cam1
  Serial8.begin(19200);//using_cam2
  Wire.begin();
  pinMode(6, INPUT); //スタート用スイッチのpin設定
  delay(2000);
  /*MPUのセットアップ***********************************************************************************************************/
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
  /*モーターのセットアップ***********************************************************************************************************/
 softSerial.begin( 9600 ); //softSerial通信の速さを9600bit/秒に設定
 sms_sts.pSerial = &Serial2;
 delay(1000);
  ID[0] = 1; //右前のモーターのID  
  ID[1] = 2; //左前
  ID[2] = 3; //右後
  ID[3] = 4; //左後

  Speed[0] = 6800; //変数Speed[0]=0とする
  Speed[1] = 6800;
  Speed[2] = 6800;
  Speed[3] = 6800;

  ACC[0] = 50; //変数ACC[0]=0とする
  ACC[1] = 50;
  ACC[2] = 50;
  ACC[3] = 50;
  Serial.println("test");


  /*スタック・キューのセットアップ***********************************************************************************************************
  init();
  st_init();
*/


  /*アルゴリズムのセットアップ***********************************************************************************************************/
  for (int t = 0; t < 90; t++) {
    for (int j = 0; j < 90; j++) {
        kabe_zahyou[t][j] |= 16;
        reach_time[t][j] = 0;
        cost[t][j] = 0;
        toutatu_zahyou[t][j] = 0; 
        }
    }
    toutatu_zahyou[50][50] = 1;
    // 現在の時刻を取得します
    time_t FirstTime;
    time(&FirstTime);  // 現在の時刻を取得して FirstTime に格納
    firstseconds = static_cast<long>(FirstTime); 
    
}

/*******************************************************************************************/
/* メインループ                                                                              
/*処理：スタートスイッチがONのときメインの行動を行う
/*      statusの値によって壁情報の取得・座標更新と探索・帰還・前後左右への移動を行う
/*      status1のとき壁情報を記録 → 拡張右手法て行く方向を決定 → 現在の時刻とスタートの時刻を照合して、330秒経っていたらstatusを2にするかどうか決定
/*      status2のとき幅優先探索 → 実際に動いて停止
/*      status3,4,5,6のとき前後左右に移動 → 黒タイルや坂だった場合に合わせて座標を更新
/*
/*更新者：吉ノ薗2025/01/22
/*　　　　清田侑希 2025/1/28　変更点：動きの最適化のためにdelayを追加|スタートスイッチの導入
/*　　　　吉ノ薗 2025/01/31　変更点：途中で進行停止になった場合座標をチェックポイントの場所に戻すようにした。
/*　　　　吉ノ薗 2025/02/01　変更点：チェックポイントの方位を追加
/*        
/*******************************************************************************************/
void loop(){
    /*.............スタートスイッチがオンになるまで待機する部分...........*/
    while (digitalRead(6) == LOW) {
        delay(10);
        Serial.println("Waiting...");
        if (digitalRead(6) == HIGH){
             //主に進行停止の時に値がずれるので再初期化する
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
            //チェックポイントとして保存した座標を現在の座標として代入する
            x = CheckX;
            y = CheckY;
            Direction = CheckD;
        }
    }
    /*.................................................................*/
    switch (Status)
    {
    case 0://壁情報取得

        get_tof_data();
        break;

    case 1://座標更新と探索
        WriteDownWall();//帰還用の記録
        
        /*// 現在の時刻を取得
        time_t NowTime;
        time(&NowTime);  // 現在の時刻を取得して NowTime に格納
        now_seconds = static_cast<long>(NowTime); 

        //330秒（＝5分半）経ったら幅優先探索を始める
        if(now_seconds - firstseconds >= 330){
            Status = 2;//帰還開始
            start_Gohome = true;
        }*/


        /*デバッグ用
        Homecount += 1;
        if(Homecount >= 10){
            Status = 2;//帰還開始
            start_Gohome = true;
        }*/

        MoveTo(judge());//拡張右手法で行く方法を決める,実際に移動して座標を変更+到達回数を加算

        break;
    
    case 2://帰還(このとき探索に戻らないよう入れ子構造にする or ここだけは関数内に直接migi()とかを入れてwhile文)   
        BFS();
        GoHome();
        break;

    case 3://直進
        susumu_heitan();
        delay(200);
        TileStatus();
        break;

    case 4://右折
        migi();
        delay(500);
        susumu_heitan();
        delay(200);
        TileStatus();
        break;

    case 5://左折
        hidari();
        delay(500);
        susumu_heitan();
        delay(200);
        TileStatus();
        break;

    case 6://後進
        migi();
        delay(500);
        migi();
        delay(500);
        susumu_heitan();
        delay(200);
        TileStatus();
        break;
    }
    
}