/*
 * ProgressBar.cpp
 */ 

#include "Indicators/ProgressBar.h"
#include "Core/UI_Manager.h"
#include <math.h>

template <class T>
uint16_t ProgressBar<T>::xCoordinateFromValue(T value)
{
	return LocX + _minValueTextWidth + UiManager.ElementPadding + (uint16_t)(((value - _minValue) / (float)(_maxValue - _minValue)) * (ProgressbarWidth - 1));
}

//------------------------------------------------------------------------------------------------------------------------------------------------

template <class T>
ProgressBar<T>::ProgressBar(T* valuePointer, T minValue, T maxValue, ProgressBarOrigin_t origin, T tickIncrement, uint16_t progressbarWidth, uint16_t progressbarHeight): UIElement(UI_INDICATOR)
{
	ProgressbarWidth = progressbarWidth;
	ProgressbarHeight = progressbarHeight;
	_valuePointer = valuePointer;
	_minValue = minValue;
	_maxValue = maxValue;
	_origin = origin;
	_tickIncrement = tickIncrement;
}

template <class T>
ProgressBar<T>::ProgressBar(uint16_t locX, uint16_t locY, T* valuePointer, T minValue, T maxValue, ProgressBarOrigin_t origin, T tickIncrement, uint16_t progressbarWidth, uint16_t progressbarHeight): UIElement(locX, locY, UI_INDICATOR)
{
	ProgressbarWidth = progressbarWidth;
	ProgressbarHeight = progressbarHeight;
	_valuePointer = valuePointer;
	_minValue = minValue;
	_maxValue = maxValue;
	_origin = origin;
	_tickIncrement = tickIncrement;
}

template <class T>
void ProgressBar<T>::Draw(bool redraw)
{
	if (Visible)
	{
		if (_lastValueDraw != *_valuePointer || redraw)
		{
			UiManager.Gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);
								
			_lastValueDraw = *_valuePointer; 
			if (_lastValueDraw > _maxValue) { _lastValueDraw = _maxValue; }                                             // Coerce value to be between _minValue and _maxValue
			else if (_lastValueDraw < _minValue) { _lastValueDraw = _minValue; }

			// Draw outer border of progress bar
			UiManager.Gfx->drawRect(xCoordinateFromValue(_minValue), LocY + Height - ProgressbarHeight, ProgressbarWidth, ProgressbarHeight, UiManager.ColorForeground);
			
			// Draw inner progress bar
			uint16_t valueXCoord = xCoordinateFromValue(_lastValueDraw);
			UiManager.Gfx->fillRect((uint16_t)fmin(valueXCoord, _originXCoord), LocY + Height - ProgressbarHeight, (uint16_t)fabs(valueXCoord - _originXCoord), ProgressbarHeight, UiManager.ColorForeground);
			
			// Draw min and max value texts
			char buffer[6];
			itoa(_maxValue, buffer, 10);
			UiManager.Gfx->setCursor(xCoordinateFromValue(_maxValue) + UiManager.ElementPadding, LocY + Height - UiManager.ElementPadding);
			UiManager.Gfx->print(buffer);
			itoa(_minValue, buffer, 10);
			UiManager.Gfx->setCursor(xCoordinateFromValue(_minValue) - UiManager.ElementPadding - _minValueTextWidth, LocY + Height - UiManager.ElementPadding);
			UiManager.Gfx->print(buffer);

			if(_tickIncrement > 0)                                    // Use _tickIncrement<=0 to disable ticks
			{
				for (T xVal = _minValue; xVal <= _maxValue; xVal+=_tickIncrement)
				{
					int xCoord = xCoordinateFromValue(xVal);
					int tickLength = (((int)xVal) % 10 == 0 ? PROGRESSBAR_LONG_TICK_LENGTH : (((int)xVal) % 5 == 0 ? PROGRESSBAR_MIDDLE_TICK_LENGTH : PROGRESSBAR_SHORT_TICK_LENGTH));
					UiManager.Gfx->drawFastVLine(xCoord, LocY + Height - ProgressbarHeight - tickLength, tickLength, UiManager.ColorForeground);
				}
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
	char buffer[6];
	itoa(_maxValue, buffer, 10);
	UiManager.Gfx->getTextBounds(buffer, 0, 0, nullptr, nullptr, &_maxValueTextWidth, nullptr);
	itoa(_minValue, buffer, 10);
	UiManager.Gfx->getTextBounds(buffer, 0, 0, nullptr, nullptr, &_minValueTextWidth, nullptr);

	Width = _minValueTextWidth + UiManager.ElementPadding + ProgressbarWidth + UiManager.ElementPadding + _maxValueTextWidth;
	Height = PROGRESSBAR_LONG_TICK_LENGTH + ProgressbarHeight;
}

template <class T>
void ProgressBar<T>::RecalculateLayout()
{
	_originXCoord = xCoordinateFromValue(_origin == PROGRESSBAR_ORIGIN_LEFT ? _minValue : (_origin == PROGRESSBAR_ORIGIN_RIGHT ? _maxValue : 0));
	RecalculateDimensions();
}
