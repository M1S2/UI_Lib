/*
 * Container.cpp
 */ 

#include "Containers/Container.h"

Container::Container(uint8_t maxNumItems) : UIElement(UI_CONTAINER)
{
	_numItems = 0;
	_maxNumItems = maxNumItems;
	_items = new UIElement*[maxNumItems]();
}

UIElement* Container::GetSelectedItem()
{
	return _items[_selectedItemIndex];
}

bool Container::AddItem(UIElement* item)
{
	if (_numItems >= _maxNumItems) { return false; }

	item->Parent = this;
	_items[_numItems] = item;
	_numItems++;
	item->RecalculateDimensions();
	
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

void Container::GetItemsBoundingBox(uint16_t* x, uint16_t* y, uint16_t* w, uint16_t* h)
{
	// Reset input parameters
	*x = 65535;
	*y = 65535;
	*w = 0;
	*h = 0;

	uint16_t maxX = 0, maxY = 0;
	for(int i = 0; i < _numItems; i++)
	{
		// Find lowest X and Y coordinates
		if(_items[i]->LocX < *x) { *x = _items[i]->LocX; }
		if(_items[i]->LocY < *y) { *y = _items[i]->LocY; }

		if((_items[i]->LocX + _items[i]->Width) > maxX) { maxX = _items[i]->LocX + _items[i]->Width; }
		if((_items[i]->LocY + _items[i]->Height) > maxY) { maxY = _items[i]->LocY + _items[i]->Height; }
	}
	if(maxX > 0) { *w = maxX - *x; }
	if(maxY > 0) { *h = maxY - *y; }
}