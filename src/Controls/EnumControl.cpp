/*
 * EnumControl.cpp
 */ 

#include "Controls/EnumControl.h"

template <class T>
EnumControl<T>::EnumControl(unsigned char locX, unsigned char locY, T* valuePointer, const char** enumNames, uint8_t numEnumValues, void* controlContext, void(*onValueChanged)(void* controlContext)) : EnumIndicator<T>(locX, locY, valuePointer, enumNames, numEnumValues)
{
	this->Type = UI_CONTROL;
	_controlContext = controlContext;
	_onValueChanged = onValueChanged;
}

template <class T>
void EnumControl<T>::Draw(Adafruit_GFX* gfx, bool isFirstPage)
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
				
		EnumIndicator<T>::Draw(gfx, isFirstPage);
		
		if(IsEditMode) 
		{ 
			gfx->setTextColor(DEFAULT_UI_ELEMENT_COLOR);
		}
	}
}

template <class T>
bool EnumControl<T>::KeyInput(Keys_t key)
{
	switch (key)
	{
		case KEYUP:
			return NextValue();
		case KEYDOWN:
			return PreviousValue();
		case KEYOK:
			ToggleEditMode();
			return true;
		default:
			return false;
	}
}

template <class T>
bool EnumControl<T>::PreviousValue()
{
	if (IsEditMode)
	{
		if (*this->_valuePointer > 0) { (*((int*)this->_valuePointer))--; }
		else if(*this->_valuePointer == 0) { (*this->_valuePointer) = (T)(this->_numEnumValues - 1); }
			
		if (_onValueChanged != NULL) { _onValueChanged(_controlContext); }
		return true;
	}
	return false;
}

template <class T>
bool EnumControl<T>::NextValue()
{
	if (IsEditMode)
	{
		if(*this->_valuePointer < (this->_numEnumValues - 1)) { (*((int*)this->_valuePointer))++; }
		else if(*this->_valuePointer >= (this->_numEnumValues - 1)) { (*this->_valuePointer) = (T)0; }
		
		if (_onValueChanged != NULL) { _onValueChanged(_controlContext); }
		return true;
	}
	return false;
}

template <class T>
void EnumControl<T>::ToggleEditMode()
{
	IsEditMode = !IsEditMode;
}