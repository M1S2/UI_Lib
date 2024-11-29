/*
 * NumericControl.cpp
 */

#include "Controls/NumericControl.h"
#include "Core/UI_Manager.h"
#include "Core/UI_Icons.h"
#include <math.h>

template <class T>
NumericControl<T>::NumericControl(T* valuePointer, const char* baseUnit, T minValue, T maxValue, int numFractionalDigits, void* controlContext, void(*onValueChanged)(void* controlContext), VirtualKeys* virtualKeys, uint16_t locX, uint16_t locY, uint8_t maxStringBufferLength) : NumericIndicator<T>(valuePointer, baseUnit, maxValue, numFractionalDigits, locX, locY, maxStringBufferLength)
{
	this->Type = UI_CONTROL;
	_lastDrawnEditMode = false;
	CurrentDigitPosition = 0;
	_minValue = minValue;
	_controlContext = controlContext;
	_onValueChanged = onValueChanged;
	_virtualKeys = virtualKeys;
}

template <class T>
void NumericControl<T>::Draw(bool redraw)
{
	if (this->Visible)
	{
		redraw = redraw || (this->_lastValueDraw != *this->_valuePointer) || (this->IsInEditMode != _lastDrawnEditMode) || (CurrentDigitPosition != _lastDrawnCurrentDigitPosition) || !this->_lastDrawnVisible;
		if(redraw)
		{
			this->_lastDrawnVisible = true;
			_lastDrawnEditMode = this->IsInEditMode;
			_lastDrawnCurrentDigitPosition = CurrentDigitPosition;
			UiManager.Gfx->fillRect(this->LocX, this->LocY, this->Width, this->Height, UiManager.ColorBackground);

			if (this->IsInEditMode)
			{
				UiManager.Gfx->fillRect(this->LocX + UiManager.ElementMargin, this->LocY + UiManager.ElementMargin, this->Width - 2 * UiManager.ElementMargin, this->Height - 2 * UiManager.ElementMargin, UiManager.ColorForeground);
				UiManager.Gfx->setTextColor(UiManager.ColorForegroundEditMode);
			}	
			else
			{
				UiManager.Gfx->drawFastHLine(this->LocX + UiManager.ElementMargin + 1, this->LocY + this->Height - UiManager.ElementMargin - UiManager.ElementPadding, this->Width - 2 * UiManager.ElementMargin - 2, UiManager.ColorForeground); 
			}
			
			NumericIndicator<T>::Draw(redraw);
			
			if(this->IsInEditMode)
			{	
				uint16_t character_width, character_space_width, dot_width, minus_width;
				UiManager.Gfx->getTextBounds("0", 0, 0, nullptr, nullptr, &character_width, nullptr);
				UiManager.Gfx->getTextBounds("00", 0, 0, nullptr, nullptr, &character_space_width, nullptr);
				character_space_width -= 2* character_width;	// subtract the with of both "0" characters to get only the space between the characters
				UiManager.Gfx->getTextBounds(".", 0, 0, nullptr, nullptr, &dot_width, nullptr);
				UiManager.Gfx->getTextBounds("-", 0, 0, nullptr, nullptr, &minus_width, nullptr);
				dot_width += 2 * character_space_width;			// some space between the dot and the characters

				uint8_t cursorDigitIndex = (-CurrentDigitPosition + (this->_numDigits - this->_numFractionalDigits)) + (((this->_numFractionalDigits + this->_unitPrefixPower) == 0 && this->_numFractionalDigits != 0) ? 1 : 0) - 1;	// if (this->_numFractionalDigits + this->_unitPrefixPower) == 0,  no comma is available
				uint16_t cursorXpos = this->LocX + minus_width + 2 + character_space_width + UiManager.ElementMargin + UiManager.ElementPadding + cursorDigitIndex * (character_width + character_space_width) + (CurrentDigitPosition < this->_unitPrefixPower ? dot_width : 0) - 1;																								// if (CurrentDigitPosition < _unitPrefixPower) cursor is right of comma

				UiManager.Gfx->fillRect(cursorXpos, this->LocY + this->Height - UiManager.ElementMargin - UiManager.ElementPadding, character_width, 2, UiManager.ColorForegroundEditMode);		// Draw cursor

				// Reset text color back to default foreground
				UiManager.Gfx->setTextColor(UiManager.ColorForeground);

				if(_virtualKeys != NULL && !UiManager.AreVirtualKeysShown)
				{
					UiManager.Gfx->drawXBitmap(this->LocX + this->Width - 2 * UiManager.ElementMargin - UiManager.ElementPadding - icon_pencil_width, this->LocY + (this->Height / 2) - (icon_pencil_height / 2), icon_pencil_bits, icon_pencil_width, icon_pencil_height, UiManager.ColorForegroundEditMode);
				}
			}
		}
	}
	else if(!this->Visible && this->_lastDrawnVisible)		// clear only when the Visible property changes from true to false
	{
		this->_lastDrawnVisible = false;
		UiManager.Gfx->fillRect(this->LocX, this->LocY, this->Width, this->Height, UiManager.ColorBackground);
	}
}

template <class T>
T NumericControl<T>::coerceValue(T value)
{
	if (value > this->_maxValue) { value = this->_maxValue; }
	else if (value < _minValue) { value = _minValue; }
	return value;
}

