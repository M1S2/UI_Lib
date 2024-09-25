/*
 * ContainerStack.cpp
 */ 

#include "Containers/ContainerStack.h"
#include "Core/UI_Manager.h"

ContainerStack::ContainerStack(StackOrientation_t stackOrientation, uint16_t marginBetweenElements)
{
	Type = UI_CONTAINER;
	_stackOrientation = stackOrientation;
	_marginBetweenElements = marginBetweenElements;
}

ContainerStack::ContainerStack(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height, StackOrientation_t stackOrientation, uint16_t marginBetweenElements)
{
	Type = UI_CONTAINER;
	LocX = locX;
	LocY = locY;
	Width = width;
	Height = height;
	_stackOrientation = stackOrientation;
	_marginBetweenElements = marginBetweenElements;
}

void ContainerStack::Draw(Adafruit_GFX* gfx)
{
	if (Visible)
	{
		ContainerPage::Draw(gfx);
	}
	else
	{
		gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);
	}
}

bool ContainerStack::AddItem(UIElement* item)
{
	if(!Container::AddItem(item, false, false)) { return false; }

	uint16_t x, y, w, h;
	GetItemsBoundingBox(&x, &y, &w, &h);
	Width = w;		// resize the ContainerStack to fit the size of the bounding box
	Height = h;

	if(_numItems <= 1) { return true; }				// it was the first added item, nothing to adapt
	UIElement* lastItem = _items[_numItems - 2];	// -1 because number to index and -1 to get the previous element

	switch (_stackOrientation)
	{
		case STACK_ORIENTATION_VERTICAL:
			item->LocX = lastItem->LocX;
			item->LocY = lastItem->LocY + lastItem->Height + _marginBetweenElements;
			break;
		case STACK_ORIENTATION_HORIZONTAL:
			item->LocX = lastItem->LocX + lastItem->Width + _marginBetweenElements;
			item->LocY = lastItem->LocY;
			break;
		default:
			break;
	}

	return true;
}

void ContainerStack::GetItemsBoundingBox(uint16_t* x, uint16_t* y, uint16_t* w, uint16_t* h)
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