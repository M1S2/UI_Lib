/*
 * ContainerPage.cpp
 */ 

#include "Containers/ContainerPage.h"

ContainerPage::ContainerPage()
{
	Type = UI_CONTAINER;
}

ContainerPage::ContainerPage(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height)
{
	Type = UI_CONTAINER;
	LocX = locX;
	LocY = locY;
	Width = width;
	Height = height;
}

void ContainerPage::Draw(Adafruit_GFX* gfx)
{
	for (int i = 0; i < _numItems; i++)
	{
		_items[i]->Draw(gfx);
	}
}

bool ContainerPage::KeyInput(Keys_t key)
{
	switch (key)
	{
		case KEYUP:
			return NextControlItem();
		case KEYDOWN:
			return PreviousControlItem();
		default:
			return false;
	}
}

void ContainerPage::InitItems()
{
	_selectedItemIndex = 0;
	if (GetSelectedItem()->Type != UI_CONTROL || GetSelectedItem()->Visible == false)
	{
		NextControlItem();
	}
}