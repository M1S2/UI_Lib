/*
 * BoolControl.cpp
 */ 

#include "Controls/BoolControl.h"
#include "Core/UI_Manager.h"

BoolControl::BoolControl(bool* valuePointer, void* controlContext, void(*onValueChanged)(void* controlContext), BoolControlStyles_t style, uint16_t locX, uint16_t locY) : UIElement(locX, locY, UI_CONTROL)
{
	_valuePointer = valuePointer;
	ControlContext = controlContext;
	OnValueChanged = onValueChanged;
	_style = style;
}

void BoolControl::Draw(bool redraw)
{
	if (Visible)
	{
		redraw = redraw || (_lastValueDraw != *_valuePointer) || !_lastDrawnVisible;
		if(redraw)
		{
			_lastDrawnVisible = true;
			_lastValueDraw = *_valuePointer;
			UiManager.Gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);

			switch(_style)
			{
				case BOOLCONTROL_STYLE_TOGGLESWITCH:
				{
					uint16_t sliderCircleHeight = DEFAULT_BOOLCONTROL_TOGGLESWITCH_HEIGHT - 2 * DEFAULT_BOOLCONTROL_TOGGLESWITCH_SLIDER_SPACING;
					if(*_valuePointer)
					{
						// Draw a filled rounded rectangle if the toggle switch is on
						UiManager.Gfx->fillRoundRect(LocX + UiManager.ElementMargin + UiManager.ElementPadding, LocY + UiManager.ElementMargin + UiManager.ElementPadding, DEFAULT_BOOLCONTROL_TOGGLESWITCH_WIDTH, DEFAULT_BOOLCONTROL_TOGGLESWITCH_HEIGHT, (DEFAULT_BOOLCONTROL_TOGGLESWITCH_HEIGHT / 2), UiManager.ColorForeground);

						// Draw the toggle switch slider using the rounded rectangle method (easier to position the circle than using the fillCircle method)
						UiManager.Gfx->fillRoundRect(LocX + Width - UiManager.ElementMargin - UiManager.ElementPadding - DEFAULT_BOOLCONTROL_TOGGLESWITCH_SLIDER_SPACING - sliderCircleHeight, LocY + UiManager.ElementMargin + UiManager.ElementPadding + DEFAULT_BOOLCONTROL_TOGGLESWITCH_SLIDER_SPACING, sliderCircleHeight, sliderCircleHeight, (sliderCircleHeight / 2),  UiManager.ColorBackground);
					}
					else
					{
						// Draw a rounded rectangle if the toggle switch is off
						UiManager.Gfx->drawRoundRect(LocX + UiManager.ElementMargin + UiManager.ElementPadding, LocY + UiManager.ElementMargin + UiManager.ElementPadding, DEFAULT_BOOLCONTROL_TOGGLESWITCH_WIDTH, DEFAULT_BOOLCONTROL_TOGGLESWITCH_HEIGHT, (DEFAULT_BOOLCONTROL_TOGGLESWITCH_HEIGHT / 2), UiManager.ColorForeground);

						// Draw the toggle switch slider using the rounded rectangle method (easier to position the circle than using the fillCircle method)
						UiManager.Gfx->fillRoundRect(LocX + UiManager.ElementMargin + UiManager.ElementPadding + DEFAULT_BOOLCONTROL_TOGGLESWITCH_SLIDER_SPACING, LocY + UiManager.ElementMargin + UiManager.ElementPadding + DEFAULT_BOOLCONTROL_TOGGLESWITCH_SLIDER_SPACING, sliderCircleHeight, sliderCircleHeight, (sliderCircleHeight / 2),  UiManager.ColorForeground);				
					}
					break;
				}
				case BOOLCONTROL_STYLE_CHECKBOX:
				{
					// Draw the outline of the checkbox
					UiManager.Gfx->drawRect(LocX + UiManager.ElementMargin + UiManager.ElementPadding, LocY + UiManager.ElementMargin + UiManager.ElementPadding, DEFAULT_BOOLCONTROL_CHECKBOX_SIZE, DEFAULT_BOOLCONTROL_CHECKBOX_SIZE, UiManager.ColorForeground);

					if(*_valuePointer)
					{
						// Draw the checkbox mark
						UiManager.Gfx->drawLine(LocX + UiManager.ElementMargin + UiManager.ElementPadding + DEFAULT_BOOLCONTROL_CHECKBOX_CHECK_SPACING, LocY + UiManager.ElementMargin + UiManager.ElementPadding + (DEFAULT_BOOLCONTROL_CHECKBOX_SIZE / 2), LocX + UiManager.ElementMargin + UiManager.ElementPadding + (DEFAULT_BOOLCONTROL_CHECKBOX_SIZE / 2), LocY + Height - UiManager.ElementMargin - UiManager.ElementPadding - DEFAULT_BOOLCONTROL_CHECKBOX_CHECK_SPACING, UiManager.ColorForeground);
						UiManager.Gfx->drawLine(LocX + UiManager.ElementMargin + UiManager.ElementPadding + (DEFAULT_BOOLCONTROL_CHECKBOX_SIZE / 2), LocY + Height - UiManager.ElementMargin - UiManager.ElementPadding - DEFAULT_BOOLCONTROL_CHECKBOX_CHECK_SPACING, LocX + Width - UiManager.ElementMargin - UiManager.ElementPadding - DEFAULT_BOOLCONTROL_CHECKBOX_CHECK_SPACING, LocY + UiManager.ElementMargin + UiManager.ElementPadding + DEFAULT_BOOLCONTROL_CHECKBOX_CHECK_SPACING, UiManager.ColorForeground);
					}
					break;
				}
				case BOOLCONTROL_STYLE_CLASSIC:
				{
					UiManager.Gfx->setCursor(LocX + UiManager.ElementMargin + UiManager.ElementPadding, LocY + Height - UiManager.ElementMargin - 2 * UiManager.ElementPadding - 1);
					UiManager.Gfx->print(*_valuePointer ? "ON" : "OFF");
					UiManager.Gfx->drawFastHLine(LocX + UiManager.ElementMargin + 1, LocY + Height - UiManager.ElementMargin - UiManager.ElementPadding, Width - 2 * UiManager.ElementMargin - 2, UiManager.ColorForeground);
					break;
				}
				default: break;
			}
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

bool BoolControl::TouchInput(uint16_t x, uint16_t y, TouchTypes touchType)
{
	if(HitTest(x, y))
	{
		ToggleValue();
		return true;
	}
	return false;
}

void BoolControl::ToggleValue()
{
	(*_valuePointer) = !(*_valuePointer);
	if (OnValueChanged != NULL) { OnValueChanged(ControlContext); }
}

void BoolControl::RecalculateDimensions()
{
	switch(_style)
	{
		case BOOLCONTROL_STYLE_TOGGLESWITCH:
		{
			Width = DEFAULT_BOOLCONTROL_TOGGLESWITCH_WIDTH + 2 * UiManager.ElementPadding + 2 * UiManager.ElementMargin;
			Height = DEFAULT_BOOLCONTROL_TOGGLESWITCH_HEIGHT + 2 * UiManager.ElementPadding + 2 * UiManager.ElementMargin;
			break;
		}
		case BOOLCONTROL_STYLE_CHECKBOX:
		{
			Width = DEFAULT_BOOLCONTROL_CHECKBOX_SIZE + 2 * UiManager.ElementPadding + 2 * UiManager.ElementMargin;
			Height = DEFAULT_BOOLCONTROL_CHECKBOX_SIZE + 2 * UiManager.ElementPadding + 2 * UiManager.ElementMargin;
			break;
		}
		case BOOLCONTROL_STYLE_CLASSIC:
		{
			int16_t x, y;
			uint16_t w, h;
			UiManager.Gfx->getTextBounds("OFF", 0, 0, &x, &y, &w, &h);
			Width = w + 2 * UiManager.ElementPadding + 2 * UiManager.ElementMargin + 2;
			Height = UiManager.FontHeight + 2 * UiManager.ElementPadding + 2 * UiManager.ElementMargin;
			break;
		}
		default: break;
	}
}