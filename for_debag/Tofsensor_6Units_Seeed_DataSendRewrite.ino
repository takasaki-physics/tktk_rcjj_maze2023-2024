#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor1, sensor2, sensor3, sensor4, sensor5, sensor6;

// 壁判定の閾値 (単位: mm)
const int WALL_THRESHOLD = 130;

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600); // Use Serial2 for JSON output
  Wire.begin();

  // Control pins setup
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

  // Initialize sensors with unique addresses
  initializeSensor(sensor1, 27, 0x30);
  initializeSensor(sensor2, 28, 0x31);
  initializeSensor(sensor3, 29, 0x32);
  initializeSensor(sensor4, 3, 0x33);
  initializeSensor(sensor5, 4, 0x34);
  initializeSensor(sensor6, 2, 0x35);
}

void loop() {
  // 各センサーの距離を取得し壁の有無を判定
  byte sensor_data[6];
  sensor_data[0] = (sensor1.readRangeSingleMillimeters() < WALL_THRESHOLD) ? 0x01 : 0x00;
  sensor_data[1] = (sensor2.readRangeSingleMillimeters() < WALL_THRESHOLD) ? 0x01 : 0x00;
  sensor_data[2] = (sensor3.readRangeSingleMillimeters() < WALL_THRESHOLD) ? 0x01 : 0x00;
  sensor_data[3] = (sensor4.readRangeSingleMillimeters() < WALL_THRESHOLD) ? 0x01 : 0x00;
  sensor_data[4] = (sensor5.readRangeSingleMillimeters() < WALL_THRESHOLD) ? 0x01 : 0x00;
  sensor_data[5] = (sensor6.readRangeSingleMillimeters() < WALL_THRESHOLD) ? 0x01 : 0x00;

  Serial1.write(255);
  Serial.println("Sent header: 255");
  delay(50);//wait
  // 判定データ送信
  for (int i = 0; i < 6; i++) {
    Serial1.write(sensor_data[i]);
  }
    
  

    Serial.print("Sensor Data: ");//デバック用にシリアルモニタに送信
    for (int i = 0; i < 6; i++) {
      Serial.print(sensor_data[i], BIN);
      if (i < 5) Serial.print(", ");
    }
    Serial.println();


  delay(500); // 必要に応じて調整
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
