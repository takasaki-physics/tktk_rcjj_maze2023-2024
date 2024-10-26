#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLEDディスプレイの幅と高さを定義
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// リセットピンは使わない (-1 で無効化)
#define OLED_RESET -1

// I2Cアドレス (一般的には 0x3C または 0x3D)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // シリアル通信の開始（デバッグ用）
  Serial.begin(9600);
  
  // ディスプレイの初期化
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // I2Cアドレスが0x3Cの場合
    Serial.println(F("SSD1306ディスプレイの初期化に失敗しました"));
    for (;;);  // 初期化失敗時に停止
  }

  // ディスプレイの初期設定
  display.clearDisplay();            // ディスプレイの内容をクリア
  display.setTextSize(1);            // 文字のサイズを設定
  display.setTextColor(WHITE);       // 文字色を白に設定
  display.setCursor(0, 0);           // カーソルをディスプレイの左上にセット

  // 表示するテキスト
  display.println("Hello World!");
  display.display();                 // ディスプレイに表示を反映
}

void loop() {
  // ここには特に動作はないため、ディスプレイに文字を表示するだけ
}
