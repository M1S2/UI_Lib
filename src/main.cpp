#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "UI_Lib_Test.h"

#ifdef DEBUG
#include "avr8-stub.h"
#endif

#define TFT_DC 9
#define TFT_CS 10

// Use hardware SPI
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

//#define TFT_MOSI  51
//#define TFT_CLK   52
//#define TFT_RST   8
//#define TFT_MISO  -1
// Use software SPI (slower!)
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

void setup()
{
  #ifdef DEBUG
    debug_init();
  #endif

  Serial.begin(9600);
  Serial.println("ILI9341 Test!"); 
 
  tft.begin();
  tft.setRotation(1);

  UI_Test_Init(&tft);
  UI_Test_BuildTree();

  UI_Test_Draw();
}

void loop()
{
  // check for incoming serial data:
  if (Serial.available() > 0)
  {
    // read incoming serial data:
    char inChar = Serial.read();

    bool processed = true;
    switch (inChar)
    {
      case '0': UI_Test_KeyInput(KEY0); break;
      case '1': UI_Test_KeyInput(KEY1); break;
      case '2': UI_Test_KeyInput(KEY2); break;
      case '3': UI_Test_KeyInput(KEY3); break;
      case '4': UI_Test_KeyInput(KEY4); break;
      case '5': UI_Test_KeyInput(KEY5); break;
      case '6': UI_Test_KeyInput(KEY6); break;
      case '7': UI_Test_KeyInput(KEY7); break;
      case '8': UI_Test_KeyInput(KEY8); break;
      case '9': UI_Test_KeyInput(KEY9); break;
      case 'l': UI_Test_KeyInput(KEYLEFT); break;
      case 'r': UI_Test_KeyInput(KEYRIGHT); break;
      case 'u': UI_Test_KeyInput(KEYUP); break;
      case 'd': UI_Test_KeyInput(KEYDOWN); break;
      case 'o': UI_Test_KeyInput(KEYOK); break;
      case 'm': UI_Test_KeyInput(KEYMILLI); break;
      case 'k': UI_Test_KeyInput(KEYKILO); break;
      case '-': UI_Test_KeyInput(KEYMINUS); break;
      case 'x': UI_Test_KeyInput(KEYX1); break;
      default: processed = false; break;
    }

    if(processed == true)
    {
      UI_Test_Draw();
    }
  }
}