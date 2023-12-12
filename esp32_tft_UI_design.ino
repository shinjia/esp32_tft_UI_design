/* esp32_tft_UI_design  by Shinjia
  - v0.2  2023/12/12
  - v0.1  2023/07/25
*/

#include "graphics_0.h"
#include "graphics_1.h"
#include "graphics_2.h"
#include "graphics_3.h"
#include "graphics_4.h"
#include "graphics_5.h"
#include "graphics_6.h"
#include "graphics_gauge.h"

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_DC   21
#define TFT_CS   17
#define TFT_MOSI 23
#define TFT_CLK  18
#define TFT_RST  22
#define TFT_MISO 19

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

int value1;
float value2;


void show_picture(int idx) {
    switch(idx) {
      case 0: 
        tft.drawRGBBitmap( 0,  0, (const uint16_t *)bitmap_AQI_0, 134, 240); 
        break;
      case 1: 
        tft.drawRGBBitmap( 0,  0, (const uint16_t *)bitmap_AQI_1, 134, 240); 
        break;
      case 2: 
        tft.drawRGBBitmap( 0,  0, (const uint16_t *)bitmap_AQI_2, 134, 240); 
        break;
      case 3: 
        tft.drawRGBBitmap( 0,  0, (const uint16_t *)bitmap_AQI_3, 134, 240); 
        break;
      case 4: 
        tft.drawRGBBitmap( 0,  0, (const uint16_t *)bitmap_AQI_4, 134, 240); 
        break;
      case 5: 
        tft.drawRGBBitmap( 0,  0, (const uint16_t *)bitmap_AQI_5, 134, 240); 
        break;
      case 6: 
        tft.drawRGBBitmap( 0,  0, (const uint16_t *)bitmap_AQI_6, 134, 240); 
        break;
    }
}


void show_flag(int idx) {
  uint16_t color;
  switch(idx) {
    case 0:
      color = tft.color565(255, 255, 255);
      break;
    case 1: 
      color = tft.color565(0, 153, 68);
      break;
    case 2: 
      color = tft.color565(255, 241, 0);
      break;
    case 3: 
      color = tft.color565(243, 152, 0);
      break;
    case 4: 
      color = tft.color565(230, 0, 18);
      break;
    case 5: 
      color = tft.color565(126, 27, 118);
      break;
    case 6: 
      color = tft.color565(136, 28, 33);
      break;
  }

  // flags
  tft.fillTriangle(145, 14, 145, 64, 300, 37, color );
  tft.fillRect(140, 10, 8, 230, ILI9341_LIGHTGREY );
  tft.drawFastVLine(142, 10, 230, ILI9341_DARKGREY );
  tft.fillCircle(144, 8, 8,  ILI9341_OLIVE);
  tft.drawCircle(144, 8, 8,  ILI9341_DARKGREY);
}


void show_message(int val) {
  // Draw title
  
  // 以軸心的位置為基準
  int base_x = 235;
  int base_y = 140;
  int pointer_len = 60;
  
  int theta = map(val, 0, 300, -180, 0);
  int end_x = base_x + pointer_len * cos(theta*PI/180);
  int end_y = base_y + pointer_len * sin(theta*PI/180);

  int tri_x1 = base_x + 5 * cos((theta+90)*PI/180);
  int tri_y1 = base_y + 5 * sin((theta+90)*PI/180);
  int tri_x2 = base_x + 5 * cos((theta-90)*PI/180);
  int tri_y2 = base_y + 5 * sin((theta-90)*PI/180);
  
  // draw gauge
  tft.drawRGBBitmap(base_x-70, base_y-72+3, (const uint16_t *)bitmap_gauge, 140, 70); 
  for(int i=1; i<7; i++) {
    tft.drawFastHLine(base_x-70, base_y+i, 140, ILI9341_BLACK);
  }

  // draw Pointer (Triangle)
  // tft.drawLine(base_x, base_y, end_x, end_y, ILI9341_WHITE);
  tft.fillTriangle(end_x, end_y, tri_x1, tri_y1, tri_x2, tri_y2, ILI9341_CYAN);

  // center circle
  tft.fillCircle(base_x, base_y, 6, ILI9341_WHITE);
  tft.fillCircle(base_x, base_y, 5, ILI9341_BLACK);

  // other
  tft.drawFastHLine(base_x-70, base_y+40, 140, ILI9341_CYAN);
  tft.drawFastHLine(base_x-70, base_y+42, 140, ILI9341_BLUE);

  // show value
  tft.setTextSize(3);

  // Show string
  String str = String(" ") + val + String(" ");
  int16_t get_x, get_y;
  uint16_t get_w, get_h;
  
  tft.getTextBounds(str.c_str(), 100, 100, &get_x, &get_y, &get_w, &get_h);

  tft.setCursor(base_x-get_w/2+1, base_y+14);  
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.print(str);  
}


void show_clock() {
  tft.setTextSize(2);

  // Show string
  tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  tft.setCursor(176, 200);  
  tft.print(" 00:00:00 ");
  tft.setCursor(176, 220);  
  tft.print("2000/88/88");
}


void test_all_bitmap() {
  for(int i=0; i<7; i++) {
    show_message(50*i);
    show_flag(i);
    show_picture(i);
    show_clock();
    delay(2000);
  }
}


void setup() {
  Serial.begin(115200);
  Serial.println("ILI9341 Start!"); 
 
  tft.begin();
  
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);  // Clear

  test_all_bitmap();
}


void loop(void) {
  value1 = random(0, 300);
  value2 = (float)random(100, 999) / 10;

  int aqi = constrain(value1, 0, 300);
  int degree = ((aqi-1)/50) + 1;
  degree = constrain(degree, 1, 6);

  show_message(aqi);
  show_flag(degree);
  show_picture(degree);
  show_clock();
  
  delay(10000);
}
