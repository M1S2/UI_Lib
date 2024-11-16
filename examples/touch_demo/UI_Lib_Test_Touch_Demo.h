/*
 * UI_Lib_Test.h
 *
 */ 

#ifndef UI_LIB_TEST_TOUCH_DEMO_H_
#define UI_LIB_TEST_TOUCH_DEMO_H_

#include "UI_Lib.h"

void UI_Test_Init(Adafruit_GFX* gfx);
void UI_Test_BuildTree();
void UI_Test_Draw();
void UI_Test_KeyInput(Keys_t key);
void UI_Test_TouchInput(uint16_t x, uint16_t y);

#endif /* UI_LIB_TEST_TOUCH_DEMO_H_ */