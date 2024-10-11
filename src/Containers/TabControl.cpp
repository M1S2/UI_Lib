/*
 * TabControl.cpp
 */ 

#include "Containers/TabControl.h"
#include "Core/UI_Manager.h"
#include <string.h>

template <uint8_t maxTabs, uint8_t maxHeaderLength, uint8_t tabHeaderMargin, uint8_t contentPadding>
TabControl<maxTabs, maxHeaderLength, tabHeaderMargin, contentPadding>::TabControl(uint16_t width, uint16_t height, uint16_t tabRegionSize, TabPositions_t tabPosition, void* controlContext, void(*onSelectedTabChanged)(void* controlContext))
{
	this->Width = width;
	this->Height = height;
	_lastDrawnTabIndex = -1;
	_tabRegionSize = tabRegionSize;
	_tabPosition = tabPosition;
	_controlContext = controlContext;
	_onSelectedTabChanged = onSelectedTabChanged;
}

template <uint8_t maxTabs, uint8_t maxHeaderLength, uint8_t tabHeaderMargin, uint8_t contentPadding>
TabControl<maxTabs, maxHeaderLength, tabHeaderMargin, contentPadding>::TabControl(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height, uint16_t tabRegionSize, TabPositions_t tabPosition, void* controlContext, void(*onSelectedTabChanged)(void* controlContext))
{
	this->Width = width;
	this->Height = height;
	this->LocX = locX;
	this->LocY = locY;
	_lastDrawnTabIndex = -1;
	_tabRegionSize = tabRegionSize;
	_tabPosition = tabPosition;
	_controlContext = controlContext;
	_onSelectedTabChanged = onSelectedTabChanged;
}

template <uint8_t maxTabs, uint8_t maxHeaderLength, uint8_t tabHeaderMargin, uint8_t contentPadding>
void TabControl<maxTabs, maxHeaderLength, tabHeaderMargin, contentPadding>::Draw() 
{
	if (this->Visible)
	{
		if(_lastDrawnTabIndex != this->_selectedItemIndex || UiManager.WasTreeRootChanged)
		{
			UiManager.Gfx->fillRect(this->LocX, this->LocY, this->Width, this->Height, UiManager.ColorBackground);
			_lastDrawnTabIndex = this->_selectedItemIndex;

			switch (_tabPosition)
			{
				case TAB_POSITION_LEFT:
				{
					UiManager.Gfx->drawRect(this->LocX + _tabRegionSize - 1, this->LocY, this->Width - _tabRegionSize + 1, this->Height, UiManager.ColorForeground);
					int yTab = this->LocY;
					
					int tabFontHeight = UiManager.FontHeight - 2;
					int tabHeight = tabFontHeight + 8;
					
					for(int i = 0; i < this->_numItems; i++)
					{
						if(i == this->_selectedItemIndex)
						{
							UiManager.Gfx->drawRect(this->LocX, yTab, _tabRegionSize, tabHeight, UiManager.ColorForeground);
							UiManager.Gfx->drawFastVLine(this->LocX + _tabRegionSize - 1, yTab + 1, tabHeight - 2, UiManager.ColorBackground);
						}
						
						if(_headers[i] != NULL) 
						{
							UiManager.Gfx->setCursor(this->LocX + 2, yTab + ((tabHeight - tabFontHeight) / 2) + UiManager.FontHeight - 2 * UiManager.ElementPadding);
							UiManager.Gfx->print(_headers[i]);
						}
						yTab+=(tabHeight + tabHeaderMargin);
					}
					break;
				}
				case TAB_POSITION_TOP:
				{
					UiManager.Gfx->drawRect(this->LocX, this->LocY + _tabRegionSize - 1, this->Width, this->Height - _tabRegionSize + 1, UiManager.ColorForeground);
					int xTab = this->LocX;
										
					for(int i = 0; i < this->_numItems; i++)
					{
						int16_t x, y;
						uint16_t w, h;
						UiManager.Gfx->getTextBounds(_headers[i], 0, 0, &x, &y, &w, &h);
						int tabWidth = w + 8;

						if(i == this->_selectedItemIndex)
						{
							UiManager.Gfx->drawRect(xTab, this->LocY, tabWidth, _tabRegionSize, UiManager.ColorForeground);
							UiManager.Gfx->drawFastHLine(xTab + 1, this->LocY + _tabRegionSize - 1, tabWidth - 2, UiManager.ColorBackground);
						}
						
						if(_headers[i] != NULL) 
						{
							UiManager.Gfx->setCursor(xTab, this->LocY + UiManager.FontHeight + 2);
							UiManager.Gfx->print(_headers[i]);
						}
						xTab+=(tabWidth + tabHeaderMargin);
					}
					break;
				}
				default:
					break;
			}
		}
		
		if(this->_items[this->_selectedItemIndex] != NULL) { this->_items[this->_selectedItemIndex]->Draw(); }
	}
	else
	{
		UiManager.Gfx->fillRect(this->LocX, this->LocY, this->Width, this->Height, UiManager.ColorBackground);
	}
}

