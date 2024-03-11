#include <stdio.h>
#include <math.h>
#include <M5Core2.h>
#include <VL53L0X.h>
#include <stdlib.h>
#include <time.h>
#include <SparkFun_I2C_Mux_Arduino_Library.h> // http://librarymanager/All#SparkFun_I2C_Mux https://github.com/sparkfun/SparkFun_I2C_Mux_Arduino_Library
#define NUMBER_OF_SENSORS 5
QWIICMUX i2cMux;
VL53L0X **sensor;
HardwareSerial myserial(2);
int val =0;
int x = 45;
int y = 45;
int i = 1;//0=done,1=n,2=e,3=w,4=s,
bool frontwall = 0;
bool rightwall = 0;
bool leftwall = 0;
bool zahyou[90][90];
bool Cancel = 0;
bool Stop = 0;
bool o = 0;
String  n = "0";
String e = "0";
String w = "0";
int count = 2;
int distance4;
int distance_differ;
int distance_migi;
int distance_mae;
int distance_hidari;
int ran = 0;
bool bump =0;

bool right_tile(){
    if (i == 1) {
        return zahyou[x+1][y];
    } else if (i == 2) {
        return zahyou[x][y+1];
    } else if (i == 3) {
        return zahyou[x][y-1];
    } else if (i == 4) {
        return zahyou[x-1][y];
    }
}

bool left_tile(){
    if (i == 1) {
        return zahyou[x-1][y];
    } else if (i == 2) {
        return zahyou[x][y-1];
    } else if (i == 3) {
        return zahyou[x][y+1];
    } else if (i == 4) {
        return zahyou[x+1][y];
    }
}

bool front_tile(){
    if (i == 1) {
        return zahyou[x][y-1];
    } else if (i == 2) {
        return zahyou[x+1][y];
    } else if (i == 3) {
        return zahyou[x-1][y];
    } else if (i == 4) {
        return zahyou[x][y+1];
    }
}
void migi(){
  myserial.write('1');
  Serial.println("migi");
  if (i==1){
    i=2;
    }else if (i==2){
      i=4;
      }else if (i==3){
        i=1;
        }else if (i==4){
          i=3;
          }
  delay(1884); //1884 ~   b ?@(1.884 b)
  delay(600);
}
void hidari(){
  myserial.write('2');
  Serial.println("hidari");
  if (i==1){
    i=3;
    }else if (i==2){
      i=1;
      }else if (i==3){
        i=4;
        }else if (i==4){
          i=2;
          }
  delay(1884); //1884 ~   b ?@(1.884 b)
  delay(600);
}
void susumu_heitan(){
  myserial.write('3');
  Serial.println("susumu_heitan");
  delay(1000);
  //1884 ~   b ?@(1.884 b)
  //sekigaisenn();
  //sendIntData(distance_differ);
  delay(50);
  myserial.flush();
  if (i==1){
    y=y-1;
    }else if (i==2){
      x=x+1;
      }else if (i==3){
        x=x-1;
        }else if (i==4){
          y=y+1;
          }
  }
