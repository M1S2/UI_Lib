/*
 * ContainerPage.cpp
 */ 

#include "Containers/ContainerPage.h"
#include "Core/UI_Manager.h"

ContainerPage::ContainerPage(uint8_t maxNumItems) : Container(maxNumItems)
{
	_itemConfiguration = new PageItemConfig[maxNumItems]();
	this->Type = UI_CONTAINER;
}

ContainerPage::ContainerPage(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height, uint8_t maxNumItems) : Container(maxNumItems)
{
	_itemConfiguration = new PageItemConfig[maxNumItems]();
	this->Type = UI_CONTAINER;
	this->LocX = locX;
	this->LocY = locY;
	this->Width = width;
	this->Height = height;
}

void ContainerPage::Draw(bool redraw)
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

bool ContainerPage::KeyInput(Keys_t key)
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

bool ContainerPage::TouchInput(uint16_t x, uint16_t y)
{
	if(HitTest(x, y))
	{
		for (int i = 0; i < this->_numItems; i++)
		{
			bool touchResult = this->_items[i]->TouchInput(x, y);
			if(touchResult)
			{
				this->ActiveChild = this->_items[i];
				return true;
			}
		}
	}
	return false;
}

void ContainerPage::InitItems()
{
	this->_selectedItemIndex = 0;
	if (this->GetSelectedItem()->Type != UI_CONTROL || this->GetSelectedItem()->Visible == false)
	{
		this->NextControlItem();
	}
}

bool ContainerPage::AddItem(UIElement* item)
{
	PageItemConfig itemConfig;
	itemConfig.item = item;
	itemConfig.relativeX = item->LocX;		// the X and Y coordinates at time of adding are treated as relative coordinates inside the parent container.
	itemConfig.relativeY = item->LocY;
	_itemConfiguration[this->_numItems] = itemConfig;

	if(!Container::AddItem(item)) { return false; }

	return true;
}

void ContainerPage::RecalculateDimensions()
{
	uint16_t x, y, w, h;
	this->GetItemsBoundingBox(&x, &y, &w, &h);
	this->Width = w;		// resize the ContainerPage to fit the size of the bounding box
	this->Height = h;
}

void ContainerPage::RecalculateLayout()
{
	for(int i = 0; i < this->_numItems; i++)
	{
		UIElement* currentItem = this->_items[i];
		PageItemConfig currentItemConfig = _itemConfiguration[i];

		currentItem->LocX = this->LocX + currentItemConfig.relativeX;
		currentItem->LocY = this->LocY + currentItemConfig.relativeY;
		currentItem->RecalculateLayout();
		currentItem->RecalculateDimensions();
	}
	this->RecalculateDimensions();
}