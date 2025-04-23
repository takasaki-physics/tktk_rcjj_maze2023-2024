#include <ArduinoJson.h>


/*...................................................................................*/
//吉ノ薗のアルゴリズムのなかのmove_2024_2025.hにある壁情報の条件
bool right_wall = false;//ここもセンサーの値を取得するファイルとして分けたい
bool front_wall = false;
bool left_wall = false;
/*..................................................................................*/
void setup() {
  Serial.begin(115200); // シリアル通信開始
  Serial1.begin(19200);
  while (!Serial) {
    // シリアルポートが接続されるのを待つ（必要に応じて削除）
  }
  Serial.println("Ready to receive JSON data.");
}

void loop() {
  static char jsonBuffer[256];
  static size_t index = 0;

  // シリアルデータの読み取り
  while (Serial1.available() > 0) {
    char incomingChar = Serial1.read();

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
            else [
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
            ]        
          }
          Serial.println(); // データ区切り
        }
        index = 0; // バッファをリセット
      }
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
}
