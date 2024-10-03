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

void BoolIndicator::Draw()
{
	if (Visible)
	{	
		// If the Draw() is called on a object of the BoolIndicator, the Type is UI_INDICATOR
		// If the Draw() is called from an BoolControl object, the Type was set to UI_CONTROL there
		if(this->Type == UI_INDICATOR)
		{
			UiManager.Gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);	
		} 
		
		_valueDraw = *_valuePointer;
	
		UiManager.Gfx->setCursor(LocX, LocY + UiManager.FontHeight - 2 * UiManager.ElementPadding);
		UiManager.Gfx->print(_valueDraw ? "ON" : "OFF");
	}
	else
	{
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