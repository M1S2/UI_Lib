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
void ContainerStack<maxItems>::Draw()
{
	if (this->Visible)
	{
		for (int i = 0; i < this->_numItems; i++)
		{
			this->_items[i]->Draw();
		}
	}
	else
	{
		UiManager.Gfx->fillRect(this->LocX, this->LocY, this->Width, this->Height, UiManager.ColorBackground);
	}
}

template <uint8_t maxItems>
bool ContainerStack<maxItems>::KeyInput(Keys_t key)
{
	switch (key)
	{
		case KEYUP:
			return this->NextControlItem();
		case KEYDOWN:
			return this->PreviousControlItem();
		default:
			return false;
	}
}

template <uint8_t maxItems>
void ContainerStack<maxItems>::InitItems()
{
	this->_selectedItemIndex = 0;
	if (this->GetSelectedItem()->Type != UI_CONTROL || this->GetSelectedItem()->Visible == false)
	{
		this->NextControlItem();
	}
}

template <uint8_t maxItems>
void ContainerStack<maxItems>::RecalculateDimensions()
{
	uint16_t x, y, w, h;
	this->GetItemsBoundingBox(&x, &y, &w, &h);
	this->Width = w;		// resize the ContainerStack to fit the size of the bounding box
	this->Height = h;
}

template <uint8_t maxItems>
void ContainerStack<maxItems>::RecalculateItemLocations()
{
	// Stack each item inside container region
	for(int i = 0; i < this->_numItems; i++)
	{
		UIElement* currentItem = this->_items[i];
		if(i == 0)
		{	
			currentItem->LocX = this->LocX;
			currentItem->LocY = this->LocY;
		}
		else
		{
			UIElement* lastItem = this->_items[i - 1];	// -1 to get the previous element

			switch (_stackOrientation)
			{
				case STACK_ORIENTATION_VERTICAL:
					currentItem->LocX = lastItem->LocX;
					currentItem->LocY = lastItem->LocY + lastItem->Height + _marginBetweenElements;
					break;
				case STACK_ORIENTATION_HORIZONTAL:
					currentItem->LocX = lastItem->LocX + lastItem->Width + _marginBetweenElements;
					currentItem->LocY = lastItem->LocY;
					break;
				default:
					break;
			}

			this->RecalculateDimensions();
		}
	}
}