/*
 * NumericControl.cpp
 */

#include "Controls/NumericControl.h"
#include "Core/UI_Manager.h"
#include <math.h>

template <class T, int stringBufferLength>
NumericControl<T, stringBufferLength>::NumericControl(T* valuePointer, const char* baseUnit, T minValue, T maxValue, int numFractionalDigits, void* controlContext, void(*onValueChanged)(void* controlContext)) : NumericIndicator<T, stringBufferLength>(valuePointer, baseUnit, maxValue, numFractionalDigits)
{
	this->Type = UI_CONTROL;
	IsEditMode = false;
	_lastDrawnEditMode = false;
	CurrentDigitPosition = 0;
	_minValue = minValue;
	_controlContext = controlContext;
	_onValueChanged = onValueChanged;
}

template <class T, int stringBufferLength>
NumericControl<T, stringBufferLength>::NumericControl(uint16_t locX, uint16_t locY, T* valuePointer, const char* baseUnit, T minValue, T maxValue, int numFractionalDigits, void* controlContext, void(*onValueChanged)(void* controlContext)) : NumericIndicator<T, stringBufferLength>(locX, locY, valuePointer, baseUnit, maxValue, numFractionalDigits)
{
	this->Type = UI_CONTROL;
	IsEditMode = false;
	_lastDrawnEditMode = false;
	CurrentDigitPosition = 0;
	_minValue = minValue;
	_controlContext = controlContext;
	_onValueChanged = onValueChanged;
}

template <class T, int stringBufferLength>
void NumericControl<T, stringBufferLength>::Draw(bool redraw)
{
	if (this->Visible)
	{
		redraw = redraw || (this->_lastValueDraw != *this->_valuePointer) || (IsEditMode != _lastDrawnEditMode) || (CurrentDigitPosition != _lastDrawnCurrentDigitPosition) || !this->_lastDrawnVisible;
		if(redraw)
		{
			this->_lastDrawnVisible = true;
			_lastDrawnEditMode = IsEditMode;
			_lastDrawnCurrentDigitPosition = CurrentDigitPosition;
			UiManager.Gfx->fillRect(this->LocX - 1, this->LocY - 1, this->Width + 2, this->Height + 2, UiManager.ColorBackground);

			if (IsEditMode)
			{
				UiManager.Gfx->fillRect(this->LocX, this->LocY, this->Width, this->Height, UiManager.ColorForeground);
				UiManager.Gfx->setTextColor(UiManager.ColorForegroundEditMode);
			}	
			else 
			{
				UiManager.Gfx->drawFastHLine(this->LocX + 1, this->LocY + this->Height - 1, this->Width - 2, UiManager.ColorForeground); 
			}
			
			NumericIndicator<T, stringBufferLength>::Draw(redraw);
			
			if(IsEditMode)
			{	
				uint16_t character_width, dot_width;
				UiManager.Gfx->getTextBounds("0", 0, 0, nullptr, nullptr, &character_width, nullptr);
				UiManager.Gfx->getTextBounds(".", 0, 0, nullptr, nullptr, &dot_width, nullptr);
				character_width += 2;	// some space between the characters
				dot_width += 4;			// some space between the dot and the characters

				uint8_t cursorDigitIndex = (-CurrentDigitPosition + (this->_numDigits - this->_numFractionalDigits)) + (((this->_numFractionalDigits + this->_unitPrefixPower) == 0 && this->_numFractionalDigits != 0) ? 1 : 0) - 1;	// if (this->_numFractionalDigits + this->_unitPrefixPower) == 0,  no comma is available
				uint16_t cursorXpos = this->LocX + 5 + cursorDigitIndex * character_width + (CurrentDigitPosition < this->_unitPrefixPower ? dot_width : 0) - 1;																								// if (CurrentDigitPosition < _unitPrefixPower) cursor is right of comma

				UiManager.Gfx->fillRect(cursorXpos, this->LocY + this->Height - 1, character_width, 2, UiManager.ColorForegroundEditMode);		// Draw cursor

				// Reset text color back to default foreground
				UiManager.Gfx->setTextColor(UiManager.ColorForeground);
			}
		}
	}
	else if(!this->Visible && this->_lastDrawnVisible)		// clear only when the Visible property changes from true to false
	{
		this->_lastDrawnVisible = false;
		UiManager.Gfx->fillRect(this->LocX, this->LocY, this->Width, this->Height, UiManager.ColorBackground);
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