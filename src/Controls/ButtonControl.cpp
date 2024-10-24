/*
 * ButtonControl.cpp
 */ 

#include "Controls/ButtonControl.h"
#include "Core/UI_Manager.h"
#include <string.h>

template <int StringLength>
ButtonControl<StringLength>::ButtonControl(const char* buttonText, void* controlContext, void(*onClick)(void* controlContext)) : UIElement(UI_CONTROL)
{
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
void ButtonControl<StringLength>::Draw(bool redraw)
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
	Height = UiManager.FontHeight + 2 * UiManager.ElementPadding + 2 * UiManager.ElementMargin + 2;
	
	int16_t x, y;
	uint16_t w, h;
	UiManager.Gfx->getTextBounds(_buttonText, 0, 0, &x, &y, &w, &h);
	Width = w + 2 * UiManager.ElementPadding + 2 * UiManager.ElementMargin + 2; 
}