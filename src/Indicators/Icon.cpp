/*
 * Icon.cpp
 */ 

#include "Indicators/Icon.h"

Icon::Icon(uint16_t locX, uint16_t locY, uint16_t iconWidth, uint16_t iconHeight, const uint8_t* iconBits) : UIElement(locX, locY, UI_INDICATOR)
{
	Width = iconWidth;
	Height = iconHeight;
	_iconBits = iconBits;
}

void Icon::Draw(Adafruit_GFX* gfx, bool isFirstPage)
{
	if (Visible)
	{
		if(_iconBits != NULL)
		{
			gfx->drawXBitmap(LocX, LocY, _iconBits, Width, Height, DEFAULT_UI_ELEMENT_COLOR);
		}
	}
}