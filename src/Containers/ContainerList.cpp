/*
 * ContainerList.cpp
 */ 

#include "Containers/ContainerList.h"

ContainerList::ContainerList(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height)
{
	Type = UI_CONTAINER;
	LocX = locX;
	LocY = locY;
	Width = width;
	Height = height;
	_lastDrawnItemIndex = -1;
}

void ContainerList::Draw(Adafruit_GFX* gfx, bool wasScreenCleared)
{
	if(_lastDrawnItemIndex != _selectedItemIndex || wasScreenCleared)
	{
		gfx->fillRect(LocX, LocY, Width, Height, UI_LIB_COLOR_BACKGROUND);
		_lastDrawnItemIndex = _selectedItemIndex;
	}

	UIElement* item = GetSelectedItem();
	if(item != NULL) { item->Draw(gfx, wasScreenCleared); }
	
	// Count visible items
	int _numVisibleItems = 0;
	int _numNonVisibleItemsBeforeSelected = 0;
	for(int i=0; i<_numItems; i++)
	{
		if(_items[i]->Visible == true) { _numVisibleItems++; }
		if(_items[i]->Visible == false && i < _selectedItemIndex) { _numNonVisibleItemsBeforeSelected++; }
	}
	
	// Draw scroll bar
	uint16_t scrollBarLeft = LocX + Width - SCROLLBAR_WIDTH - SCROLLBAR_MARGIN;
	uint16_t scrollBarBoxHeight = Height - 2 * SCROLLBAR_MARGIN;
	uint16_t scrollBarHeight = scrollBarBoxHeight / _numVisibleItems;
	
	gfx->drawRect(scrollBarLeft, LocY + SCROLLBAR_MARGIN, SCROLLBAR_WIDTH, scrollBarBoxHeight, UI_LIB_COLOR_FOREGROUND);
	gfx->fillRect(scrollBarLeft, LocY + SCROLLBAR_MARGIN + ((_selectedItemIndex - _numNonVisibleItemsBeforeSelected) * scrollBarHeight), SCROLLBAR_WIDTH, scrollBarHeight, UI_LIB_COLOR_FOREGROUND);
}

bool ContainerList::KeyInput(Keys_t key)
{
	switch (key)
	{
		case KEYUP:
			return NextItem();
		case KEYDOWN:
			return PreviousItem();
		default:
			return false;
	}
}

bool ContainerList::NextItem()
{
	if (_selectedItemIndex >= _numItems - 1)
	{
		return false;
	} 	
	
	while (_selectedItemIndex < _numItems - 1) 
	{
		_selectedItemIndex++;
		ActiveChild = GetSelectedItem();
		if(ActiveChild != NULL && ActiveChild->Visible == true) { return true; }
	}
	_selectedItemIndex--;	// Decrement index to stay in range (was incremented in the loop before)
	ActiveChild = GetSelectedItem();
	return false;
}

bool ContainerList::PreviousItem()
{
	if (_selectedItemIndex <= 0)
	{
		return false;
	}
	
	while (_selectedItemIndex > 0)
	{
		_selectedItemIndex--;
		ActiveChild = GetSelectedItem();
		if(ActiveChild != NULL && ActiveChild->Visible == true) { return true; }
	}
	_selectedItemIndex++;	// Increment index to stay in range (was decremented in the loop before)
	ActiveChild = GetSelectedItem();
	return false;
}