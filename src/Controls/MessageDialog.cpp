/*
 * MessageDialog.cpp
 */ 

#include "Controls/MessageDialog.h"
#include "../Indicators/Label.cpp"
#include "../Controls/ButtonControl.cpp"
#include "Core/UI_Manager.h"

template <int messageLength>
MessageDialog<messageLength>::MessageDialog(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height, const char* message, MessageSeverity_t severity, MessageButtons_t buttons, void* controlContext, void(*onOkClick)(void* controlContext), void(*onCancelClick)(void* controlContext)) : UIElement(locX, locY, UI_CONTROL),
	_page(),
	_severityIcon(locX, locY, icon_info_width, icon_info_height, (severity == MSG_INFO ? icon_info_bits : (severity == MSG_WARNING ? icon_warning_bits : icon_error_bits)), (severity == MSG_INFO ? MSG_INFO_COLOR : (severity == MSG_WARNING ? MSG_WARNING_COLOR : MSG_ERR_COLOR))),
	_message(locX + icon_info_width + 5, locY, message),
	_buttonOk(locX + width / 2 - 20 - (buttons == MSG_BTN_OK_CANCEL ? 22 : 0), locY + height - DEFAULT_UI_ELEMENT_HEIGHT - 4, DEFAULT_UI_ELEMENT_WIDTH, DEFAULT_UI_ELEMENT_HEIGHT, "OK", controlContext, onOkClick),
	_buttonCancel(locX + width / 2 - 20 + (buttons == MSG_BTN_OK_CANCEL ? 22 : 0), locY + height - DEFAULT_UI_ELEMENT_HEIGHT - 4, DEFAULT_UI_ELEMENT_WIDTH, DEFAULT_UI_ELEMENT_HEIGHT, "Cancel", controlContext, onCancelClick)
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

template <int messageLength>
void MessageDialog<messageLength>::Draw(Adafruit_GFX* gfx)
{
	gfx->fillRect(LocX - 1, LocY - 1, Width + 2, Height + 2, UiManager.ColorBackground);
	_page.Draw(gfx);
}

template <int messageLength>
bool MessageDialog<messageLength>::KeyInput(Keys_t key)
{
	return _page.KeyInput(key);
}