template <uint8_t maxTabs, uint8_t maxHeaderLength, uint8_t tabHeaderMargin, uint8_t contentPadding>
bool TabControl<maxTabs, maxHeaderLength, tabHeaderMargin, contentPadding>::KeyInput(Keys_t key)
{
	switch (key)
	{
		case KEYLEFT:
			this->PreviousItem();
			return true;
		case KEYRIGHT:
			this->NextItem();
			return true;
		default:
			return false;
	}
}

template <uint8_t maxTabs, uint8_t maxHeaderLength, uint8_t tabHeaderMargin, uint8_t contentPadding>
bool TabControl<maxTabs, maxHeaderLength, tabHeaderMargin, contentPadding>::AddItem(const char* header, UIElement* tabContent)
{
	if(!Container<maxTabs>::AddItem(tabContent)) { return false; }

	strncpy(_headers[this->_numItems - 1], header, maxHeaderLength);		// Copy a maximum number of StringLength characters to the _header buffer. If text is shorter, the array is zero padded.
	_headers[this->_numItems - 1][maxHeaderLength - 1] = '\0';				// The _header buffer must contain at least one termination character ('\0') at the end to protect from overflow.

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
	tabContent->LocX += this->LocX + xRegion_Offset;
	tabContent->LocY += this->LocY + yRegion_Offset;
	// Strech the tabContent to fill up the full tab content region space if the tabContent Width or Height are zero
	if(tabContent->Type == UI_CONTAINER && tabContent->Width == 0)
	{
		tabContent->Width = this->Width - xRegion_Offset - contentPadding;	
	}
	if(tabContent->Type == UI_CONTAINER && tabContent->Height == 0)
	{
		tabContent->Height = this->Height - yRegion_Offset - contentPadding;
	}
	
	if (this->ActiveChild == NULL) { this->ActiveChild = tabContent; }

	return true;
}

template <uint8_t maxTabs, uint8_t maxHeaderLength, uint8_t tabHeaderMargin, uint8_t contentPadding>
void TabControl<maxTabs, maxHeaderLength, tabHeaderMargin, contentPadding>::SelectTab(int index)
{	
	this->_selectedItemIndex = index;
	if (this->_selectedItemIndex >= this->_numItems) { this->_selectedItemIndex = 0; }
	else if (this->_selectedItemIndex < 0) { this->_selectedItemIndex = this->_numItems - 1; }
	
	this->ActiveChild = this->_items[this->_selectedItemIndex];
	
	if (_onSelectedTabChanged != NULL) { _onSelectedTabChanged(_controlContext); }
}

template <uint8_t maxTabs, uint8_t maxHeaderLength, uint8_t tabHeaderMargin, uint8_t contentPadding>
int TabControl<maxTabs, maxHeaderLength, tabHeaderMargin, contentPadding>::GetSelectedTabIndex()
{
	return this->_selectedItemIndex;
}

template <uint8_t maxTabs, uint8_t maxHeaderLength, uint8_t tabHeaderMargin, uint8_t contentPadding>
void TabControl<maxTabs, maxHeaderLength, tabHeaderMargin, contentPadding>::RecalculateDimensions()
{
	if(this->Parent != NULL)
	{
		// Strech the TabControl to fill up the full parent container space
		this->Width = this->Parent->Width;
		this->Height = this->Parent->Height;
	}
	else
	{
		// The TabControl is the top most control, size it to fill the complete screen
		this->Width = UiManager.Gfx->width();
		this->Height = UiManager.Gfx->height();
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

template <uint8_t maxTabs, uint8_t maxHeaderLength, uint8_t tabHeaderMargin, uint8_t contentPadding>
void TabControl<maxTabs, maxHeaderLength, tabHeaderMargin, contentPadding>::RecalculateItemLocations()
{
	#warning TBD.............
}