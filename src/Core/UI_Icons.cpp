/*
 * UI_Icons.cpp
 */ 

#include "Core/UI_Icons.h"

unsigned const char icon_info_bits[] PROGMEM = {
	0xc0, 0x03, 0x30, 0x0c, 0x0c, 0x30, 0x84, 0x21, 0x82, 0x41, 0x02, 0x40,
	0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x82, 0x41, 0x82, 0x41,
0x84, 0x21, 0x0c, 0x30, 0x30, 0x0c, 0xc0, 0x03 };

unsigned const char icon_warning_bits[] PROGMEM = {
	0x80, 0x01, 0x40, 0x02, 0x40, 0x02, 0x20, 0x04, 0x20, 0x04, 0x90, 0x09,
	0x90, 0x09, 0x88, 0x11, 0x88, 0x11, 0x84, 0x21, 0x84, 0x21, 0x02, 0x40,
0x82, 0x41, 0x81, 0x81, 0x01, 0x80, 0xfe, 0x7f };

unsigned const char icon_error_bits[] PROGMEM = {
	0xc0, 0x03, 0x30, 0x0c, 0x0c, 0x30, 0x04, 0x20, 0x12, 0x48, 0x22, 0x44,
	0x41, 0x82, 0x81, 0x81, 0x81, 0x81, 0x41, 0x82, 0x22, 0x44, 0x12, 0x48,
0x04, 0x20, 0x0c, 0x30, 0x30, 0x0c, 0xc0, 0x03 };