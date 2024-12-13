#pragma once

#include <stdio.h>
#include <Wire.h>
#include <SCServo.h> //SCServoを使うと宣言]
#include <sensor\MPU6050_2024.h>
MPU6050_2024 Mpu;
SMS_STS sms_sts;

class move_2024_2025
{
private:
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
public:
    move_2024_2025();
    bool right_wall = false;//ここもセンサーの値を取得するファイルとして分けたい
    bool front_wall = false;
    bool left_wall = false;
    int8_t BlackTile =false;
    int8_t blue_count =false;
    bool Slope = false;//sloopがあるかどうか
    void tiziki_kaitenn();
    void tiziki();
    void tiziki_2();
    void motor_setup();
    void migi();
    void hidari();
    void susumu_heitan();
};

move_2024_2025::move_2024_2025()
{
  Serial.begin(115200);//using_serial_monitor
  Wire.begin();
  Serial2.begin(1000000);//using_servo_sts3032
  Serial3.begin(19200);//using_tof
  Serial1.begin(19200);//using_Sub_Kairo
  Serial7.begin(19200);//using_cam1
  Serial8.begin(19200);//using_cam2
}