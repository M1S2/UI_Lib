/*
 * BoolIndicator.cpp
 */ 

#include "Indicators/BoolIndicator.h"
#include "Core/UI_Manager.h"

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

void BoolIndicator::Draw(bool redraw)
{
	if (Visible)
	{
		if (_lastValueDraw != *_valuePointer || redraw || !_lastDrawnVisible)
		{
			_lastDrawnVisible = true;
			_lastValueDraw = *_valuePointer;

			// If the Draw() is called on a object of the BoolIndicator, the Type is UI_INDICATOR
			// If the Draw() is called from an BoolControl object, the Type was set to UI_CONTROL there
			if(this->Type == UI_INDICATOR)
			{
				UiManager.Gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);	
			}
		
			UiManager.Gfx->setCursor(LocX, LocY + UiManager.FontHeight - 2 * UiManager.ElementPadding);
			UiManager.Gfx->print(*_valuePointer ? "ON" : "OFF");
		}
	}
	else if(!Visible && _lastDrawnVisible)		// clear only when the Visible property changes from true to false
	{
		_lastDrawnVisible = false;
		UiManager.Gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);
	}
}

void BoolIndicator::RecalculateDimensions()
{
	Height = UiManager.FontHeight + 2 * UiManager.ElementPadding;
	
	int16_t x, y;
	uint16_t w, h;
	UiManager.Gfx->getTextBounds("OFF", 0, 0, &x, &y, &w, &h);
	Width = w + 2 * UiManager.ElementPadding; 
}