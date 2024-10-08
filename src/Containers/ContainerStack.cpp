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
		ContainerPage<maxItems>::Draw();
	}
	else
	{
		UiManager.Gfx->fillRect(this->LocX, this->LocY, this->Width, this->Height, UiManager.ColorBackground);
	}
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