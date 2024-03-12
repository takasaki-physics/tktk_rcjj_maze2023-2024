// Arduino入門編⑪ 超音波センサーHC-SR04で距離を測定する！
// https://burariweb.info

#define trigPin 8  // トリガーピンをD8に
#define echoPin 9  // エコーピンをD9に

float Duration = 0;  // 計測した時間
float Distance = 0;  // 距離


void setup(){
  
  Serial.begin(9600);  // シリアルモニタの開始
  
  pinMode(echoPin,INPUT);   // エコーピンを入力に
  pinMode(trigPin,OUTPUT);  // トリガーピンを出力に

}


void loop(){
  
  digitalWrite(trigPin,LOW);              // 計測前に一旦トリガーピンをLowに
  delayMicroseconds(2);

  digitalWrite(trigPin,HIGH);             // トリガーピンを10μsの時間だけHighに
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);

  Duration = pulseIn(echoPin,HIGH);      // エコーピンからの入力
  
  Duration = Duration / 2;               // 距離を1/2に
  Distance = Duration*340*100/1000000;   // 音速を掛けて単位をcmに



  if(Distance >= 400 || Distance <= 2){  // センサーの測定範囲外の処理
    
    Serial.println("距離 = 測定範囲外");

  }
  
  else{
    Serial.print("距離 ");               // シリアルモニタに距離を表示させる
    Serial.print(Distance);
    Serial.println(" cm");

  }

     delay(100);
      
}