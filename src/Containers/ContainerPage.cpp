/*
 * ContainerPage.cpp
 */ 

#include "Containers/ContainerPage.h"

template <uint8_t maxItems>
ContainerPage<maxItems>::ContainerPage()
{
	this->Type = UI_CONTAINER;
}

template <uint8_t maxItems>
ContainerPage<maxItems>::ContainerPage(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height)
{
	this->Type = UI_CONTAINER;
	this->LocX = locX;
	this->LocY = locY;
	this->Width = width;
	this->Height = height;
}

template <uint8_t maxItems>
void ContainerPage<maxItems>::Draw()
{
	for (int i = 0; i < this->_numItems; i++)
	{
		this->_items[i]->Draw();
	}
}

template <uint8_t maxItems>
bool ContainerPage<maxItems>::KeyInput(Keys_t key)
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
void ContainerPage<maxItems>::InitItems()
{
	this->_selectedItemIndex = 0;
	if (this->GetSelectedItem()->Type != UI_CONTROL || this->GetSelectedItem()->Visible == false)
	{
		this->NextControlItem();
	}
}

template <uint8_t maxItems>
void ContainerPage<maxItems>::RecalculateDimensions()
{
	uint16_t x, y, w, h;
	this->GetItemsBoundingBox(&x, &y, &w, &h);
	this->Width = w;		// resize the ContainerPage to fit the size of the bounding box
	this->Height = h;
}

template <uint8_t maxItems>
void ContainerPage<maxItems>::RecalculateItemLocations()
{
	// Nothing recalculated here (yet)
}