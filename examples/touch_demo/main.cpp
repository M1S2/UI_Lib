#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <TS_Display.h>
#include "UI_Lib_Test_Touch_Demo.h"

#define TFT_DC 9
#define TFT_CS 10
#define TOUCH_CS_PIN 7
#define TOUCH_IRQ_PIN 6

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);    // Use hardware SPI
XPT2046_Touchscreen ts(TOUCH_CS_PIN);
TS_Display ts_display;

void setup()
{
    Serial.begin(9600);
    Serial.println("ILI9341 Test!"); 
 
    pinMode(TOUCH_IRQ_PIN, INPUT);

    tft.begin();
    tft.setRotation(1);

    ts.begin();
    ts.setRotation(tft.getRotation());

    ts_display.begin(&ts, &tft);
    /*
    TS_LR_X: 440  TS_LR_Y: 380  TS_UL_X: 3904  TS_UL_Y: 3792
    UL corner (0, 0) maps to touchscreen (3904, 3792)
    LR corner (319, 239) maps to touchscreen (451, 395)
    */
    ts_display.setTS_calibration(440, 380, 3904, 3792);

    UI_Test_Init(&tft);
    UI_Test_BuildTree();

    UI_Test_Draw();
}

#define LONG_TOUCH_DELAY_MS     750
unsigned long touchStartTime = 0;

enum TouchEventStates
{
  TOUCH_EVENTS_WAIT_FOR_TOUCH,
  TOUCH_EVENTS_WAIT_LONG_TOUCH_DELAY,
  TOUCH_EVENTS_LONG_TOUCH_DETECTED
};
TouchEventStates touchEventState = TOUCH_EVENTS_WAIT_FOR_TOUCH;

void touch_handling()
{
    if(!digitalRead(TOUCH_IRQ_PIN) || touchEventState != TOUCH_EVENTS_WAIT_FOR_TOUCH)
	{
        int16_t x, y, pres, px, py;
        eTouchEvent touchEvent = ts_display.getTouchEvent(x, y, pres, &px, &py);

        switch (touchEventState)
        {
            case TOUCH_EVENTS_WAIT_FOR_TOUCH:
                if(touchEvent == TS_TOUCH_EVENT || touchEvent == TS_TOUCH_PRESENT)
                {
                    touchStartTime = millis();
                    touchEventState = TOUCH_EVENTS_WAIT_LONG_TOUCH_DELAY;
                }
                break;
            case TOUCH_EVENTS_WAIT_LONG_TOUCH_DELAY:
                if(touchEvent == TS_RELEASE_EVENT || touchEvent == TS_NO_TOUCH)
                {
                    // Normal touch
                    UI_Test_TouchInput(x, y, TOUCH_NORMAL);
                    touchEventState = TOUCH_EVENTS_WAIT_FOR_TOUCH;
                }
                else if(millis() - touchStartTime >= LONG_TOUCH_DELAY_MS)
                {
                    // Long touch
                    UI_Test_TouchInput(x, y, TOUCH_LONG);
                    touchEventState = TOUCH_EVENTS_LONG_TOUCH_DETECTED;
                }
                break;
            case TOUCH_EVENTS_LONG_TOUCH_DETECTED:
                if(touchEvent == TS_RELEASE_EVENT || touchEvent == TS_NO_TOUCH)
                {
                    touchEventState = TOUCH_EVENTS_WAIT_FOR_TOUCH;
                }
                break;
            default: break;
        }
    }
}

void key_handling()
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

void loop()
{
    touch_handling();
    key_handling();

    UI_Test_Draw();
}