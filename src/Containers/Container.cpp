/*
 * Container.cpp
 */ 

#include "Containers/Container.h"

template <uint8_t maxItems>
Container<maxItems>::Container() : UIElement(UI_CONTAINER)
{
	_numItems = 0;
}

template <uint8_t maxItems>
UIElement* Container<maxItems>::GetSelectedItem()
{
	return _items[_selectedItemIndex];
}

template <uint8_t maxItems>
bool Container<maxItems>::AddItem(UIElement* item)
{
	if (_numItems >= maxItems) { return false; }

	item->Parent = this;
	_items[_numItems] = item;
	_numItems++;
	item->RecalculateDimensions();

	this->RecalculateLayout();
	
	if (ActiveChild == NULL) { ActiveChild = item; }
	return true;
}

template <uint8_t maxItems>
bool Container<maxItems>::NextItem()
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

template <uint8_t maxItems>
bool Container<maxItems>::PreviousItem()
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

template <uint8_t maxItems>
bool Container<maxItems>::NextControlItem()
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

template <uint8_t maxItems>
bool Container<maxItems>::PreviousControlItem()
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

template <uint8_t maxItems>
void Container<maxItems>::GetItemsBoundingBox(uint16_t* x, uint16_t* y, uint16_t* w, uint16_t* h)
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