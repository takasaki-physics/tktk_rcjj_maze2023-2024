#include <math.h>
#include <Wire.h>
#include <SCServo.h> //SCServoを使うと宣言
#include <Servo.h>
#include <SoftwareSerial.h> //SoftwareSerial(複数機器とシリアル通信)を使うと宣言
#include "Adafruit_TCS34725.h"
#include "MPU6050_6Axis_MotionApps20.h"
#define commonAnode true
#define BNO055_SAMPLERATE_DELAY_MS (100)
#define MAX_SAMPLES 10
//モーターについての宣言
SoftwareSerial mySerial(10,13);
SoftwareSerial softSerial(2,3);
Servo servo;
MPU6050 mpu;
SMS_STS sms_sts;
#define trigPin 8  // トリガーピンをD8に
#define echoPin 9  // エコーピンをD9に
#define trigPin2 6  // トリガーピンをD8に
#define echoPin2 7 // エコーピンをD9に
float Duration = 0;  // 計測した時間
float Distance = 0;  // 距離
float Duration2 = 0;  // 計測した時間
float Distance2 = 0;  // 距離
int r, g, b;
uint16_t clear;
uint16_t red =500;
uint16_t blue =500;
uint16_t green =500;
const int BUMPER_PIN = 12;
const int BUMPER_PIN2 = 11;
byte gammatable[256];
int black_count =0;
int blue_count =0;
int susumu_kaisuu= 0;
int LED_pin=4;
int i=1;
int n=1;
int count4 = 0;
int tiziki_restart = 0;
double kakudo;
int kakudo_true;
double kakudo2;
int kakudo_true2;
int mawasu;
double katamuki;
int katamuki_true;
bool vic = 0;
bool saka = 0;
double pitch;
double roll;
double yaw;
int count2 = 0;
bool count3 = 0;
bool migi_none =0;
int count5 =0;
bool chousei =0;
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
double ragu =0;

static uint16_t color16(uint16_t r, uint16_t g, uint16_t b) { //RGB値を24ビットから16ビットへ
  uint16_t _color;
  _color = (uint16_t)(r & 0xF8) << 8;
  _color |= (uint16_t)(g & 0xFC) << 3;
  _color |= (uint16_t)(b & 0xF8) >> 3;
  return _color;
}

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

unsigned int SERVO_POS = 0; //変数SERVO_POS=0とする
byte ID[4]; //IDはそれぞれのモーターのID
s16 Position[4]; //Positionはモーターが回る角度(右一回転＝4095)
u16 Speed[4]; 
byte ACC[4];

void getYawPitchRoll() {
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


void setupMPU() {
  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
  mpu.initialize();
  devStatus = mpu.dmpInitialize();
  // supply your own gyro offsets here, scaled for min sensitivity

  mpu.setXAccelOffset(-4410);
  mpu.setYAccelOffset(-2947);
  mpu.setZAccelOffset(1015);
  mpu.setXGyroOffset(143);
  mpu.setYGyroOffset(48);
  mpu.setZGyroOffset(30);
  
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
}


void tiziki_kaitenn()
{
  getYawPitchRoll();


  kakudo2 = yaw;
  kakudo_true2 = int(kakudo2);
  Serial.println(kakudo_true2);
}


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
        Position[0] = 30; //右に回転
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
        Position[0] = 30; //右に回転
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
        Position[0] = 30; //右に回転
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
        Position[0] = 30; //右に回転
        Position[1] = 30;
        Position[2] = 30;
        Position[3] = 30;
        sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
        delay(18);
      
    }
  }
  delay(100);
  // delay(BNO055_SAMPLERATE_DELAY_MS);
}

void tiziki_2(){
  getYawPitchRoll();
  katamuki = pitch;
  katamuki_true = int(katamuki);
  Serial.println(katamuki_true); 
  
}

void serialEvent(){
  if (Serial2.available() >0){
    int receivedData = Serial2.read();
    Serial2.flush();
    discover_hisaisha();
  }
}

void serialEvent2(){
    if (Serial3.available() >0){
    int receivedData = Serial3.read();
    Serial3.flush();
    discover_hisaisha2();
  }
}

