/*
 * ContainerList.cpp
 */ 

#include "Containers/ContainerList.h"
#include "Core/UI_Manager.h"

ContainerList::ContainerList(uint8_t maxNumItems, uint8_t scrollBarWidth, uint8_t scrollBarMargin) : Container(maxNumItems)
{
	this->Type = UI_CONTAINER;
	_lastDrawnItemIndex = -1;
	_scrollBarWidth = scrollBarWidth;
	_scrollBarMargin = scrollBarMargin;
}

ContainerList::ContainerList(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height, uint8_t maxNumItems, uint8_t scrollBarWidth, uint8_t scrollBarMargin) : Container(maxNumItems)
{
	this->Type = UI_CONTAINER;
	this->LocX = locX;
	this->LocY = locY;
	this->Width = width;
	this->Height = height;
	_lastDrawnItemIndex = -1;
	_scrollBarWidth = scrollBarWidth;
	_scrollBarMargin = scrollBarMargin;
}

void ContainerList::Draw(bool redraw)
{
	if (this->Visible)
	{
		redraw = redraw || (_lastDrawnItemIndex != this->_selectedItemIndex) || !this->_lastDrawnVisible;
		this->_lastDrawnVisible = true;
		if(redraw)
		{
			UiManager.Gfx->fillRect(this->LocX, this->LocY, this->Width, this->Height, UiManager.ColorBackground);
			_lastDrawnItemIndex = this->_selectedItemIndex;
		}

		UIElement* item = this->GetSelectedItem();
		if(item != NULL) { item->Draw(redraw); }
		
		if(redraw)
		{
			// Count visible items
			int _numVisibleItems = 0;
			int _numNonVisibleItemsBeforeSelected = 0;
			for(int i = 0; i < this->_numItems; i++)
			{
				if(this->_items[i]->Visible == true) { _numVisibleItems++; }
				if(this->_items[i]->Visible == false && i < this->_selectedItemIndex) { _numNonVisibleItemsBeforeSelected++; }
			}
			
			// Draw scroll bar
			uint16_t scrollBarArrowSize = _scrollBarWidth;		// height and width of the up and down arrows
			uint16_t scrollBarLeft = this->LocX + this->Width - _scrollBarWidth - _scrollBarMargin;
			uint16_t scrollBarBoxHeight = this->Height - 4 * _scrollBarMargin - 2 * scrollBarArrowSize - 2;	// -2 to take the outline into account
			uint16_t scrollBarHeight = scrollBarBoxHeight / _numVisibleItems;

			UiManager.Gfx->drawRect(scrollBarLeft, this->LocY + 2 * _scrollBarMargin + scrollBarArrowSize, _scrollBarWidth, scrollBarBoxHeight, UiManager.ColorForeground);
			UiManager.Gfx->fillRect(scrollBarLeft, this->LocY + 2 * _scrollBarMargin + scrollBarArrowSize + ((this->_selectedItemIndex - _numNonVisibleItemsBeforeSelected) * scrollBarHeight), _scrollBarWidth, scrollBarHeight, UiManager.ColorForeground);

			// Show Up Arrow if not the first element is selected
			if((this->_selectedItemIndex - _numNonVisibleItemsBeforeSelected) > 0)
			{
				UiManager.Gfx->fillTriangle(scrollBarLeft + (_scrollBarWidth / 2), this->LocY + _scrollBarMargin, scrollBarLeft, this->LocY + _scrollBarMargin + scrollBarArrowSize - 1, scrollBarLeft + scrollBarArrowSize, this->LocY + _scrollBarMargin + scrollBarArrowSize - 1, UiManager.ColorForeground);
			}
			// Show Down Arrow if not the last element is selected
			if(this->_selectedItemIndex < (_numVisibleItems - 1))
			{
				UiManager.Gfx->fillTriangle(scrollBarLeft + (_scrollBarWidth / 2), this->LocY + this->Height - _scrollBarMargin - 1, scrollBarLeft, this->LocY + this->Height - _scrollBarMargin - scrollBarArrowSize, scrollBarLeft + scrollBarArrowSize, this->LocY + this->Height - _scrollBarMargin - scrollBarArrowSize, UiManager.ColorForeground);
			}
		}
	}
	else if(!this->Visible && this->_lastDrawnVisible)		// clear only when the Visible property changes from true to false
	{
		this->_lastDrawnVisible = false;
		UiManager.Gfx->fillRect(this->LocX, this->LocY, this->Width, this->Height, UiManager.ColorBackground);
	}
}

bool ContainerList::KeyInput(Keys_t key)
{
	switch (key)
	{
		case KEYUP:
			return this->NextItem();
		case KEYDOWN:
			return this->PreviousItem();
		default:
			return false;
	}
}

void ContainerList::RecalculateDimensions()
{
	if(this->Parent != NULL)
	{
		// Nothing to do here. Size is updated by parent container.
	}
	else
	{
		// The ContainerList is the top most control, size it to fill the complete screen
		this->Width = UiManager.Gfx->width();
		this->Height = UiManager.Gfx->height();
	}
}

void ContainerList::RecalculateLayout()
{
	// Move each item inside container region
	for(int i = 0; i < this->_numItems; i++)
	{
		UIElement* currentItem = this->_items[i];
		currentItem->LocX = this->LocX;
		currentItem->LocY = this->LocY;
		currentItem->RecalculateLayout();
		currentItem->RecalculateDimensions();
	}
}