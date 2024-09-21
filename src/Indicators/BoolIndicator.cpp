/*
 * BoolIndicator.cpp
 */ 

#include "Indicators/BoolIndicator.h"

BoolIndicator::BoolIndicator(bool* valuePointer) : UIElement(UI_INDICATOR)
{
	Width = 50;
	_valuePointer = valuePointer;
}

BoolIndicator::BoolIndicator(uint16_t locX, uint16_t locY, bool* valuePointer) : UIElement(locX, locY, UI_INDICATOR)
{
	Width = 50;
	_valuePointer = valuePointer;
}

void BoolIndicator::Draw(Adafruit_GFX* gfx, bool wasScreenCleared)
{
	if (Visible)
	{	
		// If the Draw() is called on a object of the BoolIndicator, the Type is UI_INDICATOR
		// If the Draw() is called from an BoolControl object, the Type was set to UI_CONTROL there
		if(this->Type == UI_INDICATOR)
		{
			gfx->fillRect(LocX, LocY, Width, Height, UI_LIB_COLOR_BACKGROUND);	
		} 
		
		_valueDraw = *_valuePointer;
	
		gfx->setCursor(LocX, LocY + UI_LIB_DEFAULT_FONT_OFFSET_Y_BASELINE);
		gfx->print(_valueDraw ? "ON" : "OFF");
	}
	else
	{
		gfx->fillRect(LocX, LocY, Width, Height, UI_LIB_COLOR_BACKGROUND);
	}
}