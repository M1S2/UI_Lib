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
void NumericIndicator<T, stringBufferLength>::calculateDisplayValue(float value)
{
	_displayValue = value;
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

	switch (_unitPrefixPower)
	{
		case -12: _unitPrefix = "p"; break;
		case -9: _unitPrefix = "n"; break;
		case -6: _unitPrefix = "u"; break;
		case -3: _unitPrefix = "m"; break;
		case 0: _unitPrefix = ""; break;
		case 3: _unitPrefix = "k"; break;
		case 6: _unitPrefix = "M"; break;
		case 9: _unitPrefix = "T"; break;
		default: _unitPrefix = ""; break;
	}
}

template <class T, int stringBufferLength>
uint8_t NumericIndicator<T, stringBufferLength>::numNonFractionalDigits(T number)
{
	uint8_t digits = 0;
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
void NumericIndicator<T, stringBufferLength>::Draw(bool redraw)
{
	if (Visible)
	{
		if (_lastValueDraw != *_valuePointer || redraw || !_lastDrawnVisible)
		{	
			_lastDrawnVisible = true;
			// If the Draw() is called on a object of the NumericIndicator, the Type is UI_INDICATOR
			// If the Draw() is called from an NumericControl object, the Type was set to UI_CONTROL there
			if(Type == UI_INDICATOR)
			{
				UiManager.Gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);
			}

			_lastValueDraw = *_valuePointer;
			calculateDisplayValue((float)_lastValueDraw);

			//https://stackoverflow.com/questions/5932214/printf-string-variable-length-item
			char formatStringBuffer[15];
			sprintf(formatStringBuffer, "%%0%d.%df%s%s", _numDigits + (_numFractionalDigits > 0 ? 1 : 0), max(0, _numFractionalDigits + _unitPrefixPower), _unitPrefix, _baseUnit);       // if _numFractionalDigits is 0, no decimal point is used (one character less); limit the number of factional characters to zero (no negative values)
			sprintf(_stringDrawBuffer, formatStringBuffer, fabs(_displayValue));

			uint16_t minus_width;
			UiManager.Gfx->getTextBounds("-", 0, 0, nullptr, nullptr, &minus_width, nullptr);

			UiManager.Gfx->setCursor(LocX + minus_width + 2 + UiManager.ElementMargin + UiManager.ElementPadding, LocY + Height - UiManager.ElementMargin - 2 * UiManager.ElementPadding - 1);
			UiManager.Gfx->print(_stringDrawBuffer);				// Draw value without minus sign
			if (_displayValue < 0) 
			{ 
				// Draw minus sign
				UiManager.Gfx->setCursor(LocX + UiManager.ElementMargin + UiManager.ElementPadding, LocY + Height - UiManager.ElementMargin - 2 * UiManager.ElementPadding - 1);
				UiManager.Gfx->print("-"); 
			}
		}
	}
	else if(!Visible && _lastDrawnVisible)		// clear only when the Visible property changes from true to false
	{
		_lastDrawnVisible = false;
		UiManager.Gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);
	}
}

template <class T, int stringBufferLength>
void NumericIndicator<T, stringBufferLength>::RecalculateDimensions()
{
	Height = UiManager.FontHeight + 2 * UiManager.ElementPadding + 2 * UiManager.ElementMargin;
	
	calculateDisplayValue(_maxValue);
	_unitPrefix = "m";			// make sure to set the _unitPrefix to a value != "" to take this width into account

	//https://stackoverflow.com/questions/5932214/printf-string-variable-length-item	
	char formatStringBuffer[15];
	sprintf(formatStringBuffer, "-%%0%d.%df%s%s", _numDigits + (_numFractionalDigits > 0 ? 1 : 0), max(0, _numFractionalDigits + _unitPrefixPower), _unitPrefix, _baseUnit);       // if _numFractionalDigits is 0, no decimal point is used (one character less); limit the number of factional characters to zero (no negative values)
	sprintf(_stringDrawBuffer, formatStringBuffer, 0.0f);		// calculate the length of a string with all zeroes ("0" is a wide characters)

	uint16_t minus_width;
	UiManager.Gfx->getTextBounds("-", 0, 0, nullptr, nullptr, &minus_width, nullptr);

	uint16_t string_width;
	UiManager.Gfx->getTextBounds(_stringDrawBuffer, 0, 0, nullptr, nullptr, &string_width, nullptr);
	Width = string_width + minus_width + 2 + 2 * UiManager.ElementPadding + 2 * UiManager.ElementMargin;
}