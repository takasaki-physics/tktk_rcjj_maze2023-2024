#include <sensor/move_2024_2025.h>

void move_2024_2025::tiziki_kaitenn()
{
  Mpu.getYawPitchRoll();


  kakudo2 = Mpu.yaw;
  kakudo_true2 = int(kakudo2);
  Serial.println(kakudo_true2);
}

//turn_hosei
void move_2024_2025::tiziki()
{
  Mpu.getYawPitchRoll();
  kakudo =Mpu.yaw;
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

void move_2024_2025::tiziki_2(){
  Mpu.getYawPitchRoll();
  katamuki = Mpu.pitch;
  katamuki_true = int(katamuki);
  Serial.println(katamuki_true); 
  
}



void move_2024_2025::motor_setup() {
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
void move_2024_2025::migi(){
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
void move_2024_2025::hidari(){
  
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
void move_2024_2025::susumu_heitan() {
    Serial3.flush();
  while (count2  <susumu_kaisuu) {
    Position[0] = 576; //go
    Position[1] = -576;
    Position[2] = -576;
    Position[3] = 576;
    sms_sts.SyncWritePosEx(ID, 4, Position, Speed, ACC);
    //serialEvent3();_color_or_loadcell
    //serialEvent1();_victim_camera1
    //serialEvent2();_victim_camera2
    count2++;
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
    BlackTile=1;
    }
  
  delay(300);

  }else {
    
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
            Slope = 1;
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
            Slope = 1;
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



void move_colororbump(){
    }
//collect_tof_data
void serialEvent(){
    Serial3.flush();
    while (Serial3.available()== 0){
        delay(100);
        }
    if (Serial3.available() >0){
        int receivedData = Serial3.read();
        //get_data_json
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
    int receivedData = Serial1.read();
    move_colororbump();
    }
}






