const byte HEADER = 255;  // ヘッダー (送信側と一致すること)
bool headerDetected = false;
byte receivedData[6];  // 受信データ格納用
int receivedIndex = 0; // データ格納位置
int Status = 0;
void setup() {
  Serial.begin(9600);   // デバッグ用
  Serial1.begin(9600);  // Serial1の初期化
  Serial.println("Waiting for data...");
}
void get_tof_data() {
    while (Serial1.available() > 0) {
      byte incomingByte = Serial1.read(); // Serial1から1バイト読み取り
      if (!headerDetected) {
        if (incomingByte == HEADER) {
          headerDetected = true;  // ヘッダー検出
          receivedIndex = 0;      // データ格納位置をリセット
          Serial.println("Header detected!"); // デバッグ用
        }
      } else {
        receivedData[receivedIndex++] = incomingByte; // データを格納
        if (receivedIndex == 6) {
          // 4バイト受信完了
          headerDetected = false;
          // 受信したデータを表示
        for (int i = 0; i < receivedIndex; i++) {
            Serial.print("Sensor ");
            Serial.print(i);
            Serial.print(": ");
            Serial.println(receivedData[i], BIN); // 受信データを2進数で表示
        }
        Serial.println("All data received");
        Status = 2;
   
        }
      }
    }
}
void susumu_heitann(){
    Serial.println("susumu");
    delay(5000);
    Serial1.clear();
    delay(100);
    Status = 0;
}
void loop() {
  if (Status == 0){
    get_tof_data();
  }else if (Status == 2){
    susumu_heitann();
  }
}