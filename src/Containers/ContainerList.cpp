/*
 * ContainerList.cpp
 */ 

#include "Containers/ContainerList.h"
#include "Core/UI_Manager.h"

template <uint8_t maxItems, uint8_t scrollBarWidth, uint8_t scrollBarMargin>
ContainerList<maxItems, scrollBarWidth, scrollBarMargin>::ContainerList()
{
	this->Type = UI_CONTAINER;
	_lastDrawnItemIndex = -1;
}

template <uint8_t maxItems, uint8_t scrollBarWidth, uint8_t scrollBarMargin>
ContainerList<maxItems, scrollBarWidth, scrollBarMargin>::ContainerList(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height)
{
	this->Type = UI_CONTAINER;
	this->LocX = locX;
	this->LocY = locY;
	this->Width = width;
	this->Height = height;
	_lastDrawnItemIndex = -1;
}

template <uint8_t maxItems, uint8_t scrollBarWidth, uint8_t scrollBarMargin>
void ContainerList<maxItems, scrollBarWidth, scrollBarMargin>::Draw()
{
	if(_lastDrawnItemIndex != this->_selectedItemIndex || UiManager.WasTreeRootChanged)
	{
		UiManager.Gfx->fillRect(this->LocX, this->LocY, this->Width, this->Height, UiManager.ColorBackground);
		_lastDrawnItemIndex = this->_selectedItemIndex;
	}

	UIElement* item = this->GetSelectedItem();
	if(item != NULL) { item->Draw(); }
	
	// Count visible items
	int _numVisibleItems = 0;
	int _numNonVisibleItemsBeforeSelected = 0;
	for(int i=0; i<this->_numItems; i++)
	{
		if(this->_items[i]->Visible == true) { _numVisibleItems++; }
		if(this->_items[i]->Visible == false && i < this->_selectedItemIndex) { _numNonVisibleItemsBeforeSelected++; }
	}
	
	// Draw scroll bar
	uint16_t scrollBarLeft = this->LocX + this->Width - scrollBarWidth - scrollBarMargin;
	uint16_t scrollBarBoxHeight = this->Height - 2 * scrollBarMargin;
	uint16_t scrollBarHeight = scrollBarBoxHeight / _numVisibleItems;

	UiManager.Gfx->drawRect(scrollBarLeft, this->LocY + scrollBarMargin, scrollBarWidth, scrollBarBoxHeight, UiManager.ColorForeground);
	UiManager.Gfx->fillRect(scrollBarLeft, this->LocY + scrollBarMargin + ((this->_selectedItemIndex - _numNonVisibleItemsBeforeSelected) * scrollBarHeight), scrollBarWidth, scrollBarHeight, UiManager.ColorForeground);
}

template <uint8_t maxItems, uint8_t scrollBarWidth, uint8_t scrollBarMargin>
bool ContainerList<maxItems, scrollBarWidth, scrollBarMargin>::KeyInput(Keys_t key)
{
	switch (key)
	{
		case KEYUP:
			return this->NextItem();
		case KEYDOWN:
			return this->PreviousItem();
		default:
			return false;
	}
}

template <uint8_t maxItems, uint8_t scrollBarWidth, uint8_t scrollBarMargin>
void ContainerList<maxItems, scrollBarWidth, scrollBarMargin>::RecalculateDimensions()
{
	// Strech the container to fill up the full parent container space
	if(this->Parent != NULL)
	{
		this->Width = this->Parent->Width;
		this->Height = this->Parent->Height;
	}
}

template <uint8_t maxItems, uint8_t scrollBarWidth, uint8_t scrollBarMargin>
void ContainerList<maxItems, scrollBarWidth, scrollBarMargin>::RecalculateItemLocations()
{
	// Move each item inside container region
	for(int i = 0; i < this->_numItems; i++)
	{
		UIElement* currentItem = this->_items[i];
		currentItem->LocX = this->LocX;
		currentItem->LocY = this->LocY;

		if(currentItem->Type == UI_CONTAINER)
		{
			((Container<maxItems>*)currentItem)->RecalculateItemLocations();
		}
	}
}