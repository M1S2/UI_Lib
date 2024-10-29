/*
 * TabControl.cpp
 */ 

#include "Containers/TabControl.h"
#include "Core/UI_Manager.h"
#include <string.h>

TabControl::TabControl(uint16_t width, uint16_t height, TabPositions_t tabPosition, void* controlContext, void(*onSelectedTabChanged)(void* controlContext), uint8_t maxNumItems, uint8_t tabHeaderMargin) : Container(maxNumItems)
{
	this->Width = width;
	this->Height = height;
	_lastDrawnTabIndex = -1;
	_tabPosition = tabPosition;
	_controlContext = controlContext;
	_onSelectedTabChanged = onSelectedTabChanged;
	_tabHeaderMargin = tabHeaderMargin;
	_headers = new UIElement*[maxNumItems]();
}

TabControl::TabControl(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height, TabPositions_t tabPosition, void* controlContext, void(*onSelectedTabChanged)(void* controlContext), uint8_t maxNumItems, uint8_t tabHeaderMargin) : Container(maxNumItems)
{
	this->Width = width;
	this->Height = height;
	this->LocX = locX;
	this->LocY = locY;
	_lastDrawnTabIndex = -1;
	_tabPosition = tabPosition;
	_controlContext = controlContext;
	_onSelectedTabChanged = onSelectedTabChanged;
	_tabHeaderMargin = tabHeaderMargin;
	_headers = new UIElement*[maxNumItems]();
}

void TabControl::Draw(bool redraw) 
{
	if (this->Visible)
	{
		redraw = redraw || (_lastDrawnTabIndex != this->_selectedItemIndex) || !this->_lastDrawnVisible;
		this->_lastDrawnVisible = true;
		if(redraw)
		{
			UiManager.Gfx->fillRect(this->LocX, this->LocY, this->Width, this->Height, UiManager.ColorBackground);
			_lastDrawnTabIndex = this->_selectedItemIndex;

			switch (_tabPosition)
			{
				case TAB_POSITION_LEFT:
				{
					UiManager.Gfx->drawRect(this->LocX + TabRegionSize - 1, this->LocY, this->Width - TabRegionSize + 1, this->Height, UiManager.ColorForeground);
					
					for(int i = 0; i < this->_numItems; i++)
					{
						if(i == this->_selectedItemIndex)
						{
							UiManager.Gfx->drawRect(this->LocX, _headers[i]->LocY - _tabHeaderMargin, TabRegionSize, _headers[i]->Height + 2 * _tabHeaderMargin, UiManager.ColorForeground);
							UiManager.Gfx->drawFastVLine(this->LocX + TabRegionSize - 1, _headers[i]->LocY - _tabHeaderMargin + 1, _headers[i]->Height + 2 * _tabHeaderMargin - 2, UiManager.ColorBackground);
						}
						
						if(_headers[i] != NULL) { _headers[i]->Draw(true); }
					}
					break;
				}
				case TAB_POSITION_TOP:
				{
					UiManager.Gfx->drawRect(this->LocX, this->LocY + TabRegionSize - 1, this->Width, this->Height - TabRegionSize + 1, UiManager.ColorForeground);
										
					for(int i = 0; i < this->_numItems; i++)
					{
						if(i == this->_selectedItemIndex)
						{
							UiManager.Gfx->drawRect(_headers[i]->LocX - _tabHeaderMargin, this->LocY, _headers[i]->Width + 2 * _tabHeaderMargin, TabRegionSize, UiManager.ColorForeground);
							UiManager.Gfx->drawFastHLine(_headers[i]->LocX - _tabHeaderMargin + 1, this->LocY + TabRegionSize - 1, _headers[i]->Width + 2 * _tabHeaderMargin - 2, UiManager.ColorBackground);						
						}
						
						if(_headers[i] != NULL) { _headers[i]->Draw(true); }
					}
					break;
				}
				default: break;
			}
		}
		
		if(this->_items[this->_selectedItemIndex] != NULL) { this->_items[this->_selectedItemIndex]->Draw(redraw); }
	}
	else if(!this->Visible && this->_lastDrawnVisible)		// clear only when the Visible property changes from true to false
	{
		this->_lastDrawnVisible = false;
		UiManager.Gfx->fillRect(this->LocX, this->LocY, this->Width, this->Height, UiManager.ColorBackground);
	}
}

