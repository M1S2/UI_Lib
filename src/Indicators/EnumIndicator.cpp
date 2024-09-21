/*
 * EnumIndicator.cpp
 */ 

#include "Indicators/EnumIndicator.h"

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
void EnumIndicator<T>::Draw(Adafruit_GFX* gfx, bool wasScreenCleared)
{
	if (Visible)
	{
		// If the Draw() is called on a object of the EnumIndicator, the Type is UI_INDICATOR
		// If the Draw() is called from an EnumControl object, the Type was set to UI_CONTROL there
		if(this->Type == UI_INDICATOR)
		{
			gfx->fillRect(LocX, LocY, Width, Height, UI_LIB_COLOR_BACKGROUND);
		}

		_valueDraw = *_valuePointer; 
			
		gfx->setCursor(LocX, LocY + UI_LIB_DEFAULT_FONT_OFFSET_Y_BASELINE);
		gfx->print(_enumNames[_valueDraw]);
	}
	else
	{
		gfx->fillRect(LocX, LocY, Width, Height, UI_LIB_COLOR_BACKGROUND);
	}
}