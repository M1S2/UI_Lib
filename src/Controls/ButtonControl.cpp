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
void ButtonControl<StringLength>::Draw()
{
	if (Visible)
	{
		UiManager.Gfx->fillRect(LocX - 1, LocY - 1, Width + 2, Height + 2, UiManager.ColorBackground);
		
		UiManager.Gfx->drawFastHLine(LocX - 1, LocY - 1, 5, UiManager.ColorForeground);					// Upper left corner
		UiManager.Gfx->drawFastVLine(LocX - 1, LocY - 1, 5, UiManager.ColorForeground);
		UiManager.Gfx->drawFastHLine(LocX + Width - 4, LocY - 1, 5, UiManager.ColorForeground);			// Upper right corner
		UiManager.Gfx->drawFastVLine(LocX + Width, LocY - 1, 5, UiManager.ColorForeground);
		UiManager.Gfx->drawFastHLine(LocX - 1, LocY + Height, 5, UiManager.ColorForeground);				// Lower left corner
		UiManager.Gfx->drawFastVLine(LocX - 1, LocY + Height - 4, 5, UiManager.ColorForeground);
		UiManager.Gfx->drawFastHLine(LocX + Width - 4, LocY + Height, 5, UiManager.ColorForeground);		// Lower right corner		
		UiManager.Gfx->drawFastVLine(LocX + Width, LocY + Height - 4, 5, UiManager.ColorForeground);
		UiManager.Gfx->setTextColor(UiManager.ColorForeground);
		UiManager.Gfx->setCursor(LocX, LocY + UiManager.FontHeight - 4);
		UiManager.Gfx->print(_buttonText);
	}
	else
	{
		UiManager.Gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);
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

template <int StringLength>
void ButtonControl<StringLength>::RecalculateDimensions()
{
	Height = UiManager.FontHeight + 2 * UiManager.ElementPadding;
	
	int16_t x, y;
	uint16_t w, h;
	UiManager.Gfx->getTextBounds(_buttonText, 0, 0, &x, &y, &w, &h);
	Width = w + 2 * UiManager.ElementPadding; 
}