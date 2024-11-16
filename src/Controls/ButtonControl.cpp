/*
 * ButtonControl.cpp
 */ 

#include "Controls/ButtonControl.h"
#include "Core/UI_Manager.h"
#include <string.h>

ButtonControl::ButtonControl(const char* buttonText, void* controlContext, void(*onClick)(void* controlContext), int maxStringLength) : UIElement(UI_CONTROL)
{
	_buttonText = new char[maxStringLength]();
	strncpy(_buttonText, buttonText, maxStringLength);	// Copy a maximum number of StringLength characters to the _buttonText buffer. If text is shorter, the array is zero padded.
	_buttonText[maxStringLength - 1] = '\0';			// The _buttonText buffer must contain at least one termination character ('\0') at the end to protect from overflow.
	_controlContext = controlContext;
	_onClick = onClick;
}

ButtonControl::ButtonControl(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height, const char* buttonText, void* controlContext, void(*onClick)(void* controlContext), int maxStringLength) : UIElement(locX, locY, UI_CONTROL)
{
	Width = width;
	Height = height;
	_buttonText = new char[maxStringLength]();
	strncpy(_buttonText, buttonText, maxStringLength);	// Copy a maximum number of StringLength characters to the _buttonText buffer. If text is shorter, the array is zero padded.
	_buttonText[maxStringLength - 1] = '\0';			// The _buttonText buffer must contain at least one termination character ('\0') at the end to protect from overflow.
	_controlContext = controlContext;
	_onClick = onClick;
}

void ButtonControl::Draw(bool redraw)
{
	if (Visible)
	{
		if(redraw || !_lastDrawnVisible)
		{
			_lastDrawnVisible = true;
			UiManager.Gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);
			
			UiManager.Gfx->drawFastHLine(LocX + UiManager.ElementMargin + 1, LocY + UiManager.ElementMargin + 1, 5, UiManager.ColorForeground);					// Upper left corner
			UiManager.Gfx->drawFastVLine(LocX + UiManager.ElementMargin + 1, LocY + UiManager.ElementMargin + 1, 5, UiManager.ColorForeground);
			UiManager.Gfx->drawFastHLine(LocX + Width - UiManager.ElementMargin - 7, LocY + UiManager.ElementMargin + 1, 5, UiManager.ColorForeground);			// Upper right corner
			UiManager.Gfx->drawFastVLine(LocX + Width - UiManager.ElementMargin - 2, LocY + UiManager.ElementMargin + 1, 5, UiManager.ColorForeground);
			UiManager.Gfx->drawFastHLine(LocX + UiManager.ElementMargin + 1, LocY + Height - UiManager.ElementMargin - 2, 5, UiManager.ColorForeground);				// Lower left corner
			UiManager.Gfx->drawFastVLine(LocX + UiManager.ElementMargin + 1, LocY + Height - UiManager.ElementMargin - 7, 5, UiManager.ColorForeground);
			UiManager.Gfx->drawFastHLine(LocX + Width - UiManager.ElementMargin - 7, LocY + Height - UiManager.ElementMargin - 2, 5, UiManager.ColorForeground);		// Lower right corner		
			UiManager.Gfx->drawFastVLine(LocX + Width - UiManager.ElementMargin - 2, LocY + Height - UiManager.ElementMargin - 6, 5, UiManager.ColorForeground);
			UiManager.Gfx->setCursor(LocX + UiManager.ElementMargin + 1, LocY + Height - UiManager.ElementMargin - 2 * UiManager.ElementPadding - 2);
			UiManager.Gfx->print(_buttonText);
		}
	}
	else if(!Visible && _lastDrawnVisible)		// clear only when the Visible property changes from true to false
	{
		_lastDrawnVisible = false;
		UiManager.Gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);
	}
}

bool ButtonControl::KeyInput(Keys_t key)
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

bool ButtonControl::TouchInput(uint16_t x, uint16_t y)
{
	if(HitTest(x, y))
	{
		if (_onClick != NULL) { _onClick(_controlContext); return true; }
		else { return false; }
	}
	return false;
}

void ButtonControl::RecalculateDimensions()
{
	Height = UiManager.FontHeight + 2 * UiManager.ElementPadding + 2 * UiManager.ElementMargin + 2;
	
	int16_t x, y;
	uint16_t w, h;
	UiManager.Gfx->getTextBounds(_buttonText, 0, 0, &x, &y, &w, &h);
	Width = w + 2 * UiManager.ElementPadding + 2 * UiManager.ElementMargin + 2; 
}