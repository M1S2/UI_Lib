/*
 * EnumIndicator.cpp
 */ 

#include "Indicators/EnumIndicator.h"
#include "Core/UI_Manager.h"

template <class T>
EnumIndicator<T>::EnumIndicator(T* valuePointer, const char** enumNames, uint8_t numEnumValues, uint16_t locX, uint16_t locY) : UIElement(locX, locY, UI_INDICATOR)
{
	_enumNames = enumNames;
	_numEnumValues = numEnumValues;
	_valuePointer = valuePointer;
}

template <class T>
void EnumIndicator<T>::Draw(bool redraw)
{
	if (Visible)
	{
		if (_lastValueDraw != *_valuePointer || redraw || !_lastDrawnVisible)
		{
			_lastDrawnVisible = true;
			_lastValueDraw = *_valuePointer;

			// If the Draw() is called on a object of the EnumIndicator, the Type is UI_INDICATOR
			// If the Draw() is called from an EnumControl object, the Type was set to UI_CONTROL there
			if(Type == UI_INDICATOR)
			{
				UiManager.Gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);
			}
				
			UiManager.Gfx->setCursor(LocX + UiManager.ElementMargin + UiManager.ElementPadding, LocY + Height - UiManager.ElementMargin - 2 * UiManager.ElementPadding - 1);
			UiManager.Gfx->print(_enumNames[_lastValueDraw]);
		}
	}
	else if(!Visible && _lastDrawnVisible)		// clear only when the Visible property changes from true to false
	{
		_lastDrawnVisible = false;
		UiManager.Gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);
	}
}

template <class T>
void EnumIndicator<T>::RecalculateDimensions()
{
	Height = UiManager.FontHeight + 2 * UiManager.ElementPadding + 2 * UiManager.ElementMargin;
	
	uint16_t maxWidth = 0;
	for(int i = 0; i < _numEnumValues; i++)
	{
		int16_t x, y;
		uint16_t w, h;
		UiManager.Gfx->getTextBounds(_enumNames[i], 0, 0, &x, &y, &w, &h);
		if(w > maxWidth) { maxWidth = w; }
	}
	
	Width = maxWidth + 2 * UiManager.ElementPadding + 2 * UiManager.ElementMargin + 2; 
}