void discover_hisaisha() {//追加分
  while ((Serial2.available() == 0)&&(count4 <50)){
    delay(200);
    count4++;
  Serial.println(count4);
  }
  Serial.println("count4");
  if (Serial2.available() > 0) {
   int receivedData = Serial2.read();
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
  while ((Serial3.available() == 0)&&(count4 <50)){
    delay(200);
    count4++;
    Serial.println(count4);
  }
  Serial.println("count4");
  if (Serial3.available() > 0) {
   int receivedData = Serial3.read();
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
  mySerial.begin( 9600 ); //softSerial通信の速さを9600bit/秒に設定
  sms_sts.pSerial = &Serial;
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

    
  
  Position[0] = 220; //右に回転
  Position[1] = 220;
  Position[2] = 220;
  Position[3] = 220;
  sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
  tiziki_kaitenn();
  //serialEvent();
  //serialEvent2();
  if (chousei ==1){
    if (kakudo_true2 >= 270){
      kakudo_true2 =kakudo_true2 - 360;
      
    }else {
      chousei =0;
    }
  }
  delay(70);
  }
  




 /* Position[0] = 4095;
  Position[1] = 4095;
  Position[2] = 4095;
  Position[3] = 4095;
  sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);

  delay(1884);
  delay(100);*/
  tiziki();//1884ミリ秒待機(1.884秒)
  delay(200);
              
}
void susumu_heitan(){
  while (count2  <susumu_kaisuu&&(red > 50 || blue > 50 || green > 50)) {
    Position[0] = 576; //前に進む
    Position[1] = -576;
    Position[2] = -576;
    Position[3] = 576;
    sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
    color();
    //serialEvent();
    //serialEvent2();
   if (digitalRead( BUMPER_PIN ) == LOW &&digitalRead( BUMPER_PIN2 ) == LOW){
    count3=1;
    count2=susumu_kaisuu-1;
   }else if (digitalRead( BUMPER_PIN ) == LOW){
    chouonnpa2();
    if(Distance2 > 10){
      delay(200);
      Position[0] = -682; //右に回転
      Position[1] = -682;
      Position[2] = -682;
      Position[3] = -682;
      sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
      delay(315);
      delay(150);
      Position[0] = -620; //前に進む
      Position[1] = 620;
      Position[2] = 620;
      Position[3] = -620;
      sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
      delay(420);
      Position[0] = 682; //右に回転
      Position[1] = 682;
      Position[2] = 682;
      Position[3] = 682;
      sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
      delay(315);
      delay(150);
      count2--;
    } else {
    count3=1;
    count2=susumu_kaisuu-1;
    }
   }else if (digitalRead( BUMPER_PIN2 ) == LOW){
    chouonnpa();
    if (Distance >10){
      delay(500);
      Position[0] = 682; //右に回転
      Position[1] = 682;
      Position[2] = 682;
      Position[3] = 682;
      sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
      delay(315);
      delay(150);
      Position[0] = -620; //前に進む
      Position[1] = 620;
      Position[2] = 620;
      Position[3] = -620;
      sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
      delay(420);
      Position[0] = -682; //右に回転
      Position[1] = -682;
      Position[2] = -682;
      Position[3] = -682;
      sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
      delay(315);
      delay(150);
      count2--;
    }else {
    count3=1;
    count2=susumu_kaisuu-1;
    } 
   }
    count2++;
   
   
  

  }
  if (count2 != susumu_kaisuu){
    while (count2 !=0){
    Position[0] = -576; //前に進む
    Position[1] = 576;
    Position[2] = 576;
    Position[3] = -576;
    sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
    delay(320);
     count2--;
     black_count=1;
    }
  }
  delay(500);
  chouonnpa(); //migi
  chouonnpa2(); //hidari
  ragu = Distance - Distance2;
  if (Distance < 12 && Distance2 <12){
    if (ragu > 1){
      while (ragu > 0){  // hidrikaitenn
        Position[0] = 30; //右に回転
        Position[1] = 30;
        Position[2] = 30;
        Position[3] = 30;
        sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
        chouonnpa(); //migi
        chouonnpa2(); //hidari
        ragu = Distance - Distance2;
      }
    }else if (ragu < -1){
      while (ragu < 0){  // hidrikaitenn
        Position[0] = -30; //右に回転
        Position[1] = -30;
        Position[2] = -30;
        Position[3] = -30;
        sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
        chouonnpa(); //migi
        chouonnpa2(); //hidari
        ragu = Distance - Distance2;
      }
    }
    while (Distance > 6.5){
      Position[0] = 80; //前に進む
      Position[1] = -80;
      Position[2] = -80;
      Position[3] = 80;
      sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
      chouonnpa(); 
    }
  }else {
    
    tiziki();

  }
  color();
  susumu_kaisuu =0;
  count2 =0;


}

void color_setup() {
    if(!tcs.begin()){
    Serial.println("sensor NO");
    delay(1000);
  }
  Serial.println("Sensor start");

  Serial.println("Color Sensor Test");
  tcs.setIntegrationTime(TCS34725_INTEGRATIONTIME_154MS);
  tcs.setGain(TCS34725_GAIN_4X);
}
void color() {
  delay(60);  // takes 50ms to read   
  tcs.getRawData(&red, &green, &blue, &clear);

  // Figure out some basic hex code for visualization
 
  uint32_t sum = clear;
  r = red*256/sum;
  g = green*256/sum;
  b = blue*256/sum;
  if (b > 105){
    blue_count++;
  }else {
    
    }
   
 

  Serial.print("Clear: ");
  Serial.println(clear);
  
  Serial.print("Red  : ");
  Serial.print(red);
  Serial.print(" (");
  Serial.print(r);
  Serial.println(")");

  Serial.print("Green: ");
  Serial.print(green);
  Serial.print(" (");
  Serial.print(g);
  Serial.println(")");

  Serial.print("Blue : ");
  Serial.print(blue);
  Serial.print(" (");
  Serial.print(b);
  Serial.println(")");

}

void SCServo(){
  servo.write(0);
  delay(1000);
  servo.write(90);
  delay(1000);
}

void chouonnpa(){
    digitalWrite(trigPin,LOW);              // 計測前に一旦トリガーピンをLowに
  delayMicroseconds(2);

  digitalWrite(trigPin,HIGH);             // トリガーピンを10μsの時間だけHighに
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);

  Duration = pulseIn(echoPin,HIGH);      // エコーピンからの入力
  
  Duration = Duration / 2;               // 距離を1/2に
  Distance = Duration*340*100/1000000;   // 音速を掛けて単位をcmに
  Serial.print("Distance");
  Serial.println(Distance);
  
  
}
void chouonnpa2(){
    digitalWrite(trigPin2,LOW);              // 計測前に一旦トリガーピンをLowに
  delayMicroseconds(2);

  digitalWrite(trigPin2,HIGH);             // トリガーピンを10μsの時間だけHighに
  delayMicroseconds(10);
  digitalWrite(trigPin2,LOW);

  Duration2 = pulseIn(echoPin2,HIGH);      // エコーピンからの入力
  
  Duration2 = Duration2 / 2;               // 距離を1/2に
  Distance2 = Duration2*340*100/1000000;   // 音速を掛けて単位をcmに
  Serial.print("Distance2");
  Serial.println(Distance2);
  
}


