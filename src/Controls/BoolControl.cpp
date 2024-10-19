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

void BoolControl::Draw(bool redraw)
{
	if (Visible)
	{
		redraw = redraw || (this->_lastValueDraw != *this->_valuePointer) || !_lastDrawnVisible;
		if(redraw)
		{
			_lastDrawnVisible = true;
			UiManager.Gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);

			BoolIndicator::Draw(redraw);
			UiManager.Gfx->drawFastHLine(LocX + UiManager.ElementMargin + 1, LocY + Height - UiManager.ElementMargin - UiManager.ElementPadding, Width - 2 * UiManager.ElementMargin - 2, UiManager.ColorForeground);
		}
	}
	else if(!Visible && _lastDrawnVisible)		// clear only when the Visible property changes from true to false
	{
		_lastDrawnVisible = false;
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