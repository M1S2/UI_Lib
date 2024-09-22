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

bool Container::AddItem(UIElement* item, bool stretchWidthEnabled, bool strechHeightEnabled)
{
	if (_numItems >= MAX_CONTAINER_ITEMS) { return false; }

	item->Parent = this;
	_items[_numItems] = item;
	_numItems++;

	// Move item inside container region
	item->LocX += LocX;
	item->LocY += LocY;
	// Strech the item to fill up the full container space if the items Width or Height are zero
	if(item->Type == UI_CONTAINER && item->Width == 0 && stretchWidthEnabled)
	{
		item->Width = Width;	
	}
	if(item->Type == UI_CONTAINER && item->Height == 0 && strechHeightEnabled)
	{
		item->Height = Height;
	}
	
	if (ActiveChild == NULL) { ActiveChild = item; }
	return true;
}

bool Container::NextItem()
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

bool Container::PreviousItem()
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

bool Container::NextControlItem()
{
	int nextControlIndex = -1;
	for (int i = (_selectedItemIndex + 1); i < _numItems; i++)
	{
		if (_items[i]->Type == UI_CONTROL && _items[i]->Visible == true)
		{
			nextControlIndex = i;
			break;
		}
		else if (_items[i]->Type == UI_CONTAINER && _items[i]->Visible == true)
		{
			Container* container = (Container*)_items[i];
			container->NextControlItem();
			nextControlIndex = i;
			break;
		}
	}
	
	if (nextControlIndex != -1)
	{
		_selectedItemIndex = nextControlIndex;
		ActiveChild = GetSelectedItem();
		return true;
	}
	return false;
}

bool Container::PreviousControlItem()
{
	int previousControlIndex = -1;
	for (int i = (_selectedItemIndex - 1); i >= 0; i--)
	{
		if (_items[i]->Type == UI_CONTROL && _items[i]->Visible == true)
		{
			previousControlIndex = i;
			break;
		}
		else if (_items[i]->Type == UI_CONTAINER && _items[i]->Visible == true)
		{
			Container* container = (Container*)_items[i];
			container->PreviousControlItem();
			previousControlIndex = i;
			break;
		}
	}
	
	if (previousControlIndex != -1)
	{
		_selectedItemIndex = previousControlIndex;
		ActiveChild = GetSelectedItem();
		return true;
	}
	return false;
}