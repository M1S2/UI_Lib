/*
 * ContainerStack.cpp
 */ 

#include "Containers/ContainerStack.h"
#include "Core/UI_Manager.h"

template <uint8_t maxItems>
ContainerStack<maxItems>::ContainerStack(StackLayout_t stackLayout, uint16_t marginBetweenElements)
{
	this->Type = UI_CONTAINER;
	_stackLayout = stackLayout;
	_marginBetweenElements = marginBetweenElements;
}

template <uint8_t maxItems>
ContainerStack<maxItems>::ContainerStack(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height, StackLayout_t stackLayout, uint16_t marginBetweenElements)
{
	this->Type = UI_CONTAINER;
	this->LocX = locX;
	this->LocY = locY;
	this->Width = width;
	this->Height = height;
	_stackLayout = stackLayout;
	_marginBetweenElements = marginBetweenElements;
}

template <uint8_t maxItems>
void ContainerStack<maxItems>::Draw(bool redraw)
{
	if (this->Visible)
	{
		this->_lastDrawnVisible = true;
		for (int i = 0; i < this->_numItems; i++)
		{
			this->_items[i]->Draw(redraw);
		}
	}
	else if(!this->Visible && this->_lastDrawnVisible)		// clear only when the Visible property changes from true to false
	{
		this->_lastDrawnVisible = false;
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
void ContainerStack<maxItems>::RecalculateLayout()
{
	uint16_t largestItemDimension = 0;			// Width of the largest item if STACK_LAYOUT_VERTICAL, Height of the largest item if STACK_LAYOUT_HORIZONTAL
	for(int i = 0; i < this->_numItems; i++)
	{
		UIElement* currentItem = this->_items[i];
		currentItem->RecalculateLayout();
		currentItem->RecalculateDimensions();
		switch (_stackLayout)
		{
			case STACK_LAYOUT_VERTICAL_LEFT:
			case STACK_LAYOUT_VERTICAL_CENTER:
			case STACK_LAYOUT_VERTICAL_RIGHT:
				if(currentItem->Width > largestItemDimension) { largestItemDimension = currentItem->Width; }
				break;
			case STACK_LAYOUT_HORIZONTAL_TOP:
			case STACK_LAYOUT_HORIZONTAL_CENTER:
			case STACK_LAYOUT_HORIZONTAL_BOTTOM:
				if(currentItem->Height > largestItemDimension) { largestItemDimension = currentItem->Height; }
				break;
			default: break;
		}	
	}

	// Stack each item inside container region
	for(int i = 0; i < this->_numItems; i++)
	{
		UIElement* currentItem = this->_items[i];
		UIElement* lastItem = NULL;
		if(i > 0) { lastItem = this->_items[i - 1]; }	// -1 to get the previous element

		switch (_stackLayout)
		{
			case STACK_LAYOUT_VERTICAL_LEFT:
				currentItem->LocX = this->LocX;
				currentItem->LocY = (lastItem == NULL) ? this->LocY : (lastItem->LocY + lastItem->Height + _marginBetweenElements);
				break;
			case STACK_LAYOUT_VERTICAL_CENTER:
				currentItem->LocX = this->LocX + ((largestItemDimension - currentItem->Width) / 2);
				currentItem->LocY = (lastItem == NULL) ? this->LocY : (lastItem->LocY + lastItem->Height + _marginBetweenElements);
				break;
			case STACK_LAYOUT_VERTICAL_RIGHT:
				currentItem->LocX = this->LocX + (largestItemDimension - currentItem->Width);
				currentItem->LocY = (lastItem == NULL) ? this->LocY : (lastItem->LocY + lastItem->Height + _marginBetweenElements);
				break;
			case STACK_LAYOUT_HORIZONTAL_TOP:
				currentItem->LocX = (lastItem == NULL) ? this->LocX : (lastItem->LocX + lastItem->Width + _marginBetweenElements);
				currentItem->LocY = this->LocY;
				break;
			case STACK_LAYOUT_HORIZONTAL_CENTER:
				currentItem->LocX = (lastItem == NULL) ? this->LocX : (lastItem->LocX + lastItem->Width + _marginBetweenElements);
				currentItem->LocY = this->LocY + ((largestItemDimension - currentItem->Height) / 2);
				break;
			case STACK_LAYOUT_HORIZONTAL_BOTTOM:
				currentItem->LocX = (lastItem == NULL) ? this->LocX : (lastItem->LocX + lastItem->Width + _marginBetweenElements);
				currentItem->LocY = this->LocY + (largestItemDimension - currentItem->Height);
				break;
			default: break;
		}

		currentItem->RecalculateLayout();
	}
	
	this->RecalculateDimensions();
}