/*
 * ButtonControl.cpp
 */ 

#include "Controls/ButtonControl.h"
#include "Core/UI_Manager.h"
#include <string.h>

template <int StringLength>
ButtonControl<StringLength>::ButtonControl(uint16_t width, uint16_t height, const char* buttonText, void* controlContext, void(*onClick)(void* controlContext)) : UIElement(UI_CONTROL)
{
	Width = width;
	Height = height;
	strncpy(_buttonText, buttonText, StringLength);		// Copy a maximum number of StringLength characters to the _buttonText buffer. If text is shorter, the array is zero padded.
	_buttonText[StringLength - 1] = '\0';				// The _buttonText buffer must contain at least one termination character ('\0') at the end to protect from overflow.
	_controlContext = controlContext;
	_onClick = onClick;
}

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
void ButtonControl<StringLength>::Draw(Adafruit_GFX* gfx)
{
	if (Visible)
	{
		gfx->fillRect(LocX - 1, LocY - 1, Width + 2, Height + 2, UiManager.ColorBackground);
		
		gfx->drawFastHLine(LocX - 1, LocY - 1, 5, UiManager.ColorForeground);					// Upper left corner
		gfx->drawFastVLine(LocX - 1, LocY - 1, 5, UiManager.ColorForeground);
		gfx->drawFastHLine(LocX + Width - 4, LocY - 1, 5, UiManager.ColorForeground);			// Upper right corner
		gfx->drawFastVLine(LocX + Width, LocY - 1, 5, UiManager.ColorForeground);
		gfx->drawFastHLine(LocX - 1, LocY + Height, 5, UiManager.ColorForeground);				// Lower left corner
		gfx->drawFastVLine(LocX - 1, LocY + Height - 4, 5, UiManager.ColorForeground);
		gfx->drawFastHLine(LocX + Width - 4, LocY + Height, 5, UiManager.ColorForeground);		// Lower right corner		
		gfx->drawFastVLine(LocX + Width, LocY + Height - 4, 5, UiManager.ColorForeground);
		gfx->setTextColor(UiManager.ColorForeground);
		gfx->setCursor(LocX, LocY + UI_LIB_DEFAULT_FONT_OFFSET_Y_BASELINE);
		gfx->print(_buttonText);
	}
	else
	{
		gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);
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