void sekigaisenn_setup()
{
  sensor = new VL53L0X *[NUMBER_OF_SENSORS];
  if (i2cMux.begin(0x70, Wire) == false)
  {
    Serial.println("Mux not detected. Freezing...");
    while (1)
      ;
  }
  Serial.println("Mux detected");
  Serial.println("Begin scanning for I2C devices");
  for (uint8_t channel = 0; channel < NUMBER_OF_SENSORS; channel++) {
      i2cMux.setPort(channel);
      sensor[channel] = new VL53L0X();
      sensor[channel]->setAddress(0x29);
      sensor[channel]->setTimeout(500);
      if (!sensor[channel]->init()) {
        Serial.println("Failed to detect and initialize sensor!");
        //while (1) {}
      }
    }
}
void sekigaisenn()
{
  for (uint8_t channel = 0; channel < NUMBER_OF_SENSORS; channel++) {
    int *distance = new int[NUMBER_OF_SENSORS];
    Serial.printf("CH%d : ", channel);
    i2cMux.setPort(channel);
    Serial.print("I2C device = ");
    for (uint8_t address = 0x01; address < 0x7F; address++) {
      Wire.beginTransmission(address);
      uint8_t returnCode = Wire.endTransmission();
      if (returnCode == 0 && address != 0x70) {
        Serial.printf("0X%X ", address);
        i2cMux.setPort(channel);                               //Tell mux to connect to this port, and this port only
        distance[channel] = sensor[channel]->readRangeSingleMillimeters(); //Get the result of the measurement from the sensor
        Serial.print("\tDistance");
        Serial.print(channel);
        Serial.print("(mm): ");
        Serial.print(distance[channel]);
        if (sensor[channel]->timeoutOccurred()) {
          Serial.print(" TIMEOUT");
          }
        Serial.println();
        if (channel == 1) {
          distance_hidari = distance[channel];
          if (distance[channel] <=210){
           
            Serial.println("left");
            leftwall = 1;
          }
          else {
            leftwall = 0;
          }
        }
        else if (channel == 2) {
          distance_mae = distance[channel];
          if (distance[channel] <=50 ){
            Serial.println("front_toomore");
            frontwall = 1;
            myserial.write('4');
            delay(2000);

          }else if (distance[channel] <=120 || Cancel==1 ){
            Serial.println("front");
            frontwall = 1;
          }
            else {
              frontwall = 0;
            }
        }
        
        else if (channel == 4) {
          distance_migi = distance[channel];
          if (distance[channel] <=120){
            Serial.println("right");
            rightwall = 1;
            }
          else {
            rightwall = 0;
            //myserial.write('5');
            //delay(200);
            
            }
            distance4 = distance[channel];
        }
        else if (channel == 5){
          if (rightwall == 1){
            distance_differ = distance[channel] - distance4;
            Serial.println(distance_differ);
          }
        }
        else {
        }
        delay(180);
      }
    }
    Serial.println();
  }
}
/*
void sendIntData(int value) {
  myserial.write('H'); 
  myserial.write(lowByte(value)); 
  myserial.write(highByte(value)); 
}*/
void hyouji(){
  if (frontwall == 1){
    n = "1";
    }else if(front_tile() == 1){
      n = "@";
      }else{
        n = "0";
        }
  if (rightwall == 1){
    e = "1";
    }else if(right_tile() == 1){
      e = "@";
      }else{
        e = "0";
        }
  if (leftwall == 1){
    w = "1";
    }else if(left_tile() == 1){
      w = "@";
      }else{
        w = "0";
        }
  M5.Lcd.setCursor(0, 13);
  M5.Lcd.printf("migi: %3d\n",distance_migi);
  M5.Lcd.printf("hidari: %3d\n",distance_hidari);
  M5.Lcd.printf("mae: %3d\n", distance_mae);
  M5.Lcd.printf("%3s %3s %3s\n",w,n,e);
  M5.Lcd.printf("%3d,%3d,%3d\n",x,y,i);
  M5.Lcd.printf("o: %3d\n",o);
}
void saka(){
  if (i==1){
    y=y-1;
    }else if (i==2){
      x=x+1;
      }else if (i==3){
        x=x-1;
        }else if (i==4){
          y=y+1;
          }
}
void setup() {
  delay(100);
  M5.begin();
  Serial.begin(115200);
  M5.Lcd.setTextSize(4);
  myserial.begin(9600);
  Wire.begin(32,33);
  srand((unsigned int)time(NULL));
  sekigaisenn_setup();
  for (int t = 0; t < 90; t++) {
  for (int j = 0; j < 90; j++) {
    zahyou[t][j] = 0;  //make it to begining 
    }
  }
  delay(100);
  sekigaisenn();
  hyouji();
  zahyou[45][45] = 1;
}
void loop(){
    sekigaisenn();
    hyouji();
    if ((frontwall==1 || front_tile()==1) && (rightwall==1 || right_tile()==1)){
      //make tile done
      zahyou[x][y]=1;  //done
    }

    if ((rightwall==1 || right_tile()==1) && (leftwall==1 || left_tile()==1)){
      //make tile done
      zahyou[x][y]=1;  //done
    }

    if ((leftwall==1 || left_tile()==1) && (frontwall==1 || front_tile()==1)){
      //make tile done
      zahyou[x][y]=1;  //done
    }
    if(o == 1){
      //migite or hidarite(3:2)
      ran = 1 + rand() % 10;//1~10
      if(rightwall == 0 && right_tile() == 0){
            o = 0;
            count =1;
            }else if(frontwall == 0 && front_tile() == 0){
              o = 0;
              count =1;
              }else if(leftwall == 0 && left_tile() == 0){
                o = 0;
                count =1;
                }else if(ran <= 6){
                    if(rightwall== 0){
                      migi();
                      susumu_heitan();
                      }else if(frontwall== 0){
                        susumu_heitan();
                        }else if(leftwall== 0){
                        hidari();
                        susumu_heitan();
                        }else {
                          hidari();
                          hidari();
                          susumu_heitan();
                          }
                          count++;
                }else if(ran >= 7){
                  if(leftwall== 0){
                    hidari();
                    susumu_heitan();
                    }else if(frontwall== 0){
                      susumu_heitan();
                      }else if(rightwall== 0){
                        migi();
                        susumu_heitan();
                        }else {
                          migi();
                          migi();
                          susumu_heitan();
                          }
                          count++;
                  }
    }else{
      //kakuchou migite
      if (rightwall ==0 && right_tile()==0){
        //migi
        migi();//turn right
        susumu_heitan();//move forward
        }else if (frontwall ==0 && front_tile()==0){
          //mae
          susumu_heitan();//move forward
          }else if (leftwall ==0 && left_tile()==0){
            //hidari
            zahyou[x][y]=1; 
            hidari();//turn left
            susumu_heitan();//move forward
            }else {
              //ikidumari
              zahyou[x][y] = 1; 
              hidari();
              hidari();//turn back
              //nukedasu  
              susumu_heitan();//move forward
              o = 1;//change to random process
                }
      count++;   
    }

    while (myserial.available()==0 && count ==1){
      delay(200);
    }
    if(myserial.available()>0 ){
    val = myserial.read();
    Serial.println("READinfor");
    }
    //tile kind 
    if(val == '4' ){//if black tile
      Serial.println("Cancel");
      Cancel = 1;
      //change to done
      zahyou[x][y] = 1;
      
      if(i==1){
        y = y + 1;
      }else if(i==2){
        x = x - 1;
      }else if(i==4){
        y = y - 1;
      }else if(i==3){
        x = x + 1;
      }
    }
    else if(val == '5'){//if blue tile
      Serial.println("Stop");
      Stop = 1;
    }
    else if(val == '7'){//if blue tile and slope
      Serial.println("saka&stop");
      Stop = 1;
      saka();
    }
    else if(val == '8'){//if slopes
      saka();    
      }else if(val == '9'){
        Stop = 1;
        bump = 1;
        if ( bump ==1){
             
             if(i==1){
              y = y + 1;
            }else if(i==2){
              x = x - 1;
            }else if(i==4){
              y = y - 1;
            }else if(i==3){
              x = x + 1;
            }
            bump = 0;
            }
      }else if(val == '3'){
        bump = 1;
        if ( bump ==1){
              
             
             if(i==1){
              y = y + 1;
            }else if(i==2){
              x = x - 1;
            }else if(i==4){
              y = y - 1;
            }else if(i==3){
              x = x + 1;
            }
            bump = 0;
            }
      }
    else {
      Serial.println("None");
      Cancel = 0;
      Stop = 0;
    }
    if(Stop == 1){//to stop if blue tile
      delay (5000);
     }else{
     }
    
    /*if(x == 45 && y == 45){ //return bonus
      delay(20000);
    }*/
    count++;
}