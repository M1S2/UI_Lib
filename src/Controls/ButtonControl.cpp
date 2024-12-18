/*
 * ButtonControl.cpp
 */ 

#include "Controls/ButtonControl.h"
#include "Core/UI_Manager.h"
#include <string.h>

ButtonControl::ButtonControl(const char* buttonText, void* controlContext, void(*onClick)(void* controlContext), uint16_t locX, uint16_t locY, int maxStringLength) : UIElement(locX, locY, UI_CONTROL)
{
	_buttonText = new char[maxStringLength]();
	strncpy(_buttonText, buttonText, maxStringLength);	// Copy a maximum number of StringLength characters to the _buttonText buffer. If text is shorter, the array is zero padded.
	_buttonText[maxStringLength - 1] = '\0';			// The _buttonText buffer must contain at least one termination character ('\0') at the end to protect from overflow.
	ControlContext = controlContext;
	OnClick = onClick;
}

void ButtonControl::Draw(bool redraw)
{
	if (Visible)
	{
		if(redraw || !_lastDrawnVisible)
		{
			_lastDrawnVisible = true;
			UiManager.Gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);
			UiManager.Gfx->drawRect(LocX + UiManager.ElementMargin, LocY + UiManager.ElementMargin, Width - 2 * UiManager.ElementMargin, Height - 2 * UiManager.ElementMargin, UiManager.ColorForeground); 
			UiManager.Gfx->setCursor(LocX + UiManager.ElementMargin + UiManager.ElementPadding + 1, LocY + Height - UiManager.ElementMargin - UiManager.ElementPadding - 2);
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
			if (OnClick != NULL) { OnClick(ControlContext); return true; }
			else { return false; }
		default:
			return false;
	}
}

bool ButtonControl::TouchInput(uint16_t x, uint16_t y, TouchTypes touchType)
{
	if(HitTest(x, y))
	{
		if (OnClick != NULL) { OnClick(ControlContext); }
		return true;
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