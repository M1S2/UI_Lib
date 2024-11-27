/*
 * MessageDialog.cpp
 */ 

#include "Controls/MessageDialog.h"
#include "Core/UI_Manager.h"

MessageDialog::MessageDialog(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height, const char* message, MessageSeverity_t severity, MessageButtons_t buttons, void* controlContext, void(*onOkClick)(void* controlContext), void(*onCancelClick)(void* controlContext), uint16_t maxMsgLength) : UIElement(locX, locY, UI_CONTROL),
	_page(5),
	_severityIcon(icon_info_width, icon_info_height, (severity == MSG_INFO ? icon_info_bits : (severity == MSG_WARNING ? icon_warning_bits : icon_error_bits)), (severity == MSG_INFO ? MSG_INFO_COLOR : (severity == MSG_WARNING ? MSG_WARNING_COLOR : MSG_ERR_COLOR)), locX, locY),
	_message(message, LABEL_COLOR_NOTSET, NULL, locX + icon_info_width + 5, locY, maxMsgLength),
	_buttonOk(locX + width / 2 - 20 - (buttons == MSG_BTN_OK_CANCEL ? 22 : 0), locY + height - DEFAULT_MSG_BUTTON_HEIGHT - 6, 0, 0, "OK", controlContext, onOkClick, 3),
	_buttonCancel(locX + width / 2 - 20 + (buttons == MSG_BTN_OK_CANCEL ? 22 : 0), locY + height - DEFAULT_MSG_BUTTON_HEIGHT - 6, 0, 0, "Cancel", controlContext, onCancelClick, 7)
{
	Width = width;
	Height = height;	
	_severity = severity;
	_page.AddItem(&_severityIcon);
	_page.AddItem(&_message);

	if (buttons == MSG_BTN_OK || buttons == MSG_BTN_OK_CANCEL) 
	{
		_buttonOk.Visible = true;
		_page.AddItem(&_buttonOk); 
	}
	if (buttons == MSG_BTN_OK_CANCEL)
	{
		_buttonCancel.Visible = true;
		_page.AddItem(&_buttonCancel);
	}

	_page.Parent = this;
	_page.InitItems();
	ActiveChild = &_page;
}

void MessageDialog::Draw(bool redraw)
{
	if(redraw)
	{
		UiManager.Gfx->fillRect(LocX - 1, LocY - 1, Width + 2, Height + 2, UiManager.ColorBackground);
		UiManager.Gfx->setTextWrap(true);
		_page.Draw(redraw);
		UiManager.Gfx->setTextWrap(false);
	}
}

bool MessageDialog::KeyInput(Keys_t key)
{
	return _page.KeyInput(key);
}

bool MessageDialog::TouchInput(uint16_t x, uint16_t y, TouchTypes touchType)
{
	if(HitTest(x, y))
	{
		return _page.TouchInput(x, y, touchType);
	}
	return false;
}

void MessageDialog::RecalculateDimensions()
{
	_buttonOk.RecalculateDimensions();
	_buttonCancel.RecalculateDimensions();
	_page.RecalculateDimensions();
}

void MessageDialog::RecalculateLayout()
{
	_page.RecalculateLayout();
}