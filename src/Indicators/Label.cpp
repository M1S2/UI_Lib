/*
 * Label.cpp
 */ 

#include "Indicators/Label.h"
#include <string.h>

Label::Label(const char* text, uint16_t color, const GFXfont* font, uint16_t locX, uint16_t locY, uint16_t maxStringLength) : UIElement(locX, locY, UI_INDICATOR)
{
	Text = new char[maxStringLength];
	_maxStringLength = maxStringLength;
	SetText(text);
	_color = color;
	_font = font;
}

void Label::Draw(bool redraw)
{
	if (Visible)
	{
		if(redraw || !_lastDrawnVisible || WasTextChangedSinceLastDraw)
		{
			_lastDrawnVisible = true;

			UiManager.Gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);

			if(WasTextChangedSinceLastDraw)
			{
				RecalculateDimensions();
			}
			WasTextChangedSinceLastDraw = false;

			int font_y_offset = UiManager.FontHeight - 2 * UiManager.ElementPadding;
			if(_font != NULL) 
			{
				UiManager.Gfx->setFont(_font);
				int16_t x, y;
				uint16_t w, h;
				UiManager.Gfx->getTextBounds("Ag", 0, 0, &x, &y, &w, &h);
				font_y_offset = h;
			}
			UiManager.Gfx->setCursor(LocX + UiManager.ElementMargin, LocY + UiManager.ElementMargin + font_y_offset);
			UiManager.Gfx->setTextColor((_color == LABEL_COLOR_NOTSET ? UiManager.ColorForeground : _color));
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

void Label::SetText(const char* text)
{
	strncpy(Text, text, _maxStringLength);	// Copy a maximum number of StringLength characters to the Text buffer. If text is shorter, the array is zero padded.
	Text[_maxStringLength - 1] = '\0';		// The Text buffer must contain at least one termination character ('\0') at the end to protect from overflow.
	WasTextChangedSinceLastDraw = true;
}

void Label::RecalculateDimensions()
{
	if(_font != NULL)
	{
		UiManager.Gfx->setFont(_font);
	}
	int16_t x, y;
	uint16_t w, h;
	UiManager.Gfx->getTextBounds(Text, 0, 0, &x, &y, &w, &h);

	Width = w + 2 * UiManager.ElementPadding + 2 * UiManager.ElementMargin;
	Height = h + 2 * UiManager.ElementPadding + 2 * UiManager.ElementMargin;

	if(_font !=NULL) 
	{ 
		UiManager.Gfx->setFont(UiManager.Font);
	}
}