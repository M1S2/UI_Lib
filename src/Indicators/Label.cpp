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
	_wasColorSet = false;
}

template <int StringLength>
Label<StringLength>::Label(uint16_t locX, uint16_t locY, const char* text) : UIElement(locX, locY, UI_INDICATOR)
{
	SetText(text);
	_font = NULL;
	_wasColorSet = false;
}

template <int StringLength>
Label<StringLength>::Label(uint16_t locX, uint16_t locY, const char* text, uint16_t color) : UIElement(locX, locY, UI_INDICATOR)
{
	SetText(text);
	_font = NULL;
	_color = color;
	_wasColorSet = true;
}

template <int StringLength>
Label<StringLength>::Label(const char* text, const GFXfont* font, uint16_t color) : UIElement(UI_INDICATOR)
{
	SetText(text);
	_font = font;
	_color = color;
	_wasColorSet = true;
}

template <int StringLength>
Label<StringLength>::Label(const char* text, uint16_t color) : UIElement(UI_INDICATOR)
{
	SetText(text);
	_font = NULL;
	_color = color;
	_wasColorSet = true;
}

template <int StringLength>
Label<StringLength>::Label(uint16_t locX, uint16_t locY, const char* text, const GFXfont* font, uint16_t color) : UIElement(locX, locY, UI_INDICATOR)
{
	SetText(text);
	_font = font;
	_color = color;
	_wasColorSet = true;
}

template <int StringLength>
void Label<StringLength>::Draw(bool redraw)
{
	if(!_wasColorSet)
	{
		// Initialize the color with the foreground color from the UiManager if the color wasn't set in the constructor
		_color = UiManager.ColorForeground;
		_wasColorSet = true;
	}

	if (Visible)
	{
		if(redraw || !_lastDrawnVisible)
		{
			_lastDrawnVisible = true;
			UiManager.Gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);

			int font_y_offset = UiManager.FontHeight - 2 * UiManager.ElementPadding;
			if(_font != NULL) 
			{
				UiManager.Gfx->setFont(_font);
				int16_t x, y;
				uint16_t w, h;
				UiManager.Gfx->getTextBounds("Ag", 0, 0, &x, &y, &w, &h);
				font_y_offset = h;
			}
			UiManager.Gfx->setCursor(LocX, LocY + font_y_offset);
			UiManager.Gfx->setTextColor(_color);
			UiManager.Gfx->print(Text);
			
			if(_font !=NULL) 
			{ 
				UiManager.Gfx->setFont(UiManager.Font);
			}
			UiManager.Gfx->setTextColor(UiManager.ColorForeground);
		}
	}
	else if(!Visible && _lastDrawnVisible)		// clear only when the Visible property changes from true to false
	{
		_lastDrawnVisible = false;
		UiManager.Gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);
	}
}

template <int StringLength>
void Label<StringLength>::SetText(const char* text)
{
	strncpy(Text, text, StringLength);		// Copy a maximum number of StringLength characters to the Text buffer. If text is shorter, the array is zero padded.
	Text[StringLength - 1] = '\0';			// The Text buffer must contain at least one termination character ('\0') at the end to protect from overflow.
}

template <int StringLength>
void Label<StringLength>::RecalculateDimensions()
{
	if(_font != NULL)
	{
		UiManager.Gfx->setFont(_font);
	}
	int16_t x, y;
	uint16_t w, h;
	UiManager.Gfx->getTextBounds(Text, 0, 0, &x, &y, &w, &h);

	Width = w + 2 * UiManager.ElementPadding;
	Height = h + 2 * UiManager.ElementPadding;

	if(_font !=NULL) 
	{ 
		UiManager.Gfx->setFont(UiManager.Font);
	}
}