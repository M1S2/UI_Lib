/*
 * UI_Lib_Test.h
 *
 */ 

#ifndef UI_LIB_TEST_H_
#define UI_LIB_TEST_H_

#include "Core/UI_Manager.h"
#include "Core/UI_Elements.h"

void UI_Test_Init(Adafruit_GFX* gfx);
void UI_Test_BuildTree();
void UI_Test_Draw(Adafruit_GFX* gfx, bool isFirstPage);
void UI_Test_KeyInput(Keys_t key);


/**** UI Icon Speed *****/
#define ui_icon_speed_width 16
#define ui_icon_speed_height 16
static const unsigned char ui_icon_speed_bits[] PROGMEM = {
	0xC0, 0x03, 0x30, 0x0C, 0x4C, 0x32, 0x04, 0x20, 0x0A, 0x58, 0x02, 0x44,
	0x05, 0xA2, 0x01, 0x81, 0x81, 0x80, 0x05, 0xA0, 0x02, 0x40, 0x02, 0x40,
	0x04, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };

#endif /* UI_LIB_TEST_H_ */