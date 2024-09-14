/*
 * TabControl.cpp
 */ 

#include "Controls/TabControl.h"
#include <string.h>

TabControl::TabControl(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, unsigned char tabWidth, void* controlContext, void(*onSelectedTabChanged)(void* controlContext)) : UIElement(locX, locY, UI_CONTROL)
{
	Width = width;
	Height = height;
	_numTabs = 0;
	_selectedTabIndex = 0;
	_tabWidth = tabWidth;
	_controlContext = controlContext;
	_onSelectedTabChanged = onSelectedTabChanged;
}

void TabControl::Draw(Adafruit_GFX* gfx, bool isFirstPage) 
{
	if (Visible)
	{
		gfx->drawRect(LocX + _tabWidth - 1, LocY, Width - _tabWidth + 1, Height, DEFAULT_UI_ELEMENT_COLOR);
		int yTab = LocY;
		
		int16_t x, y;
		uint16_t w, h;
		gfx->getTextBounds("A", 0, 0, &x, &y, &w, &h);
		int tabFontHeight = h;
		int tabHeight = tabFontHeight + 8;
		
		for(int i = 0; i < _numTabs; i++)
		{
			if(i == _selectedTabIndex)
			{
				gfx->drawRect(LocX, yTab, _tabWidth, tabHeight, DEFAULT_UI_ELEMENT_COLOR);
				gfx->drawFastVLine(LocX + _tabWidth - 1, yTab + 1, tabHeight - 2, DEFAULT_UI_ELEMENT_COLOR_CONTRAST);
			}
			
			if(_headers[i] != NULL) 
			{
				gfx->setCursor(LocX + 2, yTab + ((tabHeight - tabFontHeight) / 2) + DEFAULT_FONT_OFFSET_Y_BASELINE);
				gfx->print(_headers[i]);
			}
			yTab+=(tabHeight + TABCONTROL_TABPAGE_MARGIN);
		}
		
		if(_tabContents[_selectedTabIndex] != NULL) { _tabContents[_selectedTabIndex]->Draw(gfx, isFirstPage); }
	}
}

bool TabControl::KeyInput(Keys_t key)
{
	switch (key)
	{
		case KEYLEFT:
			PreviousTab();
			return true;
		case KEYRIGHT:
			NextTab();
			return true;
		default:
			return false;
	}
}

void TabControl::AddTab(const char* header, UIElement* tabContent)
{
	if (_numTabs >= MAX_TABCONTROL_TABS) { return; }

	tabContent->Parent = this;
	_tabContents[_numTabs] = tabContent;
	strncpy(_headers[_numTabs], header, MAX_HEADER_LENGTH);		// Copy a maximum number of StringLength characters to the _header buffer. If text is shorter, the array is zero padded.
	_headers[_numTabs][MAX_HEADER_LENGTH - 1] = '\0';			// The _header buffer must contain at least one termination character ('\0') at the end to protect from overflow.
	_numTabs++;

	if (ActiveChild == NULL) { ActiveChild = tabContent; }
}

void TabControl::NextTab()
{
	_selectedTabIndex++;
	SelectTab(_selectedTabIndex);
}

void TabControl::PreviousTab()
{
	_selectedTabIndex--;
	SelectTab(_selectedTabIndex);
}

void TabControl::SelectTab(int index)
{	
	_selectedTabIndex = index;
	if (_selectedTabIndex >= _numTabs) { _selectedTabIndex = 0; }
	else if (_selectedTabIndex < 0) { _selectedTabIndex = _numTabs - 1; }
	
	ActiveChild = _tabContents[_selectedTabIndex];
	
	if (_onSelectedTabChanged != NULL) { _onSelectedTabChanged(_controlContext); }
}

int TabControl::GetSelectedTabIndex()
{
	return _selectedTabIndex;
}