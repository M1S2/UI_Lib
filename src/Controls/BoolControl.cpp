/*
 * BoolControl.cpp
 */ 

#include "Controls/BoolControl.h"

BoolControl::BoolControl(bool* valuePointer, void* controlContext, void(*onValueChanged)(void* controlContext)) : BoolIndicator(valuePointer)
{
	Type = UI_CONTROL;
	_controlContext = controlContext;
	_onValueChanged = onValueChanged;
}

BoolControl::BoolControl(uint16_t locX, uint16_t locY, bool* valuePointer, void* controlContext, void(*onValueChanged)(void* controlContext)) : BoolIndicator(locX, locY, valuePointer)
{
	Type = UI_CONTROL;
	_controlContext = controlContext;
	_onValueChanged = onValueChanged;
}

void BoolControl::Draw(Adafruit_GFX* gfx, bool wasScreenCleared)
{
	if (Visible)
	{
		gfx->fillRect(LocX - 1, LocY - 1, Width + 2, Height + 2, UI_LIB_COLOR_BACKGROUND);

		BoolIndicator::Draw(gfx, wasScreenCleared);
		gfx->drawFastHLine(LocX, LocY + Height, Width, UI_LIB_COLOR_FOREGROUND);
	}
	else
	{
		gfx->fillRect(LocX, LocY, Width, Height, UI_LIB_COLOR_BACKGROUND);
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