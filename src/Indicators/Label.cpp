/*
 * Label.cpp
 */ 

#include "Indicators/Label.h"
#include <string.h>

template <int StringLength>
Label<StringLength>::Label(const char* text) : UIElement(UI_INDICATOR)
{
	SetText(text);
	_font = NULL;
	_color = UI_LIB_COLOR_FOREGROUND;
}

template <int StringLength>
Label<StringLength>::Label(uint16_t locX, uint16_t locY, const char* text) : UIElement(locX, locY, UI_INDICATOR)
{
	SetText(text);
	_font = NULL;
	_color = UI_LIB_COLOR_FOREGROUND;
}

template <int StringLength>
Label<StringLength>::Label(const char* text, const GFXfont* font, uint16_t color) : UIElement(UI_INDICATOR)
{
	SetText(text);
	_font = font;
	_color = color;
}

template <int StringLength>
Label<StringLength>::Label(uint16_t locX, uint16_t locY, const char* text, const GFXfont* font, uint16_t color) : UIElement(locX, locY, UI_INDICATOR)
{
	SetText(text);
	_font = font;
	_color = color;
}

template <int StringLength>
void Label<StringLength>::Draw(Adafruit_GFX* gfx, bool wasScreenCleared)
{
	if (Visible)
	{
		gfx->fillRect(LocX, LocY, Width, Height, UI_LIB_COLOR_BACKGROUND);

		int font_y_offset = UI_LIB_DEFAULT_FONT_OFFSET_Y_BASELINE;
		if(_font != NULL) 
		{
			gfx->setFont(_font);
			int16_t x, y;
			uint16_t w, h;
			gfx->getTextBounds("A", 0, 0, &x, &y, &w, &h);
			font_y_offset = h;
		}
		gfx->setCursor(LocX, LocY + font_y_offset);
		gfx->setTextColor(_color);
		gfx->print(Text);
		
		if(_font !=NULL) 
		{ 
			gfx->setFont(UI_LIB_DEFAULT_FONT);
		}
		gfx->setTextColor(UI_LIB_COLOR_FOREGROUND);
	}
	else
	{
		gfx->fillRect(LocX, LocY, Width, Height, UI_LIB_COLOR_BACKGROUND);
	}
}

template <int StringLength>
void Label<StringLength>::SetText(const char* text)
{
	strncpy(Text, text, StringLength);		// Copy a maximum number of StringLength characters to the Text buffer. If text is shorter, the array is zero padded.
	Text[StringLength - 1] = '\0';			// The Text buffer must contain at least one termination character ('\0') at the end to protect from overflow.
}