#include <SCServo.h> //SCServoを使うと宣言
#include <SoftwareSerial.h> //SoftwareSerial(複数機器とシリアル通信)を使うと宣言

//モーターについての宣言
SoftwareSerial softSerial(13,14);
SMS_STS sms_sts;

unsigned int SERVO_POS = 0; //変数SERVO_POS=0とする
byte ID[4]; //IDはそれぞれのモーターのID
s16 Position[4]; //Positionはモーターが回る角度(右一回転＝4095)
u16 Speed[4]; 
byte ACC[4];

void setup(){
  Serial.begin(115200);
  Serial2.begin(1000000);
  motor_setup();

}

void loop(){
 susumu_heitan();
 migi();
}

void motor_setup(){
 softSerial.begin( 9600 ); //softSerial通信の速さを9600bit/秒に設定
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
  Serial.println("test");
}

void migi(){
  Position[0] = -4095; //右に回転
  Position[1] = -4095;
  Position[2] = -4095;
  Position[3] = -4095;
  sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
  Serial.println("1");
  delay(1884); //1884ミリ秒待機(1.884秒)
  delay(1884);
}

void susumu_heitan(){
  Position[0] = 4095; //前に進む
  Position[1] = -4095;
  Position[2] = -4095;
  Position[3] = 4095;

  sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
  Serial.println("2");
  delay(1884); //1884ミリ秒待機(1.884秒)
  delay(1884);
}
