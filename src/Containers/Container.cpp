/*
 * Container.cpp
 */ 

#include "Containers/Container.h"

Container::Container() : UIElement(UI_CONTAINER)
{
	_numItems = 0;
}

UIElement* Container::GetSelectedItem()
{
	return _items[_selectedItemIndex];
}

bool Container::AddItem(UIElement* item)
{
	if (_numItems >= MAX_CONTAINER_ITEMS) { return false; }

	item->Parent = this;
	_items[_numItems] = item;
	_numItems++;

	// Move item inside container region
	item->LocX += LocX;
	item->LocY += LocY;
	// Strech the item to fill up the full container space if the items Width or Height are zero
	if(item->Type == UI_CONTAINER && item->Width == 0)
	{
		item->Width = Width;	
	}
	if(item->Type == UI_CONTAINER && item->Height == 0)
	{
		item->Height = Height;
	}
	
	if (ActiveChild == NULL) { ActiveChild = item; }
	return true;
}