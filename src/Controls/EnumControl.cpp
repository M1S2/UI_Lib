/*
 * EnumControl.cpp
 */ 

#include "Controls/EnumControl.h"
#include "Core/UI_Manager.h"

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
void EnumControl<T>::Draw(bool redraw)
{
	if (this->Visible)
	{
		redraw = redraw || (this->_lastValueDraw != *this->_valuePointer) || (IsEditMode != _lastDrawnEditMode) || !this->_lastDrawnVisible;
		if(redraw)
		{
			this->_lastDrawnVisible = true;
			_lastDrawnEditMode = IsEditMode;
			UiManager.Gfx->fillRect(this->LocX, this->LocY, this->Width, this->Height, UiManager.ColorBackground);
			
			if (IsEditMode)
			{
				UiManager.Gfx->fillRect(this->LocX + UiManager.ElementMargin, this->LocY + UiManager.ElementMargin, this->Width - 2 * UiManager.ElementMargin, this->Height - 2 * UiManager.ElementMargin, UiManager.ColorForeground);
				UiManager.Gfx->setTextColor(UiManager.ColorForegroundEditMode);
			}	
			else 
			{
				UiManager.Gfx->drawFastHLine(this->LocX + UiManager.ElementMargin + 1, this->LocY + this->Height - UiManager.ElementMargin - UiManager.ElementPadding, this->Width - 2 * UiManager.ElementMargin - 2, UiManager.ColorForeground); 
			}
					
			EnumIndicator<T>::Draw(redraw);
			
			if(IsEditMode) 
			{ 
				// Reset text color back to default foreground
				UiManager.Gfx->setTextColor(UiManager.ColorForeground);
			}
		}
	}
	else if(!this->Visible && this->_lastDrawnVisible)		// clear only when the Visible property changes from true to false
	{
		this->_lastDrawnVisible = false;
		UiManager.Gfx->fillRect(this->LocX, this->LocY, this->Width, this->Height, UiManager.ColorBackground);
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
bool EnumControl<T>::TouchInput(uint16_t x, uint16_t y)
{
	if(this->HitTest(x, y))
	{
		bool touchWasInRightHalf = (x >= (this->LocX + this->Width / 2));
		bool previousEditMode = IsEditMode;
		IsEditMode = true;			// Make sure that the control is set to edit mode (otherwise the NextValue() and PreviousValue() methods don't have any effect)
		if(touchWasInRightHalf)
		{
			NextValue();
		}
		else
		{
			PreviousValue();
		}
		IsEditMode = previousEditMode;
		return true;
	}
	return false;
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