#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "XPT2046.h"
#include "UI_Lib_Test_Touch_Demo.h"

#define TFT_DC 9
#define TFT_CS 10
#define TOUCH_CS_PIN 7
#define TOUCH_IRQ_PIN 6

#define DISPLAY_WIDTH  320
#define DISPLAY_HEIGHT 240

/*
A: 0.0916671
B: -0.0004169
C: -18.9931370
D: 0.0001501
E: 0.0678279
F: -22.0341150
W: 320
H: 240
R: 1
*/
XPT2046::Calibration CALIBRATION = {0.0916671,-0.0004169,-18.9931370,0.0001501,0.0678279,-22.0341150,320,240,1};

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);    // Use hardware SPI
XPT2046 ts(TOUCH_CS_PIN, TOUCH_IRQ_PIN);

void setup()
{
    Serial.begin(115200);
    Serial.println("ILI9341 Test!"); 

    tft.begin();
    tft.setRotation(1);

    ts.begin();
    ts.setSampleCount(1);
    ts.setDebounceTimeout(10);
    ts.setCalibration(CALIBRATION);
    ts.setPowerDown(false);
    ts.setRotation(tft.getRotation());

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
    static XPT2046::Point touchedPoint;

    bool isTouched = ts.touched();
    if(isTouched || touchEventState != TOUCH_EVENTS_WAIT_FOR_TOUCH)
	{
        switch (touchEventState)
        {
            case TOUCH_EVENTS_WAIT_FOR_TOUCH:
                if(isTouched)
                {
                    touchStartTime = millis();
                    touchedPoint = ts.getTouchPosition();
                    touchEventState = TOUCH_EVENTS_WAIT_LONG_TOUCH_DELAY;
                }
                break;
            case TOUCH_EVENTS_WAIT_LONG_TOUCH_DELAY:
                if(!isTouched)
                {
                    // Normal touch
                    Serial.println("Normal: x=" + String(touchedPoint.x, 10) + " y=" + String(touchedPoint.y, 10));
                    UI_Test_TouchInput(touchedPoint.x, touchedPoint.y, TOUCH_NORMAL);
                    touchEventState = TOUCH_EVENTS_WAIT_FOR_TOUCH;
                }
                else if(millis() - touchStartTime >= LONG_TOUCH_DELAY_MS)
                {
                    // Long touch
                    Serial.println("Long: x=" + String(touchedPoint.x, 10) + " y=" + String(touchedPoint.y, 10));
                    UI_Test_TouchInput(touchedPoint.x, touchedPoint.y, TOUCH_LONG);
                    touchEventState = TOUCH_EVENTS_LONG_TOUCH_DETECTED;
                }
                break;
            case TOUCH_EVENTS_LONG_TOUCH_DETECTED:
                if(!isTouched)
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