template <class T>
uint8_t NumericControl<T>::extractDigit(T number, int8_t position)
{
	float divisor = pow(10, position);
	uint32_t truncated = uint32_t((abs(number) / divisor) + 0.1f);		// +0.1f not really clean workaround. Is there some rounding problem?
	return truncated % 10;
}


template <class T>
bool NumericControl<T>::KeyInput(Keys_t key)
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

template <class T>
bool NumericControl<T>::TouchInput(uint16_t x, uint16_t y, TouchTypes touchType)
{
	if(this->HitTest(x, y))
	{
		switch (touchType)
		{
			case TOUCH_NORMAL:
			{
				bool touchWasOnEditIcon = this->IsInEditMode && (_virtualKeys != NULL) && !UiManager.AreVirtualKeysShown && (x >= (this->LocX + (this->Width / 2)));
				if(touchWasOnEditIcon)
				{
					_virtualKeys->ShowVirtualKeys(this);
					return true;
				}
				return false;
			}
			case TOUCH_LONG:
			{
				ToggleEditMode();
				return true;
			}
			default: break;
		}
	}
	return false;
}

template <class T>
bool NumericControl<T>::KeyKilo()
{
	if (this->IsInEditMode)
	{
		float multiplicator = pow(10, 3 - this->_unitPrefixPower);
		(*this->_valuePointer) = coerceValue((*this->_valuePointer) * multiplicator);
		
		if (_onValueChanged != NULL && multiplicator != 1) { _onValueChanged(_controlContext); }
		return true;
	}
	return false;
}

template <class T>
bool NumericControl<T>::KeyMilli()
{
	if (this->IsInEditMode)
	{
		float multiplicator = pow(10, -3 - this->_unitPrefixPower);
		(*this->_valuePointer) = coerceValue((*this->_valuePointer) * multiplicator);
		
		if (_onValueChanged != NULL && multiplicator != 1) { _onValueChanged(_controlContext); }
		return true;
	}
	return false;
}

template <class T>
bool NumericControl<T>::KeyX1()
{
	if (this->IsInEditMode)
	{
		float multiplicator = pow(10, 0 - this->_unitPrefixPower);
		(*this->_valuePointer) = coerceValue((*this->_valuePointer) * multiplicator);
		
		if (_onValueChanged != NULL && multiplicator != 1) { _onValueChanged(_controlContext); }
		return true;
	}
	return false;
}

template <class T>
bool NumericControl<T>::KeyMinus()
{
	if (this->IsInEditMode)
	{
		(*this->_valuePointer) = coerceValue(-(*this->_valuePointer));
		
		if (_onValueChanged != NULL && (*this->_valuePointer) != 0) { _onValueChanged(_controlContext); }
		return true;
	}
	return false;
}

template <class T>
bool NumericControl<T>::KeyNumeric(Keys_t key)
{
	if (this->IsInEditMode)
	{
		T oldValue = *this->_valuePointer;
		
		uint8_t keyNum = Keys_GetKeyNumInt(key);
		if(keyNum < 0 || keyNum > 9) { return false; }		// if the keyNum isn't in the range 0..9, the given key is no numeric key
			
		uint8_t digit = extractDigit(oldValue, CurrentDigitPosition);
		float multiplicator = pow(10, CurrentDigitPosition) * (oldValue < 0 ? -1.0f : 1.0f);
		(*this->_valuePointer) = coerceValue(oldValue - (digit * multiplicator) + (keyNum * multiplicator));
		if (CurrentDigitPosition > -this->_numFractionalDigits) { CurrentDigitPosition--; }	// Move cursor right

		if (_onValueChanged != NULL && oldValue != (*this->_valuePointer)) { _onValueChanged(_controlContext); }
		return true;
	}
	return false;
}

template <class T>
bool NumericControl<T>::ValueUp()
{
	if (this->IsInEditMode)
	{
		float deltaValue = pow(10, CurrentDigitPosition);
		(*this->_valuePointer) = coerceValue((*this->_valuePointer) + deltaValue);
		
		if (_onValueChanged != NULL && deltaValue != 0) { _onValueChanged(_controlContext); }
		return true;
	}
	return false;
}

template <class T>
bool NumericControl<T>::ValueDown()
{
	if (this->IsInEditMode)
	{
		float deltaValue = pow(10, CurrentDigitPosition);
		(*this->_valuePointer) = coerceValue((*this->_valuePointer) - deltaValue);
		
		if (_onValueChanged != NULL && deltaValue != 0) { _onValueChanged(_controlContext); }
		return true;
	}
	return false;
}

template <class T>
bool NumericControl<T>::CursorLeft()
{
	if (this->IsInEditMode)
	{
		if (CurrentDigitPosition < (this->_numDigits - this->_numFractionalDigits - 1))
		{
			CurrentDigitPosition++;
		}
		return true;
	}
	return false;
}

template <class T>
bool NumericControl<T>::CursorRight()
{
	if (this->IsInEditMode)
	{
		if (CurrentDigitPosition > -this->_numFractionalDigits)
		{
			CurrentDigitPosition--;
		}
		return true;
	}
	return false;
}

template <class T>
void NumericControl<T>::ToggleEditMode()
{
	UiManager.UpdateIsInEditModeElement(this, !this->IsInEditMode);
}

template <class T>
void NumericControl<T>::RecalculateDimensions()
{
	NumericIndicator<T>::RecalculateDimensions();

	if(_virtualKeys != NULL)
	{
		// If the NumericControl has a virtual keyboard assigned, reserve place for a region to show an edit icon
		this->Width += icon_pencil_width;
	}
}