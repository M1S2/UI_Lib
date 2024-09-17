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
	
	if (ActiveChild == NULL) { ActiveChild = item; }
	return true;
}