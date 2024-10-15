/*
 * Icon.cpp
 */ 

#include "Indicators/Icon.h"
#include "Core/UI_Manager.h"

Icon::Icon(uint16_t iconWidth, uint16_t iconHeight, const uint8_t* iconBits) : UIElement(0, 0, UI_INDICATOR)
{
	Width = iconWidth;
	Height = iconHeight;
	_iconBits = iconBits;
	_wasColorSet = false;
}

Icon::Icon(uint16_t iconWidth, uint16_t iconHeight, const uint8_t* iconBits, uint16_t color) : UIElement(0, 0, UI_INDICATOR)
{
	Width = iconWidth;
	Height = iconHeight;
	_iconBits = iconBits;
	_color = color;
	_wasColorSet = true;
}

Icon::Icon(uint16_t locX, uint16_t locY, uint16_t iconWidth, uint16_t iconHeight, const uint8_t* iconBits) : UIElement(locX, locY, UI_INDICATOR)
{
	Width = iconWidth;
	Height = iconHeight;
	_iconBits = iconBits;
	_wasColorSet = false;
}

Icon::Icon(uint16_t locX, uint16_t locY, uint16_t iconWidth, uint16_t iconHeight, const uint8_t* iconBits, uint16_t color) : UIElement(locX, locY, UI_INDICATOR)
{
	Width = iconWidth;
	Height = iconHeight;
	_iconBits = iconBits;
	_color = color;
	_wasColorSet = true;
}

void Icon::Draw(bool redraw)
{
	if(!_wasColorSet)
	{
		// Initialize the color with the foreground color from the UiManager if the color wasn't set in the constructor
		_color = UiManager.ColorForeground;
		_wasColorSet = true;
	}

	if (Visible)
	{
		if(_iconBits != NULL && redraw)
		{
			UiManager.Gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);
			UiManager.Gfx->drawXBitmap(LocX, LocY, _iconBits, Width, Height, _color);
		}
	}
	else
	{
		UiManager.Gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);
	}
}

void Icon::RecalculateDimensions()
{
	// Override default UIElement behaviour. Dimensions are set in the constructor and shouldn't be changed here
}