void setup(){
 Wire.begin();
 Wire.beginTransmission(0x68);
 Wire.write(0x6B);
 Wire.write(0x00);
 Wire.endTransmission();
 Serial.begin(9600); 
 Serial1.begin(9600);
 Serial2.begin(19200);
 Serial3.begin(19200);
 softSerial.begin(19200);
 servo.attach(10, 500, 2400);
  pinMode(echoPin,INPUT);   // エコーピンを入力に
  pinMode(trigPin,OUTPUT);  // トリガーピンを出力に
  pinMode(echoPin2,INPUT);
  pinMode(trigPin2,OUTPUT);
 setupMPU();
 pinMode(10, OUTPUT);
 pinMode( BUMPER_PIN, INPUT_PULLUP );
 pinMode( BUMPER_PIN2, INPUT_PULLUP );
 pinMode(LED_pin,OUTPUT);
 motor_setup();
 color_setup();

}

void loop(){
  while(Serial1.available() > 0){
    //discover_hisaisha();
    int val = Serial1.read();
    // 1バイト分のデータを読み取る
    delay(100);

    //serialEvent();
    //serialEvent2();
 
    if(val == '1'){        
      Serial.println("hello 1");
      migi();
    } else if(val == '2'){
      Serial.println("Hello 2");
      hidari();
    }
    else if(val== '3'){
      Serial.println("Hello 3");
      susumu_kaisuu = 10;
      delay(300);
      susumu_heitan();
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
   //discover_hisaisha();
   if (tiziki_restart == 30){
    setupMPU();
    delay(500);
    tiziki_restart = 0;
   }
   if (saka == 1 && blue_count > 0){
    Serial1.write('7'); 
    blue_count =0;
    saka =0;  
   }else if (saka ==1){
    Serial1.write('8');
    blue_count =0;
    saka = 0;
    
   }else if (blue_count > 0&& count3 == 1){
    Serial1.write('9');
    blue_count =0;
    count3 = 0;
    
   }else if(count3 == 1){
    Serial1.write('3');
    count3 =0;
   }else if (black_count > 0){
    Serial1.write('4');
    Serial.println("black");
    black_count =0;
  }else if (blue_count > 0){
    Serial1.write('5');
    Serial.println("blue");
    blue_count =0;
  }else {
    Serial1.write('6');
  }
    }else if(val == '4'){
      while( digitalRead( BUMPER_PIN ) == HIGH && digitalRead(BUMPER_PIN2) == HIGH ){
          Position[0] = 50; //前に進む
          Position[1] = -50;
          Position[2] = -50;
          Position[3] = 50;
          sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
          delay(30);
          Serial.println("H");
        
      }
    Serial.println("LOW");
    Position[0] = -1000; //前に進む
    Position[1] = 1000;
    Position[2] = 1000;
    Position[3] = -1000;
    sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
    delay(270);
      
    }
    
    
  }


    
  
   
}



  
  
