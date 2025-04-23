#include <math.h>
#include <Wire.h>
#include <SCServo.h> //SCServoを使うと宣言
SMS_STS sms_sts;

/*...................................................................................*/
//吉ノ薗のアルゴリズムのなかのmove_2024_2025.hにある壁情報の条件
bool right_wall = false;//ここもセンサーの値を取得するファイルとして分けたい
bool front_wall = false;
bool left_wall = false;
/*..................................................................................*/

int receivedData2 =0;
unsigned int SERVO_POS = 0; //変数SERVO_POS=0とする
byte ID[4]; //IDはそれぞれのモーターのID
s16 Position[4]; //Positionはモーターが回る角度(右一回転＝4095)
u16 Speed[4]; 
byte ACC[4];
bool while_out = 0;//susumu_heitannのwhileから抜け出す
bool black_count =0;
bool blue_count =0;
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
bool saka = 0;//sloopがあるかどうか
int count2 = 0;//進んだ回数＿10回でわかれている
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
    Serial1.flush();
  while (count2  <susumu_kaisuu) {
    Position[0] = 576; //go
    Position[1] = -576;
    Position[2] = -576;
    Position[3] = 576;
    sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
    delay(320);
    //serialEvent3();//color_or_loadcell
    //serialEvent1();//victim_camera1
    //serialEvent2();//victim_camera2
    count2++;
    if (while_out == 1)[
        Serial.println("go out susumu");
        while_out = 0;
        break;
    ]
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
    if ( blue_count == 1)[
        delay(5500);//青タイルで5秒待機
        blue_count =0;
    ]
    
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


void get_tof_data(){
  static char jsonBuffer[256];
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
              int distance = jsonDoc[sensorKey];
              Serial.print(sensorKey);
              Serial.print(": ");
              Serial.print(distance);
              Serial.println(" mm");
            }
            if (distance <= 150){
                if (i == 1 || i== 4){
                    front_wall == true;
                    Serial.println("frontwall");
                    }
                else if (i== 2 || i == 3){
                    left_wall == true;
                    Serial.println("leftwall");
                }
                else if (i== 5 || i==6){
                    right_wall == true;
                    Serial.println("rightwall");
                }
                
            }
            else {
                if (i == 1 || i== 4){
                    front_wall == false;
                    Serial.println("no_frontwall");
                    }
                else if (i== 2 || i == 3){
                    left_wall == false;
                    Serial.println("no_leftwall");
                }
                else if (i== 5 || i==6){
                    right_wall == false;
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
    Seial,println("Data OK.");
    }

void move_colororbump(){
    // black_tile
    if (receivedData2 == 'B'){
        Serial.println("Black_tile");
        while_out = 1;

    }

    
    //blue_tile"Ao"
    else if (receivedData2 == 'A'){
        Serial.println("Blue_tile");
        blue_count = 1;
    }
    else if (receivedData2 =='1'){
       Serial.println("Bunp_left");
      delay(100);
      Position[0] = -341; //右に回転
      Position[1] = -341;
      Position[2] = -341;
      Position[3] = -341;
      sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
      delay(315);
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
      delay(315);
      delay(150);
      count2--;
    }
    else if (receivedData2 =='2'){
       Serial.println("Bunp_right");
      delay(100);
      Position[0] = 341; //右に回転
      Position[1] = 341;
      Position[2] = 341;
      Position[3] = 341;
      sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
      delay(315);
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
      delay(315);
      delay(150);
      count2--;
    }

    
    }
//collect_tof_data
void serialEvent(){
    Serial3.flush();
    while (Serial3.available()== 0){
        delay(100);
        }
    if (Serial3.available() >0){
        get_tof_data();
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
    }








