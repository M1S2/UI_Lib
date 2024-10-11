/*
 * TabControl.cpp
 */ 

#include "Containers/TabControl.h"
#include "Core/UI_Manager.h"
#include <string.h>

template <uint8_t maxTabs, uint8_t tabHeaderMargin, uint8_t contentPadding>
TabControl<maxTabs, tabHeaderMargin, contentPadding>::TabControl(uint16_t width, uint16_t height, TabPositions_t tabPosition, void* controlContext, void(*onSelectedTabChanged)(void* controlContext))
{
	this->Width = width;
	this->Height = height;
	_lastDrawnTabIndex = -1;
	_tabPosition = tabPosition;
	_controlContext = controlContext;
	_onSelectedTabChanged = onSelectedTabChanged;
}

template <uint8_t maxTabs, uint8_t tabHeaderMargin, uint8_t contentPadding>
TabControl<maxTabs, tabHeaderMargin, contentPadding>::TabControl(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height, TabPositions_t tabPosition, void* controlContext, void(*onSelectedTabChanged)(void* controlContext))
{
	this->Width = width;
	this->Height = height;
	this->LocX = locX;
	this->LocY = locY;
	_lastDrawnTabIndex = -1;
	_tabPosition = tabPosition;
	_controlContext = controlContext;
	_onSelectedTabChanged = onSelectedTabChanged;
}

template <uint8_t maxTabs, uint8_t tabHeaderMargin, uint8_t contentPadding>
void TabControl<maxTabs, tabHeaderMargin, contentPadding>::Draw() 
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
					
					for(int i = 0; i < this->_numItems; i++)
					{
						if(i == this->_selectedItemIndex)
						{
							UiManager.Gfx->drawRect(this->LocX, _headers[i]->LocY - tabHeaderMargin, _tabRegionSize, _headers[i]->Height + 2 * tabHeaderMargin, UiManager.ColorForeground);
							UiManager.Gfx->drawFastVLine(this->LocX + _tabRegionSize - 1, _headers[i]->LocY - tabHeaderMargin + 1, _headers[i]->Height + 2 * tabHeaderMargin - 2, UiManager.ColorBackground);
						}
						
						if(_headers[i] != NULL) { _headers[i]->Draw(); }
					}
					break;
				}
				case TAB_POSITION_TOP:
				{
					UiManager.Gfx->drawRect(this->LocX, this->LocY + _tabRegionSize - 1, this->Width, this->Height - _tabRegionSize + 1, UiManager.ColorForeground);
										
					for(int i = 0; i < this->_numItems; i++)
					{
						if(i == this->_selectedItemIndex)
						{
							UiManager.Gfx->drawRect(_headers[i]->LocX - tabHeaderMargin, this->LocY, _headers[i]->Width + 2 * tabHeaderMargin, _tabRegionSize, UiManager.ColorForeground);
							UiManager.Gfx->drawFastHLine(_headers[i]->LocX - tabHeaderMargin + 1, this->LocY + _tabRegionSize - 1, _headers[i]->Width + 2 * tabHeaderMargin - 2, UiManager.ColorBackground);						
						}
						
						if(_headers[i] != NULL) { _headers[i]->Draw(); }
					}
					break;
				}
				default: break;
			}
		}
		
		if(this->_items[this->_selectedItemIndex] != NULL) { this->_items[this->_selectedItemIndex]->Draw(); }
	}
	else
	{
		UiManager.Gfx->fillRect(this->LocX, this->LocY, this->Width, this->Height, UiManager.ColorBackground);
	}
}

template <uint8_t maxTabs, uint8_t tabHeaderMargin, uint8_t contentPadding>
bool TabControl<maxTabs, tabHeaderMargin, contentPadding>::KeyInput(Keys_t key)
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

