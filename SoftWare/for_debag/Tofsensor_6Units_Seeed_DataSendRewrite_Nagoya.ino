#include <Wire.h>
#include <VL53L0X.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <stdio.h>

VL53L0X sensor1, sensor2, sensor3, sensor4, sensor5, sensor6;

// 壁判定の閾値 (単位: mm)
//const int WALL_THRESHOLD = 170;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define HEADER 255

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int RightWeight = 0;
int LeftWeight = 0;
int FrontWeight = 0;
int x = 0;
int y = 0;
int Vector = 0;
int GoHomeStatus = 0;

bool headerDetected = false;
byte receivedData[7];
int receivedIndex = 0;

// I2Cバスの使用状態を管理するフラグ
volatile bool isI2CInUse = false;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Wire.begin();
  delay(2000);
  Serial.println("Start");

  // ピンの初期化
  pinMode(28, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(29, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(27, LOW);
  digitalWrite(28, LOW);
  digitalWrite(29, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(2, LOW);
  delay(10);

  // センサーの初期化
  initializeSensor(sensor1, 27, 0x30);
  initializeSensor(sensor2, 28, 0x31);
  initializeSensor(sensor3, 29, 0x32);
  initializeSensor(sensor4, 3, 0x33);
  initializeSensor(sensor5, 4, 0x34);
  initializeSensor(sensor6, 2, 0x35);
}
/*........Tofセンサーの値によって壁との距離を測定しメインマイコンに送信する...........*/
void loop() {
  while (isI2CInUse) {
    delayMicroseconds(100); // I2Cが使用中なら待機
  }

  isI2CInUse = true; // I2C使用開始

  // 各センサーの距離を取得し、250を超えたら250に固定
  byte sensor_data[6];
  sensor_data[0] = min(sensor1.readRangeSingleMillimeters(), 250);
  sensor_data[1] = min(sensor2.readRangeSingleMillimeters(), 250);
  sensor_data[2] = min(sensor3.readRangeSingleMillimeters(), 250);
  sensor_data[3] = min(sensor4.readRangeSingleMillimeters(), 250);
  sensor_data[4] = min(sensor5.readRangeSingleMillimeters(), 250);
  sensor_data[5] = min(sensor6.readRangeSingleMillimeters(), 250);

  Serial1.write(255);
  Serial.println("Sent Header");
  delay(50); // wait
  
  // 判定データ送信 (Serial1 には 2 進数で送信)
  for (int i = 0; i < 6; i++) {
    Serial1.write(sensor_data[i]);
  }
  
  Serial.print("Sensor Data: "); // デバッグ用にシリアルモニタに送信 (10 進数 )
  for (int i = 0; i < 6; i++) {
    Serial.print(sensor_data[i], DEC);
    if (i < 5) Serial.print(", ");
  }
  Serial.println();

  isI2CInUse = false; // I2C使用終了
  delay(500);
}

void initializeSensor(VL53L0X &sensor, int pin, int address) {
  digitalWrite(pin, HIGH);
  delay(100);
  sensor.setTimeout(500);
  if (!sensor.init()) {
    Serial.print("Failed to detect and initialize sensor at pin ");
    Serial.println(pin);
    while (1);
  }
  sensor.setMeasurementTimingBudget(20000);
  sensor.setAddress(address);
}

void updateDisplay() {
    while (isI2CInUse) {
    delayMicroseconds(100); // I2Cが使用中なら待機
  }
  isI2CInUse = true;
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Go home: " + String(GoHomeStatus));
  display.println("Right: " + String(RightWeight));
  display.println("Left: " + String(LeftWeight));
  display.println("Front: " + String(FrontWeight));
  display.print("X: " + String(x) + " Y: " + String(y));
  display.println(" Vector: " + String(Vector));
  display.display();

  Serial.print("Received data: ");
  for (int i = 0; i < 7; i++) {
    Serial.print(receivedData[i], DEC);
    if (i < 6) Serial.print(", ");
  }
  Serial.println();
  isI2CInUse = false;
}

void getSerialData() {
  while (Serial1.available() > 0) {
    byte incomingByte = Serial1.read();
    if (!headerDetected) {
      if (incomingByte == HEADER) {
        headerDetected = true;
        receivedIndex = 0;
        Serial.println("Header received!");
      }
    } else {
      receivedData[receivedIndex++] = incomingByte;
      if (receivedIndex == 7) {
        headerDetected = false;
        GoHomeStatus = receivedData[0];
        RightWeight = receivedData[1];
        LeftWeight = receivedData[2];
        FrontWeight = receivedData[3];
        x = receivedData[4];
        y = receivedData[5];
        Vector = receivedData[6];
        updateDisplay();
        while (Serial1.available() > 0) {
          Serial1.read();
        }
      }
    }
  }
}

void setup1() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Wire.begin();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  updateDisplay();
}
/*........メインマイコンから送信されたデータをもとにディスプレイに座標と座標の重みを表示する...........*/
void loop1() {
  while (isI2CInUse) {
    delayMicroseconds(100); // I2Cが使用中なら待機
  }

  getSerialData();
}
