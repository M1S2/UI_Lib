/*
 * ProgressBar.cpp
 */ 

#include "Indicators/ProgressBar.h"
#include <math.h>

template <class T>
uint16_t ProgressBar<T>::xCoordinateFromValue(T value)
{
	return LocX + (uint16_t)(((value - _minValue) / (float)(_maxValue - _minValue)) * (Width - 1));
}

//------------------------------------------------------------------------------------------------------------------------------------------------

template <class T>
ProgressBar<T>::ProgressBar(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height, T* valuePointer, T minValue, T maxValue, ProgressBarOrigin_t origin, T tickIncrement): UIElement(locX, locY, UI_INDICATOR)
{
	Width = width;
	Height = height;
	_valuePointer = valuePointer;
	_minValue = minValue;
	_maxValue = maxValue;
	_origin = origin;
	_tickIncrement = tickIncrement;

	_originXCoord = xCoordinateFromValue(_origin == PROGRESSBAR_ORIGIN_LEFT ? _minValue : (_origin == PROGRESSBAR_ORIGIN_RIGHT ? _maxValue : 0));
}

template <class T>
void ProgressBar<T>::Draw(Adafruit_GFX* gfx, bool isFirstPage)
{
	if (Visible)
	{
		if (isFirstPage) 
		{ 
			_valueDraw = *_valuePointer; 
			if (_valueDraw > _maxValue) { _valueDraw = _maxValue; }			// Coerce value to be between _minValue and _maxValue
			else if (_valueDraw < _minValue) { _valueDraw = _minValue; }
		}

		uint16_t valueXCoord = xCoordinateFromValue(_valueDraw);
		
		// Draw outer border of progress bar
		gfx->drawRect(xCoordinateFromValue(_minValue), LocY, Width, Height, DEFAULT_UI_ELEMENT_COLOR);

		// Change font for min and max value strings
		//const u8g_fntpgm_uint8_t* tmp_font;
		//tmp_font = u8g->font;
		//u8g_SetFont(u8g, u8g_font_5x7r);
				
		char buffer[6];
		itoa(_maxValue, buffer, 10);
		gfx->setCursor(xCoordinateFromValue(_maxValue) + 3, LocY + DEFAULT_FONT_OFFSET_Y_BASELINE);
		gfx->print(buffer);

		itoa(_minValue, buffer, 10);
		uint16_t minValueTextWidth;
		gfx->getTextBounds(buffer, 0, 0, nullptr, nullptr, &minValueTextWidth, nullptr);
		gfx->setCursor(xCoordinateFromValue(_minValue) - 3 - minValueTextWidth, LocY + DEFAULT_FONT_OFFSET_Y_BASELINE);
		gfx->print(buffer);

		// Change back font to previous font
		//gfx->setFont(DEFAULT_FONT);

		gfx->fillRect((uint16_t)fmin(valueXCoord, _originXCoord), LocY, (uint16_t)fabs(valueXCoord - _originXCoord), Height, DEFAULT_UI_ELEMENT_COLOR);
		
		if(_tickIncrement > 0)			// Use _tickIncrement<=0 to disable ticks
		{
			for (T xVal = _minValue; xVal <= _maxValue; xVal+=_tickIncrement)
			{
				int xCoord = xCoordinateFromValue(xVal);
				int tickLength = (((int)xVal) % 10 == 0 ? 3 : (((int)xVal) % 5 == 0 ? 2 : 1));
				gfx->drawFastVLine(xCoord, LocY - tickLength, tickLength, DEFAULT_UI_ELEMENT_COLOR);
			}
		}
	}
}