template <uint8_t maxTabs, uint8_t tabHeaderMargin, uint8_t contentPadding>
bool TabControl<maxTabs, tabHeaderMargin, contentPadding>::AddItem(UIElement* header, UIElement* tabContent)
{
	_headers[this->_numItems] = header;
	header->RecalculateDimensions();		

	if(!Container<maxTabs>::AddItem(tabContent)) { return false; }

	RecalculateDimensions();

	return true;
}

template <uint8_t maxTabs, uint8_t tabHeaderMargin, uint8_t contentPadding>
void TabControl<maxTabs, tabHeaderMargin, contentPadding>::SelectTab(int index)
{	
	this->_selectedItemIndex = index;
	if (this->_selectedItemIndex >= this->_numItems) { this->_selectedItemIndex = 0; }
	else if (this->_selectedItemIndex < 0) { this->_selectedItemIndex = this->_numItems - 1; }
	
	this->ActiveChild = this->_items[this->_selectedItemIndex];
	
	if (_onSelectedTabChanged != NULL) { _onSelectedTabChanged(_controlContext); }
}

template <uint8_t maxTabs, uint8_t tabHeaderMargin, uint8_t contentPadding>
int TabControl<maxTabs, tabHeaderMargin, contentPadding>::GetSelectedTabIndex()
{
	return this->_selectedItemIndex;
}

template <uint8_t maxTabs, uint8_t tabHeaderMargin, uint8_t contentPadding>
void TabControl<maxTabs, tabHeaderMargin, contentPadding>::RecalculateDimensions()
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
}

template <uint8_t maxTabs, uint8_t tabHeaderMargin, uint8_t contentPadding>
void TabControl<maxTabs, tabHeaderMargin, contentPadding>::RecalculateItemLocations()
{
	uint16_t maxHeaderWidth = 0, maxHeaderHeight = 0;
	for (int i = 0; i < this->_numItems; i++)
	{
		if(_headers[i] == NULL) { continue; }
		if(_headers[i]->Width > maxHeaderWidth) { maxHeaderWidth = _headers[i]->Width; }
		if(_headers[i]->Height > maxHeaderHeight) { maxHeaderHeight = _headers[i]->Height; }
	}
	switch (_tabPosition)
	{
		case TAB_POSITION_LEFT: _tabRegionSize = maxHeaderWidth + 2 * tabHeaderMargin; break;
		case TAB_POSITION_TOP: _tabRegionSize = maxHeaderHeight + 2 * tabHeaderMargin; break;
		default: break;
	}

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
		default: break;
	}
	
	uint16_t currentHeaderX = this->LocX + tabHeaderMargin, currentHeaderY = this->LocY + tabHeaderMargin;
	for(int i = 0; i < this->_numItems; i++)
	{
		UIElement* currentItem = this->_items[i];
		UIElement* currentHeader = _headers[i];

		if(currentHeader != NULL)
		{
			currentHeader->LocX = currentHeaderX;
			currentHeader->LocY = currentHeaderY;
			switch (_tabPosition)
			{
				case TAB_POSITION_LEFT: currentHeaderY += currentHeader->Height + 2 * tabHeaderMargin; break;
				case TAB_POSITION_TOP: currentHeaderX += currentHeader->Width + 2 * tabHeaderMargin; break;
				default: break;
			}

			if(currentHeader->Type == UI_CONTAINER)
			{
				((Container<maxTabs>*)currentHeader)->RecalculateItemLocations();
			}
		}

		// Move tabContent inside tab content region
		currentItem->LocX = this->LocX + xRegion_Offset;
		currentItem->LocY = this->LocY + yRegion_Offset;
		// Strech the tabContent to fill up the full tab content region space if the tabContent Width or Height are zero
		if(currentItem->Type == UI_CONTAINER && currentItem->Width == 0)
		{
			currentItem->Width = this->Width - xRegion_Offset - contentPadding;	
		}
		if(currentItem->Type == UI_CONTAINER && currentItem->Height == 0)
		{
			currentItem->Height = this->Height - yRegion_Offset - contentPadding;
		}
	}
}