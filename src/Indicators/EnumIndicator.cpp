/*
 * EnumIndicator.cpp
 */ 

#include "Indicators/EnumIndicator.h"

template <class T>
EnumIndicator<T>::EnumIndicator(unsigned char locX, unsigned char locY, T* valuePointer, const char** enumNames, uint8_t numEnumValues) : UIElement(locX, locY, UI_INDICATOR)
{
	_enumNames = enumNames;
	_numEnumValues = numEnumValues;
	_valuePointer = valuePointer;
}

template <class T>
void EnumIndicator<T>::Draw(Adafruit_GFX* gfx, bool isFirstPage)
{
	if (Visible)
	{
		if (isFirstPage) { _valueDraw = *_valuePointer; }
			
		gfx->setCursor(LocX, LocY + DEFAULT_FONT_OFFSET_Y_BASELINE);
		gfx->print(_enumNames[_valueDraw]);
	}
}