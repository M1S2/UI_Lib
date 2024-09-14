/*
 * UI_Manager.cpp
 */ 

#include "Core/UI_Manager.h"
#include "Core/UI_Elements.h"

UI_Manager::UI_Manager()
{
	_visualTreeRoot = NULL;
	_focusElement = NULL;
}

void UI_Manager::Init(Adafruit_GFX* gfx)
{
	gfx->setTextColor(DEFAULT_UI_ELEMENT_COLOR);
	gfx->setFont(DEFAULT_FONT);
}

void UI_Manager::Draw(Adafruit_GFX* gfx, bool isFirstPage)
{
	gfx->fillScreen(BACKGROUND_COLOR);

	if (_visualTreeRoot == NULL) { return; }
	
	if(_focusElement != NULL && _focusElement->Visible && _focusElement->Type != UI_INDICATOR) 
	{ 
		gfx->drawRect(_focusElement->LocX - 1, _focusElement->LocY - 1, _focusElement->Width + 2, _focusElement->Height + 2, DEFAULT_UI_ELEMENT_COLOR); 
	}
	_visualTreeRoot->Draw(gfx, isFirstPage);
}

void UI_Manager::ChangeVisualTreeRoot(UIElement* visualTreeRoot)
{
	_visualTreeRoot = visualTreeRoot;
	setFocusToLeaf();
}

void UI_Manager::setFocusToLeaf()
{
	if (_visualTreeRoot == NULL) { return; }
	
	UIElement* visualTreeElement = _visualTreeRoot;
	UIElement* leaf = NULL;

	while (visualTreeElement != NULL)
	{
		leaf = visualTreeElement;
		visualTreeElement = visualTreeElement->ActiveChild;		// Traverse down the tree
	}
	_focusElement = leaf;
}

bool UI_Manager::KeyInput(Keys_t key)
{
	bool keyProcessed = false;
	UIElement* visualTreeElement = _focusElement;
	while (!keyProcessed && visualTreeElement != NULL)
	{
		keyProcessed = visualTreeElement->KeyInput(key);
		if (!keyProcessed)
		{
			visualTreeElement = visualTreeElement->Parent;		// Traverse up the tree
		}
	}
	setFocusToLeaf();
	return keyProcessed;
}