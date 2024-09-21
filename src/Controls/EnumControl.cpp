/*
 * EnumControl.cpp
 */ 

#include "Controls/EnumControl.h"

template <class T>
EnumControl<T>::EnumControl(T* valuePointer, const char** enumNames, uint8_t numEnumValues, void* controlContext, void(*onValueChanged)(void* controlContext)) : EnumIndicator<T>(valuePointer, enumNames, numEnumValues)
{
	this->Type = UI_CONTROL;
	_controlContext = controlContext;
	_onValueChanged = onValueChanged;
}

template <class T>
EnumControl<T>::EnumControl(uint16_t locX, uint16_t locY, T* valuePointer, const char** enumNames, uint8_t numEnumValues, void* controlContext, void(*onValueChanged)(void* controlContext)) : EnumIndicator<T>(locX, locY, valuePointer, enumNames, numEnumValues)
{
	this->Type = UI_CONTROL;
	_controlContext = controlContext;
	_onValueChanged = onValueChanged;
}

template <class T>
void EnumControl<T>::Draw(Adafruit_GFX* gfx, bool wasScreenCleared)
{
	if (this->Visible)
	{
		gfx->fillRect(this->LocX - 1, this->LocY - 1, this->Width + 2, this->Height + 2, UI_LIB_COLOR_BACKGROUND);
		
		if (IsEditMode)
		{
			gfx->fillRect(this->LocX, this->LocY, this->Width, this->Height, UI_LIB_COLOR_FOREGROUND);
			gfx->setTextColor(UI_LIB_COLOR_CONTRAST);
		}	
		else 
		{
			gfx->drawFastHLine(this->LocX, this->LocY + this->Height, this->Width, UI_LIB_COLOR_FOREGROUND); 
		}
				
		EnumIndicator<T>::Draw(gfx, wasScreenCleared);
		
		if(IsEditMode) 
		{ 
			gfx->setTextColor(UI_LIB_COLOR_FOREGROUND);
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