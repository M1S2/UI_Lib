/*
 * ButtonControl.cpp
 */ 

#include "Controls/ButtonControl.h"
#include <string.h>

template <int StringLength>
ButtonControl<StringLength>::ButtonControl(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height, const char* buttonText, void* controlContext, void(*onClick)(void* controlContext)) : UIElement(locX, locY, UI_CONTROL)
{
	Width = width;
	Height = height;
	strncpy(_buttonText, buttonText, StringLength);		// Copy a maximum number of StringLength characters to the _buttonText buffer. If text is shorter, the array is zero padded.
	_buttonText[StringLength - 1] = '\0';				// The _buttonText buffer must contain at least one termination character ('\0') at the end to protect from overflow.
	_controlContext = controlContext;
	_onClick = onClick;
}

template <int StringLength>
void ButtonControl<StringLength>::Draw(Adafruit_GFX* gfx, bool isFirstPage)
{
	if (Visible)
	{
		gfx->drawFastHLine(LocX - 1, LocY - 1, 3, DEFAULT_UI_ELEMENT_COLOR);					// Upper left corner
		gfx->drawFastVLine(LocX - 1, LocY - 1, 3, DEFAULT_UI_ELEMENT_COLOR);
		gfx->drawFastHLine(LocX + Width - 2, LocY - 1, 3, DEFAULT_UI_ELEMENT_COLOR);			// Upper right corner
		gfx->drawFastVLine(LocX + Width, LocY - 1, 3, DEFAULT_UI_ELEMENT_COLOR);
		gfx->drawFastHLine(LocX - 1, LocY + Height, 3, DEFAULT_UI_ELEMENT_COLOR);				// Lower left corner
		gfx->drawFastVLine(LocX - 1, LocY + Height - 2, 3, DEFAULT_UI_ELEMENT_COLOR);
		gfx->drawFastHLine(LocX + Width - 2, LocY + Height, 3, DEFAULT_UI_ELEMENT_COLOR);		// Lower right corner		
		gfx->drawFastVLine(LocX + Width, LocY + Height - 2, 3, DEFAULT_UI_ELEMENT_COLOR);
		gfx->setTextColor(DEFAULT_UI_ELEMENT_COLOR);
		gfx->setCursor(LocX, LocY + DEFAULT_FONT_OFFSET_Y_BASELINE);
		gfx->print(_buttonText);
	}
}

template <int StringLength>
bool ButtonControl<StringLength>::KeyInput(Keys_t key)
{
	switch (key)
	{
		case KEYOK:
			if (_onClick != NULL) { _onClick(_controlContext); return true; }
			else { return false; }
		default:
			return false;
	}
}