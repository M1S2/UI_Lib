/*
 * NumericControl.cpp
 */

#include "Controls/NumericControl.h"
#include <math.h>

template <class T, int stringBufferLength>
NumericControl<T, stringBufferLength>::NumericControl(uint16_t locX, uint16_t locY, T* valuePointer, const char* baseUnit, T minValue, T maxValue, int numFractionalDigits, void* controlContext, void(*onValueChanged)(void* controlContext)) : NumericIndicator<T, stringBufferLength>(locX, locY, valuePointer, baseUnit, maxValue, numFractionalDigits)
{
	this->Type = UI_CONTROL;
	IsEditMode = false;
	CurrentDigitPosition = 0;
	_minValue = minValue;
	_controlContext = controlContext;
	_onValueChanged = onValueChanged;
}

template <class T, int stringBufferLength>
void NumericControl<T, stringBufferLength>::Draw(Adafruit_GFX* gfx, bool isFirstPage)
{
	if (this->Visible)
	{
		if (IsEditMode)
		{
			gfx->fillRect(this->LocX, this->LocY, this->Width, this->Height, DEFAULT_UI_ELEMENT_COLOR);
			gfx->setTextColor(DEFAULT_UI_ELEMENT_COLOR_CONTRAST);
		}	
		else 
		{
			gfx->drawFastHLine(this->LocX, this->LocY + this->Height, this->Width, DEFAULT_UI_ELEMENT_COLOR); 
		}
		
		NumericIndicator<T, stringBufferLength>::Draw(gfx, isFirstPage);
		
		if(IsEditMode)
		{																								 
			uint8_t cursorDigitIndex = (-CurrentDigitPosition + (this->_numDigits - this->_numFractionalDigits)) + (((this->_numFractionalDigits + this->_unitPrefixPower) == 0 && this->_numFractionalDigits != 0) ? 1 : 0);	// if (this->_numFractionalDigits + this->_unitPrefixPower) == 0,  no comma is available
			uint8_t cursorXpos = this->LocX + cursorDigitIndex * 6 + (CurrentDigitPosition < this->_unitPrefixPower ? 3 : 0) - 1;																								// if (CurrentDigitPosition < _unitPrefixPower) cursor is right of comma
			gfx->drawFastHLine(cursorXpos, this->LocY + this->Height, 5, DEFAULT_UI_ELEMENT_COLOR_CONTRAST);		// Draw cursor

			gfx->setTextColor(DEFAULT_UI_ELEMENT_COLOR);
		}
	}
}

template <class T, int stringBufferLength>
T NumericControl<T, stringBufferLength>::coerceValue(T value)
{
	if (value > this->_maxValue) { value = this->_maxValue; }
	else if (value < _minValue) { value = _minValue; }
	return value;
}

template <class T, int stringBufferLength>
unsigned char NumericControl<T, stringBufferLength>::extractDigit(float number, int8_t position)
{
	float divisor = pow(10, position);
	uint32_t truncated = uint32_t((fabs(number) / divisor) + 0.1f);		// +0.1f not really clean workaround. Is there some rounding problem?
	return truncated % 10;
}


template <class T, int stringBufferLength>
bool NumericControl<T, stringBufferLength>::KeyInput(Keys_t key)
{
	switch (key)
	{
		case KEYLEFT:
			return CursorLeft();
		case KEYRIGHT:
			return CursorRight();
		case KEYUP:
			return ValueUp();
		case KEYDOWN:
			return ValueDown();
		case KEYOK:
			ToggleEditMode();
			return true;
		case KEYKILO:
			return KeyKilo();
		case KEYMILLI:
			return KeyMilli();
		case KEYX1:
			return KeyX1();
		case KEYMINUS:
			return KeyMinus();
		case KEY0:
		case KEY1:
		case KEY2:
		case KEY3:
		case KEY4:
		case KEY5:
		case KEY6:
		case KEY7:
		case KEY8:
		case KEY9:
			return KeyNumeric(key);
		default:
			return false;
	}
}

