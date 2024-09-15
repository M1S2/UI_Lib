/*
 * BoolIndicator.cpp
 */ 

#include "Indicators/BoolIndicator.h"

BoolIndicator::BoolIndicator(uint16_t locX, uint16_t locY, bool* valuePointer) : UIElement(locX, locY, UI_INDICATOR)
{
	Width = 50;
	_valuePointer = valuePointer;
}

void BoolIndicator::Draw(Adafruit_GFX* gfx, bool isFirstPage)
{
	if (Visible)
	{	
		if (isFirstPage) { _valueDraw = *_valuePointer; }

		gfx->setCursor(LocX, LocY + DEFAULT_FONT_OFFSET_Y_BASELINE);
		gfx->print(_valueDraw ? "ON" : "OFF");
	}
}