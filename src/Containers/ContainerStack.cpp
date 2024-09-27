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

	RecalculateDimensions();

	return true;
}
