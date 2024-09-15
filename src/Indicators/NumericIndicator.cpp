/*
 * NumericIndicator.cpp
 */ 

#include "Indicators/NumericIndicator.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

#include <stdlib.h>

template <class T, int stringBufferLength>
void NumericIndicator<T, stringBufferLength>::calculateDisplayValue()
{
	_displayValue = (float)_valueDraw;
	_unitPrefixPower = 0;

	if (fabs(_displayValue) < pow(10, -_numFractionalDigits))
	{
		_displayValue = 0;
		/* _unitPrefixPower is set above. */
	}
	else if (fabs(_displayValue) >= 1000)
	{
		while (fabs(_displayValue) >= 1000)
		{
			_unitPrefixPower += 3;
			_displayValue /= 1000;
		}
	}
	else if (fabs(_displayValue) < 1)
	{
		while (fabs(_displayValue) < 1)
		{
			_unitPrefixPower -= 3;
			_displayValue *= 1000;
		}
	}

	_unitPrefix = (_unitPrefixPower == -3 ? "m" : (_unitPrefixPower == 0 ? "" : (_unitPrefixPower == 3 ? "k" : (_unitPrefixPower == 6 ? "M" : ""))));
}

template <class T, int stringBufferLength>
int NumericIndicator<T, stringBufferLength>::numNonFractionalDigits(T number)
{
	int digits = 0;
	//if (number < 0) digits = 1; // remove this line if '-' counts as a digit
	while (fabs(number) >= 1)
	{
		number /= 10;
		digits++;
	}
	return digits;
}


template <class T, int stringBufferLength>
NumericIndicator<T, stringBufferLength>::NumericIndicator(uint16_t locX, uint16_t locY, T* valuePointer, const char* baseUnit, T maxValue, unsigned char numFractionalDigits) : UIElement(locX, locY, UI_INDICATOR)
{
	_valuePointer = valuePointer;
	_baseUnit = baseUnit;
	_unitPrefix = "";
	_maxValue = maxValue;
	_numFractionalDigits = numFractionalDigits;
	_numDigits = _numFractionalDigits + numNonFractionalDigits(maxValue);
	_firstDraw = true;
}

template <class T, int stringBufferLength>
void NumericIndicator<T, stringBufferLength>::Draw(Adafruit_GFX* gfx, bool isFirstPage)
{
	if (Visible)
	{
		if (isFirstPage)
		{
			_valueDraw = *_valuePointer;
			if (_lastValueDraw != _valueDraw || _firstDraw)
			{
				_lastValueDraw = _valueDraw;
				calculateDisplayValue();

				//https://stackoverflow.com/questions/5932214/printf-string-variable-length-item
				char formatStringBuffer[10];
				sprintf(formatStringBuffer, "%%0%d.%df%s%s", _numDigits + (_numFractionalDigits > 0 ? 1 : 0), _numFractionalDigits + _unitPrefixPower, _unitPrefix, _baseUnit);       // if _numFractionalDigits is 0, no decimal point is used (one character less)
				sprintf(_stringDrawBuffer, formatStringBuffer, fabs(_displayValue));
				_firstDraw = false;
			}
		}

		gfx->setCursor(LocX + 5, LocY + DEFAULT_FONT_OFFSET_Y_BASELINE);
		gfx->print(_stringDrawBuffer);				// Draw value without minus sign
		if (_displayValue < 0) 
		{ 
			// Draw minus sign
			gfx->setCursor(LocX, LocY + DEFAULT_FONT_OFFSET_Y_BASELINE);
			gfx->print("-"); 
		}
	}
}