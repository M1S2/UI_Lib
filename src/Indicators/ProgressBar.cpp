/*
 * ProgressBar.cpp
 */ 

#include "Indicators/ProgressBar.h"
#include "Core/UI_Manager.h"
#include <math.h>

template <class T>
uint16_t ProgressBar<T>::xCoordinateFromValue(T value)
{
	return LocX + (uint16_t)(((value - _minValue) / (float)(_maxValue - _minValue)) * (Width - 1));
}

//------------------------------------------------------------------------------------------------------------------------------------------------

template <class T>
ProgressBar<T>::ProgressBar(uint16_t width, uint16_t height, T* valuePointer, T minValue, T maxValue, ProgressBarOrigin_t origin, T tickIncrement): UIElement(UI_INDICATOR)
{
	Width = width;
	Height = height;
	_valuePointer = valuePointer;
	_minValue = minValue;
	_maxValue = maxValue;
	_origin = origin;
	_tickIncrement = tickIncrement;
}

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
}

template <class T>
void ProgressBar<T>::Draw()
{
	if (Visible)
	{
		UiManager.Gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);
		
		_valueDraw = *_valuePointer; 
		if (_valueDraw > _maxValue) { _valueDraw = _maxValue; }			// Coerce value to be between _minValue and _maxValue
		else if (_valueDraw < _minValue) { _valueDraw = _minValue; }

		_originXCoord = xCoordinateFromValue(_origin == PROGRESSBAR_ORIGIN_LEFT ? _minValue : (_origin == PROGRESSBAR_ORIGIN_RIGHT ? _maxValue : 0));
		
		uint16_t valueXCoord = xCoordinateFromValue(_valueDraw);
		
		// Draw outer border of progress bar
		UiManager.Gfx->drawRect(xCoordinateFromValue(_minValue), LocY, Width, Height, UiManager.ColorForeground);
				
		char buffer[6];
		itoa(_maxValue, buffer, 10);
		UiManager.Gfx->setCursor(xCoordinateFromValue(_maxValue) + 3, LocY + UiManager.FontHeight - 2 * UiManager.ElementPadding);
		UiManager.Gfx->print(buffer);

		itoa(_minValue, buffer, 10);
		uint16_t minValueTextWidth;
		UiManager.Gfx->getTextBounds(buffer, 0, 0, nullptr, nullptr, &minValueTextWidth, nullptr);
		UiManager.Gfx->setCursor(xCoordinateFromValue(_minValue) - 3 - minValueTextWidth, LocY + UiManager.FontHeight - 2 * UiManager.ElementPadding);
		UiManager.Gfx->print(buffer);

		UiManager.Gfx->fillRect((uint16_t)fmin(valueXCoord, _originXCoord), LocY, (uint16_t)fabs(valueXCoord - _originXCoord), Height, UiManager.ColorForeground);
		
		if(_tickIncrement > 0)			// Use _tickIncrement<=0 to disable ticks
		{
			for (T xVal = _minValue; xVal <= _maxValue; xVal+=_tickIncrement)
			{
				int xCoord = xCoordinateFromValue(xVal);
				int tickLength = (((int)xVal) % 10 == 0 ? 3 : (((int)xVal) % 5 == 0 ? 2 : 1));
				UiManager.Gfx->drawFastVLine(xCoord, LocY - tickLength, tickLength, UiManager.ColorForeground);
			}
		}
	}
	else
	{
		UiManager.Gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);
	}
}

template <class T>
void ProgressBar<T>::RecalculateDimensions()
{
	// Override default UIElement behaviour. Dimensions are set in the constructor and shouldn't be changed here
}