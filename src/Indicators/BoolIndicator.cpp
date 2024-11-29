/*
 * BoolIndicator.cpp
 */ 

#include "Indicators/BoolIndicator.h"
#include "Core/UI_Manager.h"

BoolIndicator::BoolIndicator(bool* valuePointer, BoolIndicatorStyles_t style, uint16_t locX, uint16_t locY) : UIElement(locX, locY, UI_INDICATOR)
{
	_valuePointer = valuePointer;
	_style = style;
}

void BoolIndicator::Draw(bool redraw)
{
	if (Visible)
	{
		if (_lastValueDraw != *_valuePointer || redraw || !_lastDrawnVisible)
		{
			_lastDrawnVisible = true;
			_lastValueDraw = *_valuePointer;
			UiManager.Gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);	
		
			switch(_style)
			{
				case BOOLINDICATOR_STYLE_LED:
				{
					// Draw the led outline using the rounded rectangle method (easier to position the circle than using the fillCircle method)
					UiManager.Gfx->drawRoundRect(LocX + UiManager.ElementMargin + UiManager.ElementPadding, LocY + UiManager.ElementMargin + UiManager.ElementPadding, DEFAULT_BOOLINDICATOR_LED_DIAMETER, DEFAULT_BOOLINDICATOR_LED_DIAMETER, (DEFAULT_BOOLINDICATOR_LED_DIAMETER / 2), UiManager.ColorForeground);	
					
					if(*_valuePointer)
					{
						// Draw the led inner circle using the rounded rectangle method (easier to position the circle than using the fillCircle method)
						UiManager.Gfx->fillRoundRect(LocX + UiManager.ElementMargin + UiManager.ElementPadding + DEFAULT_BOOLINDICATOR_LED_SPACING, LocY + UiManager.ElementMargin + UiManager.ElementPadding + DEFAULT_BOOLINDICATOR_LED_SPACING, DEFAULT_BOOLINDICATOR_LED_DIAMETER - 2 * DEFAULT_BOOLINDICATOR_LED_SPACING, DEFAULT_BOOLINDICATOR_LED_DIAMETER - 2 * DEFAULT_BOOLINDICATOR_LED_SPACING, ((DEFAULT_BOOLINDICATOR_LED_DIAMETER - 2 * DEFAULT_BOOLINDICATOR_LED_SPACING) / 2), UiManager.ColorForeground);	
					}					
					break;
				}
				case BOOLINDICATOR_STYLE_CLASSIC:
				{
					UiManager.Gfx->setCursor(LocX + UiManager.ElementMargin + UiManager.ElementPadding, LocY + Height - UiManager.ElementMargin - 2 * UiManager.ElementPadding - 1);
					UiManager.Gfx->print(*_valuePointer ? "ON" : "OFF");
					break;
				}
				default: break;
			}
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
	switch(_style)
	{
		case BOOLINDICATOR_STYLE_LED:
		{
			Width = DEFAULT_BOOLINDICATOR_LED_DIAMETER + 2 * UiManager.ElementPadding + 2 * UiManager.ElementMargin;
			Height = DEFAULT_BOOLINDICATOR_LED_DIAMETER + 2 * UiManager.ElementPadding + 2 * UiManager.ElementMargin;
			break;
		}
		case BOOLINDICATOR_STYLE_CLASSIC:
		{
			int16_t x, y;
			uint16_t w, h;
			UiManager.Gfx->getTextBounds("OFF", 0, 0, &x, &y, &w, &h);
			Width = w + 2 * UiManager.ElementPadding + 2 * UiManager.ElementMargin + 2;
			Height = UiManager.FontHeight + 2 * UiManager.ElementPadding + 2 * UiManager.ElementMargin;
			break;
		}
		default: break;
	}
}