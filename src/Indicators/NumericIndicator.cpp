/*
 * NumericIndicator.cpp
 */ 

#include "Indicators/NumericIndicator.h"
#include "Core/UI_Manager.h"
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
NumericIndicator<T, stringBufferLength>::NumericIndicator(T* valuePointer, const char* baseUnit, T maxValue, unsigned char numFractionalDigits) : UIElement(UI_INDICATOR)
{
	_valuePointer = valuePointer;
	_baseUnit = baseUnit;
	_unitPrefix = "";
	_maxValue = maxValue;
	_numFractionalDigits = numFractionalDigits;
	_numDigits = _numFractionalDigits + numNonFractionalDigits(maxValue);
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
}

template <class T, int stringBufferLength>
void NumericIndicator<T, stringBufferLength>::Draw()
{
	if (Visible)
	{
		// If the Draw() is called on a object of the NumericIndicator, the Type is UI_INDICATOR
		// If the Draw() is called from an NumericControl object, the Type was set to UI_CONTROL there
		if(this->Type == UI_INDICATOR)
		{
			UiManager.Gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);
		}

		_valueDraw = *_valuePointer;
		if (_lastValueDraw != _valueDraw)
		{
			_lastValueDraw = _valueDraw;
			calculateDisplayValue();

			//https://stackoverflow.com/questions/5932214/printf-string-variable-length-item
			char formatStringBuffer[10];
			sprintf(formatStringBuffer, "%%0%d.%df%s%s", _numDigits + (_numFractionalDigits > 0 ? 1 : 0), _numFractionalDigits + _unitPrefixPower, _unitPrefix, _baseUnit);       // if _numFractionalDigits is 0, no decimal point is used (one character less)
			sprintf(_stringDrawBuffer, formatStringBuffer, fabs(_displayValue));
		}

		UiManager.Gfx->setCursor(LocX + 5, LocY + UiManager.FontHeight - 2 * UiManager.ElementPadding);
		UiManager.Gfx->print(_stringDrawBuffer);				// Draw value without minus sign
		if (_displayValue < 0) 
		{ 
			// Draw minus sign
			UiManager.Gfx->setCursor(LocX, LocY + UiManager.FontHeight - 2 * UiManager.ElementPadding);
			UiManager.Gfx->print("-"); 
		}
	}
	else
	{
		UiManager.Gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);
	}
}

template <class T, int stringBufferLength>
void NumericIndicator<T, stringBufferLength>::RecalculateDimensions()
{
	Height = UiManager.FontHeight + 2 * UiManager.ElementPadding;
	
	uint16_t character_width, base_unit_width;
	UiManager.Gfx->getTextBounds("0", 0, 0, nullptr, nullptr, &character_width, nullptr);
	UiManager.Gfx->getTextBounds(_baseUnit, 0, 0, nullptr, nullptr, &base_unit_width, nullptr);
	Width = (_numDigits + 2) * character_width + base_unit_width + 2 * UiManager.ElementPadding;		// + 2 because (dot between digits and one unit prefix)
}