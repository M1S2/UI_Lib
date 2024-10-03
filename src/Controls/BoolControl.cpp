/*
 * BoolControl.cpp
 */ 

#include "Controls/BoolControl.h"
#include "Core/UI_Manager.h"

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

void BoolControl::Draw()
{
	if (Visible)
	{
		UiManager.Gfx->fillRect(LocX - 1, LocY - 1, Width + 2, Height + 2, UiManager.ColorBackground);

		BoolIndicator::Draw();
		UiManager.Gfx->drawFastHLine(LocX, LocY + Height, Width, UiManager.ColorForeground);
	}
	else
	{
		UiManager.Gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);
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