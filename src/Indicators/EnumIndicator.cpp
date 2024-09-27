/*
 * EnumIndicator.cpp
 */ 

#include "Indicators/EnumIndicator.h"
#include "Core/UI_Manager.h"

template <class T>
EnumIndicator<T>::EnumIndicator(T* valuePointer, const char** enumNames, uint8_t numEnumValues) : UIElement(UI_INDICATOR)
{
	_enumNames = enumNames;
	_numEnumValues = numEnumValues;
	_valuePointer = valuePointer;
}

template <class T>
EnumIndicator<T>::EnumIndicator(uint16_t locX, uint16_t locY, T* valuePointer, const char** enumNames, uint8_t numEnumValues) : UIElement(locX, locY, UI_INDICATOR)
{
	_enumNames = enumNames;
	_numEnumValues = numEnumValues;
	_valuePointer = valuePointer;
}

template <class T>
void EnumIndicator<T>::Draw(Adafruit_GFX* gfx)
{
	if (Visible)
	{
		// If the Draw() is called on a object of the EnumIndicator, the Type is UI_INDICATOR
		// If the Draw() is called from an EnumControl object, the Type was set to UI_CONTROL there
		if(this->Type == UI_INDICATOR)
		{
			gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);
		}

		_valueDraw = *_valuePointer; 
			
		gfx->setCursor(LocX, LocY + UiManager.FontHeight - 4);
		gfx->print(_enumNames[_valueDraw]);
	}
	else
	{
		gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);
	}
}