bool TabControl::KeyInput(Keys_t key)
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

bool TabControl::AddItem(UIElement* header, UIElement* tabContent)
{
	_headers[this->_numItems] = header;
	header->RecalculateDimensions();		

	if(!Container::AddItem(tabContent)) { return false; }

	RecalculateDimensions();

	return true;
}

void TabControl::SelectTab(int index)
{	
	this->_selectedItemIndex = index;
	if (this->_selectedItemIndex >= this->_numItems) { this->_selectedItemIndex = 0; }
	else if (this->_selectedItemIndex < 0) { this->_selectedItemIndex = this->_numItems - 1; }
	
	this->ActiveChild = this->_items[this->_selectedItemIndex];
	
	if (_onSelectedTabChanged != NULL) { _onSelectedTabChanged(_controlContext); }
}

int TabControl::GetSelectedTabIndex()
{
	return this->_selectedItemIndex;
}

void TabControl::GetContentRegionSize(uint16_t* w, uint16_t* h)
{
	switch (_tabPosition)
	{
		case TAB_POSITION_LEFT:
			*w = this->Width - TabRegionSize - 2 * UiManager.ElementPadding;
			*h = this->Height - 2 * UiManager.ElementPadding;
			break;
		case TAB_POSITION_TOP:
			*w = this->Width - 2 * UiManager.ElementPadding;
			*h = this->Height - TabRegionSize - 2 * UiManager.ElementPadding;
			break;
		default: break;
	}
}

void TabControl::RecalculateDimensions()
{
	if(this->Width == 0 && this->Height == 0)
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
}

void TabControl::RecalculateLayout()
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
		case TAB_POSITION_LEFT: TabRegionSize = maxHeaderWidth + 2 * _tabHeaderMargin; break;
		case TAB_POSITION_TOP: TabRegionSize = maxHeaderHeight + 2 * _tabHeaderMargin; break;
		default: break;
	}

	uint16_t xRegion_Offset = 0, yRegion_Offset = 0;
	switch (_tabPosition)
	{
		case TAB_POSITION_LEFT:
			xRegion_Offset = TabRegionSize + UiManager.ElementPadding;
			yRegion_Offset = UiManager.ElementPadding;
			break;
		case TAB_POSITION_TOP:
			xRegion_Offset = UiManager.ElementPadding;
			yRegion_Offset = TabRegionSize + UiManager.ElementPadding;
			break;
		default: break;
	}
	
	uint16_t currentHeaderX = this->LocX + _tabHeaderMargin, currentHeaderY = this->LocY + _tabHeaderMargin;
	for(int i = 0; i < this->_numItems; i++)
	{
		UIElement* currentItem = this->_items[i];
		UIElement* currentHeader = _headers[i];

		if(currentHeader != NULL)
		{
			currentHeader->LocX = currentHeaderX;
			currentHeader->LocY = currentHeaderY;
			currentHeader->RecalculateLayout();
			currentHeader->RecalculateDimensions();
			switch (_tabPosition)
			{
				case TAB_POSITION_LEFT: currentHeaderY += currentHeader->Height + 2 * _tabHeaderMargin; break;
				case TAB_POSITION_TOP: currentHeaderX += currentHeader->Width + 2 * _tabHeaderMargin; break;
				default: break;
			}
		}

		// Move tabContent inside tab content region
		currentItem->LocX = this->LocX + xRegion_Offset;
		currentItem->LocY = this->LocY + yRegion_Offset;
		// Strech the tabContent to fill up the full tab content region space
		if(currentItem->Type == UI_CONTAINER && currentItem->Width == 0)
		{
			currentItem->Width = this->Width - xRegion_Offset - UiManager.ElementPadding;	
		}
		if(currentItem->Type == UI_CONTAINER && currentItem->Height == 0)
		{
			currentItem->Height = this->Height - yRegion_Offset - UiManager.ElementPadding;
		}
		currentItem->RecalculateLayout();
		currentItem->RecalculateDimensions();
	}

	RecalculateDimensions();
}