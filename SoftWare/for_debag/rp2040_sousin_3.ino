#include <stdio.h>
#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include <HX711.h> // ロードセルアンプのライブラリ

//color sensor
#define commonAnode true

// our RGB -> eye-recognized gamma color
byte gammatable[256];

static uint16_t color16(uint16_t r, uint16_t g, uint16_t b) {
  uint16_t _color;
  _color = (uint16_t)(r & 0xF8) << 8;
  _color |= (uint16_t)(g & 0xFC) << 3;
  _color |= (uint16_t)(b & 0xF8) >> 3;
  return _color;
}

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

int blue_send_limit = 5; //青タイルがあることを送信する回数の上限

const int DT_PIN = 27;
const int SCK_PIN = 26;
const int DT_PIN2 = 4;
const int SCK_PIN2 = 3;
HX711 scale, scale2;





void load1_setup(){
  scale.begin(DT_PIN, SCK_PIN);

  Serial.println("read:");
  Serial.println(scale.read());

  scale.set_scale();
  scale.tare();

  Serial.print("calibrating...");
  delay(2000);
  Serial.println(scale.get_units(10));

  scale.set_scale(-375.00);
  scale.tare();

  Serial.print("read (calibrated):");
  Serial.println(scale.get_units(10));
}

void load2_setup(){
  scale2.begin(DT_PIN2,SCK_PIN2);

  Serial.print("read:");
  Serial.println(scale2.read());

  scale2.set_scale();
  scale2.tare();

  Serial.print("calibrating...");
  delay(2000);
  Serial.println(scale2.get_units(10));

  scale2.set_scale(-375.00);
  scale2.tare();

  Serial.print("read (calibrated):");
  Serial.printf("load cell:%g\n",scale2.get_units(10));
}
void load1(){
  scale.power_down();
  delay(500);
  scale.power_up();
}
void load2(){
  scale2.power_down();
  delay(500);
  scale2.power_up();
}
void setup() {
  Serial.begin(115200);
  Serial1.begin(19200);
  delay(2000);
  Serial.println("start");
  load1_setup();
  load2_setup();
}

/*.............ロードセルのひずみデータを計測し壁に衝突しているかを検知する..............*/
void loop() {
//load1();
//load2();

float load = scale.get_units(1);
float load_2 = scale2.get_units(1);
Serial.println(load);
Serial.println(load_2);
if (load<= -900 && load_2 >= 900) {
  Serial1.write(5);
  Serial.println("kabe_mae");
  delay(1500);
}else if(load<=-1200 || load_2<=-1200){
    Serial1.write(3);//left
    Serial.println("kabe_hidari");
    delay(1500); //一定時間送信せず大量送信を防止
  }else if(load>=1200|| load_2>=1200){
    Serial1.write(4);//right
    Serial.println("kabe_migi");
    delay(1500); //一定時間送信せず大量送信を防止
  }// 送信側でloadの値を送信
delay(100);

}


void setup1() {
  Serial.begin(115200);
  Serial1.begin(19200);
  Wire.begin();
  while(!tcs.begin()){
    Serial.println("sensor NO");   
    delay(2000);
   }

  Serial.println("Color Sensor Test");
  tcs.setIntegrationTime(TCS34725_INTEGRATIONTIME_154MS);
  tcs.setGain(TCS34725_GAIN_4X); // put your setup code here, to run once:

}

/*........カラーセンサーの値を読み、床の黒タイル、青タイルを検知する.................*/
void loop1() {
  uint16_t clear, red, green, blue;
  delay(60);  // takes 50ms to read   
  tcs.getRawData(&red, &green, &blue, &clear);

  // Figure out some basic hex code for visualization
  uint32_t sum = clear;
  int r, g, b;
  r = red*256/sum;
  g = green*256/sum;
  b = blue*256/sum;

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

  if( b>=100){
    Serial1.write(2);//blue
    Serial.println("send:2");
    blue_send_limit--;
  }else if (red<=500 && green<=450 && blue<=350){
    Serial1.write(1);//black
    Serial.println("send:1");
    delay(2000);
  }if (blue_send_limit == 0){
    Serial.println("Stop sending...");
    delay(10000);
    blue_send_limit = 5;
  }
  

  delay(10); // 適切なディレイを追加するか調整してください

  
}

