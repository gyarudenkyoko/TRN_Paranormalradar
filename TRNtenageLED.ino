//①FastLEDライブラリ入ってない人はarduinoにライブラリをインストールする
//https://github.com/FastLED/FastLED
//②ボードマネージャからESP32をインストールする
//ファイル→環境設定→追加のボードマネージャのURLに下記をコピペ（ほかにボードマネージャが設定されてる場合は;で区切って追加
//https://dl.espressif.com/dl/package_esp32_index.json/
//上の2個がわかんない場合は「arduino　インストール」「FastLED ライブラリ インストール」「ESP32　ボードマネージャ　設定」でググりな

//ESPr® Developer 32 https://www.switch-science.com/catalog/3210/
//ESPr® Developer 32[GND]⇔WS2812B[GND]
//ESPr® Developer 32[27]⇔WS2812B[DIN]
//ESPr® Developer 32[3v3]⇔WS2812B[5V]
//書きこむときはPCとボードつないで「ツール」→「ボード」→「ESP32 Dev Module」を選ぶ

#include <FastLED.h>
#define NUM_LEDS 15// LEDテープのLEDの粒の数を設定する
#define DATA_PIN 27// LEDテープのDINピンと接続しているArduinoのピン
CRGB leds[NUM_LEDS]; // LEDテープの配列初期設定用（変更しない）

int val = 0;//ホールセンサーの値を入れる
int count =0;//乱数を発生した回数
int trn = 0;//esp_randamで生成した乱数

void setup() {
  Serial.begin(9600); //シリアル通信速度　シリアルモニタを「9600」に設定しないと文字化けするよ。速度は適当に設定したよ
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); //LEDテープを使用するための初期設定
    }

void loop() {
  //ホールセンサーの値とハードウェアから乱数を生成してシリアルモニタに表示する。
  //ハードウェアから生成した乱数をもとに0または1をランダムに生成する 
  //10回繰り返し、1が出たらcountに足していく→10回0と1をランダムに出して1が出た数をカウントする
  
    for (int i = 0; i < 10; ++i){
      val = hallRead();
      Serial.print("hallsensor = ");
      Serial.println(val);//to graph 
      Serial.print("TRN = ");
      Serial.println(esp_random());
      trn = random(0,2);
      Serial.println(trn);
      //countsum = trn + countsum;
        if (trn == 1){
          count = count +1;
        }
      delay(70);
  }
  //10回0と1をランダムに生成して1が何個出たかシリアルモニタに表示
  Serial.print("count = ");
  Serial.println(count);//to graph
  
  //1が0回出た場合は赤でLEDを光らせる
  if(count == 0){
  FadeInOut(0xff, 0x00, 0x00); // Red
  FadeInOut(0xff, 0x00, 0x00); // Red
  }
  //1が1回出た場合は黄色でLEDを光らせる
  if(count ==1){
   FadeInOut(0xff, 0xff, 0x00); // Yellow
   FadeInOut(0xff, 0xff, 0x00); // Yellow
  }
  //1が2回か3回出た場合はホットピンクでLEDを光らせる
  if((count >= 2)&&(count <= 3)){
   FadeInOut(0xff, 0x69, 0xB4); // Hotpink
   FadeInOut(0xff, 0x69, 0xB4); // Hotpink
  }
  //1が4回出た場合はホットピンクでLEDを光らせる
  if(count == 4){
   FadeInOut(0x9a, 0xcd, 0x32); // GreenYellow
   FadeInOut(0x9a, 0xcd, 0x32); // GreenYellow
  }
   //1が5回出た場合は白でLEDを光らせる
  if(count == 5){
    FadeInOut(0xff, 0xff, 0xff); // white
    FadeInOut(0xff, 0xff, 0xff); // white
  } 
  //1が6回出た場合はダークターコイズでLEDを光らせる
  if(count == 6){
   FadeInOut(0x00, 0xce, 0xd1); // DarkTurquoise
   FadeInOut(0x00, 0xce, 0xd1); // DarkTurquoise
  }
  //1が7回か8回出た場合はホットピンクでLEDを光らせる
  if((count >= 7)&&(count <= 8)){
   FadeInOut(0xff, 0x69, 0xB4); // Hotpink
   FadeInOut(0xff, 0x69, 0xB4); // Hotpink
  }
  //1が9回出た場合は黄色でLEDを光らせる
  if(count == 9){
   FadeInOut(0xff, 0xff, 0x00); // Yellow
   FadeInOut(0xff, 0xff, 0x00); // Yellow
  }
  //1が10回出た場合は赤色でLEDを光らせる
  if(count == 10){
    FadeInOut(0xff, 0x00, 0x00); // Red
    FadeInOut(0xff, 0x00, 0x00); // Red
  }  
  count = 0; //１のカウントを0に戻す
  delay(150);
  
}

void FadeInOut(byte red, byte green, byte blue){
  float r, g, b;
      
  for(int k = 0; k < 256; k=k+1) { 
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }
     
  for(int k = 255; k >= 0; k=k-2) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }
}
void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H 
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue); 
  }
  showStrip();
}
