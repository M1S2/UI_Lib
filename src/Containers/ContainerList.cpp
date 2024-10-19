/*
 * ContainerList.cpp
 */ 

#include "Containers/ContainerList.h"
#include "Core/UI_Manager.h"

template <uint8_t maxItems, uint8_t scrollBarWidth, uint8_t scrollBarMargin>
ContainerList<maxItems, scrollBarWidth, scrollBarMargin>::ContainerList()
{
	this->Type = UI_CONTAINER;
	_lastDrawnItemIndex = -1;
}

template <uint8_t maxItems, uint8_t scrollBarWidth, uint8_t scrollBarMargin>
ContainerList<maxItems, scrollBarWidth, scrollBarMargin>::ContainerList(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height)
{
	this->Type = UI_CONTAINER;
	this->LocX = locX;
	this->LocY = locY;
	this->Width = width;
	this->Height = height;
	_lastDrawnItemIndex = -1;
}

template <uint8_t maxItems, uint8_t scrollBarWidth, uint8_t scrollBarMargin>
void ContainerList<maxItems, scrollBarWidth, scrollBarMargin>::Draw(bool redraw)
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
			uint16_t scrollBarArrowSize = scrollBarWidth;		// height and width of the up and down arrows
			uint16_t scrollBarLeft = this->LocX + this->Width - scrollBarWidth - scrollBarMargin;
			uint16_t scrollBarBoxHeight = this->Height - 4 * scrollBarMargin - 2 * scrollBarArrowSize - 2;	// -2 to take the outline into account
			uint16_t scrollBarHeight = scrollBarBoxHeight / _numVisibleItems;

			UiManager.Gfx->drawRect(scrollBarLeft, this->LocY + 2 * scrollBarMargin + scrollBarArrowSize, scrollBarWidth, scrollBarBoxHeight, UiManager.ColorForeground);
			UiManager.Gfx->fillRect(scrollBarLeft, this->LocY + 2 * scrollBarMargin + scrollBarArrowSize + ((this->_selectedItemIndex - _numNonVisibleItemsBeforeSelected) * scrollBarHeight), scrollBarWidth, scrollBarHeight, UiManager.ColorForeground);

			// Show Up Arrow if not the first element is selected
			if((this->_selectedItemIndex - _numNonVisibleItemsBeforeSelected) > 0)
			{
				UiManager.Gfx->fillTriangle(scrollBarLeft + (scrollBarWidth / 2), this->LocY + scrollBarMargin, scrollBarLeft, this->LocY + scrollBarMargin + scrollBarArrowSize - 1, scrollBarLeft + scrollBarArrowSize, this->LocY + scrollBarMargin + scrollBarArrowSize - 1, UiManager.ColorForeground);
			}
			// Show Down Arrow if not the last element is selected
			if(this->_selectedItemIndex < (_numVisibleItems - 1))
			{
				UiManager.Gfx->fillTriangle(scrollBarLeft + (scrollBarWidth / 2), this->LocY + this->Height - scrollBarMargin - 1, scrollBarLeft, this->LocY + this->Height - scrollBarMargin - scrollBarArrowSize, scrollBarLeft + scrollBarArrowSize, this->LocY + this->Height - scrollBarMargin - scrollBarArrowSize, UiManager.ColorForeground);
			}
		}
	}
	else if(!this->Visible && this->_lastDrawnVisible)		// clear only when the Visible property changes from true to false
	{
		this->_lastDrawnVisible = false;
		UiManager.Gfx->fillRect(this->LocX, this->LocY, this->Width, this->Height, UiManager.ColorBackground);
	}
}

template <uint8_t maxItems, uint8_t scrollBarWidth, uint8_t scrollBarMargin>
bool ContainerList<maxItems, scrollBarWidth, scrollBarMargin>::KeyInput(Keys_t key)
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

template <uint8_t maxItems, uint8_t scrollBarWidth, uint8_t scrollBarMargin>
void ContainerList<maxItems, scrollBarWidth, scrollBarMargin>::RecalculateDimensions()
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

template <uint8_t maxItems, uint8_t scrollBarWidth, uint8_t scrollBarMargin>
void ContainerList<maxItems, scrollBarWidth, scrollBarMargin>::RecalculateLayout()
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