template <class T, int stringBufferLength>
bool NumericControl<T, stringBufferLength>::KeyKilo()
{
	if (IsEditMode)
	{
		float multiplicator = pow(10, 3 - this->_unitPrefixPower);
		(*this->_valuePointer) = coerceValue((*this->_valuePointer) * multiplicator);
		
		if (_onValueChanged != NULL && multiplicator != 1) { _onValueChanged(_controlContext); }
		return true;
	}
	return false;
}

template <class T, int stringBufferLength>
bool NumericControl<T, stringBufferLength>::KeyMilli()
{
	if (IsEditMode)
	{
		float multiplicator = pow(10, -3 - this->_unitPrefixPower);
		(*this->_valuePointer) = coerceValue((*this->_valuePointer) * multiplicator);
		
		if (_onValueChanged != NULL && multiplicator != 1) { _onValueChanged(_controlContext); }
		return true;
	}
	return false;
}

template <class T, int stringBufferLength>
bool NumericControl<T, stringBufferLength>::KeyX1()
{
	if (IsEditMode)
	{
		float multiplicator = pow(10, 0 - this->_unitPrefixPower);
		(*this->_valuePointer) = coerceValue((*this->_valuePointer) * multiplicator);
		
		if (_onValueChanged != NULL && multiplicator != 1) { _onValueChanged(_controlContext); }
		return true;
	}
	return false;
}

template <class T, int stringBufferLength>
bool NumericControl<T, stringBufferLength>::KeyMinus()
{
	if (IsEditMode)
	{
		(*this->_valuePointer) = coerceValue(-(*this->_valuePointer));
		
		if (_onValueChanged != NULL && (*this->_valuePointer) != 0) { _onValueChanged(_controlContext); }
		return true;
	}
	return false;
}

template <class T, int stringBufferLength>
bool NumericControl<T, stringBufferLength>::KeyNumeric(Keys_t key)
{
	if (IsEditMode)
	{
		T oldValue = *this->_valuePointer;
		
		unsigned char keyNum = Keys_GetKeyNumInt(key);
		if(keyNum < 0 || keyNum > 9) { return false; }		// if the keyNum isn't in the range 0..9, the given key is no numeric key
			
		unsigned char digit = extractDigit(oldValue, CurrentDigitPosition);
		float multiplicator = pow(10, CurrentDigitPosition);
		(*this->_valuePointer) = coerceValue(oldValue - (digit * multiplicator) + (keyNum * multiplicator));
		if (CurrentDigitPosition > -this->_numFractionalDigits) { CurrentDigitPosition--; }	// Move cursor right

		if (_onValueChanged != NULL && oldValue != (*this->_valuePointer)) { _onValueChanged(_controlContext); }
		return true;
	}
	return false;
}

template <class T, int stringBufferLength>
bool NumericControl<T, stringBufferLength>::ValueUp()
{
	if (IsEditMode)
	{
		float deltaValue = pow(10, CurrentDigitPosition);
		(*this->_valuePointer) = coerceValue((*this->_valuePointer) + deltaValue);
		
		if (_onValueChanged != NULL && deltaValue != 0) { _onValueChanged(_controlContext); }
		return true;
	}
	return false;
}

template <class T, int stringBufferLength>
bool NumericControl<T, stringBufferLength>::ValueDown()
{
	if (IsEditMode)
	{
		float deltaValue = pow(10, CurrentDigitPosition);
		(*this->_valuePointer) = coerceValue((*this->_valuePointer) - deltaValue);
		
		if (_onValueChanged != NULL && deltaValue != 0) { _onValueChanged(_controlContext); }
		return true;
	}
	return false;
}

template <class T, int stringBufferLength>
bool NumericControl<T, stringBufferLength>::CursorLeft()
{
	if (IsEditMode)
	{
		if (CurrentDigitPosition < (this->_numDigits - this->_numFractionalDigits - 1))
		{
			CurrentDigitPosition++;
		}
		return true;
	}
	return false;
}

template <class T, int stringBufferLength>
bool NumericControl<T, stringBufferLength>::CursorRight()
{
	if (IsEditMode)
	{
		if (CurrentDigitPosition > -this->_numFractionalDigits)
		{
			CurrentDigitPosition--;
		}
		return true;
	}
	return false;
}

template <class T, int stringBufferLength>
void NumericControl<T, stringBufferLength>::ToggleEditMode()
{
	IsEditMode = !IsEditMode;
}