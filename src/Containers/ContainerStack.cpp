/*
 * ContainerStack.cpp
 */ 

#include "Containers/ContainerStack.h"
#include "Core/UI_Manager.h"

template <uint8_t maxItems>
ContainerStack<maxItems>::ContainerStack(StackOrientation_t stackOrientation, uint16_t marginBetweenElements)
{
	this->Type = UI_CONTAINER;
	_stackOrientation = stackOrientation;
	_marginBetweenElements = marginBetweenElements;
}

template <uint8_t maxItems>
ContainerStack<maxItems>::ContainerStack(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height, StackOrientation_t stackOrientation, uint16_t marginBetweenElements)
{
	this->Type = UI_CONTAINER;
	this->LocX = locX;
	this->LocY = locY;
	this->Width = width;
	this->Height = height;
	_stackOrientation = stackOrientation;
	_marginBetweenElements = marginBetweenElements;
}

template <uint8_t maxItems>
void ContainerStack<maxItems>::Draw(Adafruit_GFX* gfx)
{
	if (this->Visible)
	{
		ContainerPage<maxItems>::Draw(gfx);
	}
	else
	{
		gfx->fillRect(this->LocX, this->LocY, this->Width, this->Height, UiManager.ColorBackground);
	}
}

template <uint8_t maxItems>
bool ContainerStack<maxItems>::AddItem(UIElement* item)
{
	if(!Container<maxItems>::AddItem(item, false, false)) { return false; }

	if(this->_numItems <= 1) { return true; }					// it was the first added item, nothing to adapt
	UIElement* lastItem = this->_items[this->_numItems - 2];	// -1 because number to index and -1 to get the previous element

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

	this->RecalculateDimensions();

	return true;
}
