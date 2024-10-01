/*
 * TabControl.cpp
 */ 

#include "Controls/TabControl.h"
#include "Core/UI_Manager.h"
#include <string.h>

template <uint8_t maxTabs, uint8_t maxHeaderLength, uint8_t tabHeaderMargin, uint8_t contentPadding>
TabControl<maxTabs, maxHeaderLength, tabHeaderMargin, contentPadding>::TabControl(uint16_t width, uint16_t height, uint16_t tabRegionSize, TabPositions_t tabPosition, void* controlContext, void(*onSelectedTabChanged)(void* controlContext)) : UIElement(UI_CONTROL)
{
	Width = width;
	Height = height;
	_numTabs = 0;
	_selectedTabIndex = 0;
	_lastDrawnTabIndex = -1;
	_tabRegionSize = tabRegionSize;
	_tabPosition = tabPosition;
	_controlContext = controlContext;
	_onSelectedTabChanged = onSelectedTabChanged;
}

template <uint8_t maxTabs, uint8_t maxHeaderLength, uint8_t tabHeaderMargin, uint8_t contentPadding>
TabControl<maxTabs, maxHeaderLength, tabHeaderMargin, contentPadding>::TabControl(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height, uint16_t tabRegionSize, TabPositions_t tabPosition, void* controlContext, void(*onSelectedTabChanged)(void* controlContext)) : UIElement(locX, locY, UI_CONTROL)
{
	Width = width;
	Height = height;
	_numTabs = 0;
	_selectedTabIndex = 0;
	_lastDrawnTabIndex = -1;
	_tabRegionSize = tabRegionSize;
	_tabPosition = tabPosition;
	_controlContext = controlContext;
	_onSelectedTabChanged = onSelectedTabChanged;
}

template <uint8_t maxTabs, uint8_t maxHeaderLength, uint8_t tabHeaderMargin, uint8_t contentPadding>
void TabControl<maxTabs, maxHeaderLength, tabHeaderMargin, contentPadding>::Draw(Adafruit_GFX* gfx) 
{
	if (Visible)
	{
		if(_lastDrawnTabIndex != _selectedTabIndex || UiManager.WasTreeRootChanged)
		{
			gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);
			_lastDrawnTabIndex = _selectedTabIndex;

			switch (_tabPosition)
			{
				case TAB_POSITION_LEFT:
				{
					gfx->drawRect(LocX + _tabRegionSize - 1, LocY, Width - _tabRegionSize + 1, Height, UiManager.ColorForeground);
					int yTab = LocY;
					
					int tabFontHeight = UiManager.FontHeight - 2;
					int tabHeight = tabFontHeight + 8;
					
					for(int i = 0; i < _numTabs; i++)
					{
						if(i == _selectedTabIndex)
						{
							gfx->drawRect(LocX, yTab, _tabRegionSize, tabHeight, UiManager.ColorForeground);
							gfx->drawFastVLine(LocX + _tabRegionSize - 1, yTab + 1, tabHeight - 2, UiManager.ColorBackground);
						}
						
						if(_headers[i] != NULL) 
						{
							gfx->setCursor(LocX + 2, yTab + ((tabHeight - tabFontHeight) / 2) + UiManager.FontHeight - 2 * UiManager.ElementPadding);
							gfx->print(_headers[i]);
						}
						yTab+=(tabHeight + tabHeaderMargin);
					}
					break;
				}
				case TAB_POSITION_TOP:
				{
					gfx->drawRect(LocX, LocY + _tabRegionSize - 1, Width, Height - _tabRegionSize + 1, UiManager.ColorForeground);
					int xTab = LocX;
										
					for(int i = 0; i < _numTabs; i++)
					{
						int16_t x, y;
						uint16_t w, h;
						gfx->getTextBounds(_headers[i], 0, 0, &x, &y, &w, &h);
						int tabWidth = w + 8;

						if(i == _selectedTabIndex)
						{
							gfx->drawRect(xTab, LocY, tabWidth, _tabRegionSize, UiManager.ColorForeground);
							gfx->drawFastHLine(xTab + 1, LocY + _tabRegionSize - 1, tabWidth - 2, UiManager.ColorBackground);
						}
						
						if(_headers[i] != NULL) 
						{
							gfx->setCursor(xTab, LocY + UiManager.FontHeight + 2);
							gfx->print(_headers[i]);
						}
						xTab+=(tabWidth + tabHeaderMargin);
					}
					break;
				}
				default:
					break;
			}
		}
		
		if(_tabContents[_selectedTabIndex] != NULL) { _tabContents[_selectedTabIndex]->Draw(gfx); }
	}
	else
	{
		gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);
	}
}

template <uint8_t maxTabs, uint8_t maxHeaderLength, uint8_t tabHeaderMargin, uint8_t contentPadding>
bool TabControl<maxTabs, maxHeaderLength, tabHeaderMargin, contentPadding>::KeyInput(Keys_t key)
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

