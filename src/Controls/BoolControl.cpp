/*
 * BoolControl.cpp
 */ 

#include "Controls/BoolControl.h"

BoolControl::BoolControl(unsigned char locX, unsigned char locY, bool* valuePointer, void* controlContext, void(*onValueChanged)(void* controlContext)) : BoolIndicator(locX, locY, valuePointer)
{
	Type = UI_CONTROL;
	_controlContext = controlContext;
	_onValueChanged = onValueChanged;
}

void BoolControl::Draw(Adafruit_GFX* gfx, bool isFirstPage)
{
	if (Visible)
	{
		BoolIndicator::Draw(gfx, isFirstPage);
		gfx->drawFastHLine(LocX, LocY + Height, Width, DEFAULT_UI_ELEMENT_COLOR);
	}
}

bool BoolControl::KeyInput(Keys_t key)
{
	switch (key)
	{
		case KEYOK:
			ToggleValue();
			return true;
		default:
			return false;
	}
}

void BoolControl::ToggleValue()
{
	(*_valuePointer) = !(*_valuePointer);
	if (_onValueChanged != NULL) { _onValueChanged(_controlContext); }
}