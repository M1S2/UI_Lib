/*
 * Icon.cpp
 */ 

#include "Indicators/Icon.h"

Icon::Icon(uint16_t iconWidth, uint16_t iconHeight, const uint8_t* iconBits, uint16_t color) : UIElement(0, 0, UI_INDICATOR)
{
	Width = iconWidth;
	Height = iconHeight;
	_iconBits = iconBits;
	_color = color;
}

Icon::Icon(uint16_t locX, uint16_t locY, uint16_t iconWidth, uint16_t iconHeight, const uint8_t* iconBits, uint16_t color) : UIElement(locX, locY, UI_INDICATOR)
{
	Width = iconWidth;
	Height = iconHeight;
	_iconBits = iconBits;
	_color = color;
}

void Icon::Draw(Adafruit_GFX* gfx, bool wasScreenCleared)
{
	if (Visible)
	{
		if(_iconBits != NULL)
		{
			gfx->fillRect(LocX, LocY, Width, Height, UI_LIB_COLOR_BACKGROUND);
			gfx->drawXBitmap(LocX, LocY, _iconBits, Width, Height, _color);
		}
	}
	else
	{
		gfx->fillRect(LocX, LocY, Width, Height, UI_LIB_COLOR_BACKGROUND);
	}
}