template <uint8_t maxTabs, uint8_t maxHeaderLength, uint8_t tabHeaderMargin, uint8_t contentPadding>
void TabControl<maxTabs, maxHeaderLength, tabHeaderMargin, contentPadding>::AddTab(const char* header, UIElement* tabContent)
{
	if (_numTabs >= maxTabs) { return; }

	tabContent->Parent = this;
	_tabContents[_numTabs] = tabContent;
	strncpy(_headers[_numTabs], header, maxHeaderLength);		// Copy a maximum number of StringLength characters to the _header buffer. If text is shorter, the array is zero padded.
	_headers[_numTabs][maxHeaderLength - 1] = '\0';			// The _header buffer must contain at least one termination character ('\0') at the end to protect from overflow.
	_numTabs++;

	RecalculateDimensions();

	uint16_t xRegion_Offset = 0, yRegion_Offset = 0;
	switch (_tabPosition)
	{
		case TAB_POSITION_LEFT:
			xRegion_Offset = _tabRegionSize + contentPadding;
			yRegion_Offset = contentPadding;
			break;
		case TAB_POSITION_TOP:
			xRegion_Offset = contentPadding;
			yRegion_Offset = _tabRegionSize + contentPadding;
			break;
		default:
			break;
	}
	
	// Move tabContent inside tab content region
	tabContent->LocX += LocX + xRegion_Offset;
	tabContent->LocY += LocY + yRegion_Offset;
	// Strech the tabContent to fill up the full tab content region space if the tabContent Width or Height are zero
	if(tabContent->Type == UI_CONTAINER && tabContent->Width == 0)
	{
		tabContent->Width = Width - xRegion_Offset - contentPadding;	
	}
	if(tabContent->Type == UI_CONTAINER && tabContent->Height == 0)
	{
		tabContent->Height = Height - yRegion_Offset - contentPadding;
	}
	
	if (ActiveChild == NULL) { ActiveChild = tabContent; }
}

template <uint8_t maxTabs, uint8_t maxHeaderLength, uint8_t tabHeaderMargin, uint8_t contentPadding>
void TabControl<maxTabs, maxHeaderLength, tabHeaderMargin, contentPadding>::NextTab()
{
	_selectedTabIndex++;
	SelectTab(_selectedTabIndex);
}

template <uint8_t maxTabs, uint8_t maxHeaderLength, uint8_t tabHeaderMargin, uint8_t contentPadding>
void TabControl<maxTabs, maxHeaderLength, tabHeaderMargin, contentPadding>::PreviousTab()
{
	_selectedTabIndex--;
	SelectTab(_selectedTabIndex);
}

template <uint8_t maxTabs, uint8_t maxHeaderLength, uint8_t tabHeaderMargin, uint8_t contentPadding>
void TabControl<maxTabs, maxHeaderLength, tabHeaderMargin, contentPadding>::SelectTab(int index)
{	
	_selectedTabIndex = index;
	if (_selectedTabIndex >= _numTabs) { _selectedTabIndex = 0; }
	else if (_selectedTabIndex < 0) { _selectedTabIndex = _numTabs - 1; }
	
	ActiveChild = _tabContents[_selectedTabIndex];
	
	if (_onSelectedTabChanged != NULL) { _onSelectedTabChanged(_controlContext); }
}

template <uint8_t maxTabs, uint8_t maxHeaderLength, uint8_t tabHeaderMargin, uint8_t contentPadding>
int TabControl<maxTabs, maxHeaderLength, tabHeaderMargin, contentPadding>::GetSelectedTabIndex()
{
	return _selectedTabIndex;
}

template <uint8_t maxTabs, uint8_t maxHeaderLength, uint8_t tabHeaderMargin, uint8_t contentPadding>
void TabControl<maxTabs, maxHeaderLength, tabHeaderMargin, contentPadding>::RecalculateDimensions()
{
	if(Parent != NULL)
	{
		// Strech the TabControl to fill up the full parent container space
		Width = Parent->Width;
		Height = Parent->Height;
	}
	else
	{
		// The TabControl is the top most control, size it to fill the complete screen
		Width = UiManager.Gfx->width();
		Height = UiManager.Gfx->height();
	}

	/*switch (_tabPosition)
	{
		case TAB_POSITION_LEFT:
		{
			uint16_t maxTabHeaderWidth = 0;
			for(int i = 0; i < _numTabs; i++)
			{
				if(_headers[i] == NULL) { continue; }
				int16_t x, y;
				uint16_t w, h;
				UiManager.Gfx->getTextBounds(_headers[i], 0, 0, &x, &y, &w, &h);
				if(w > maxTabHeaderWidth) { maxTabHeaderWidth = w; }
			}
			_tabRegionSize = maxTabHeaderWidth + 2 * UiManager.ElementPadding + 2;
			break;
		}
		case TAB_POSITION_TOP:
		{
			_tabRegionSize = UiManager.FontHeight + 2 * UiManager.ElementPadding + 2;
			break;
		}
		default: break;
	}*/
}