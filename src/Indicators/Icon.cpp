/*
 * Icon.cpp
 */ 

#include "Indicators/Icon.h"
#include "Core/UI_Manager.h"

Icon::Icon(uint16_t iconWidth, uint16_t iconHeight, const uint8_t* iconBits, uint16_t color, uint16_t locX, uint16_t locY) : UIElement(locX, locY, UI_INDICATOR)
{
	_iconWidth = iconWidth;
	_iconHeight = iconHeight;
	_iconBits = iconBits;
	_color = color;
}

void Icon::Draw(bool redraw)
{
	if (Visible)
	{
		if(_iconBits != NULL && (redraw || !_lastDrawnVisible))
		{
			_lastDrawnVisible = true;
			UiManager.Gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);
			UiManager.Gfx->drawXBitmap(LocX + UiManager.ElementMargin + UiManager.ElementPadding, LocY + UiManager.ElementMargin + UiManager.ElementPadding, _iconBits, _iconWidth, _iconHeight, (_color == ICON_COLOR_NOTSET ? UiManager.ColorForeground : _color));
		}
	}
	else if(!Visible && _lastDrawnVisible)		// clear only when the Visible property changes from true to false
	{
		_lastDrawnVisible = false;
		UiManager.Gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);
	}
}

void Icon::RecalculateDimensions()
{
	Width = _iconWidth + 2 * UiManager.ElementPadding + 2 * UiManager.ElementMargin;
	Height = _iconHeight + 2 * UiManager.ElementPadding + 2 * UiManager.ElementMargin;
}