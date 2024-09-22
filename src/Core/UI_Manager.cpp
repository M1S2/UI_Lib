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
	gfx->fillScreen(UI_LIB_COLOR_BACKGROUND);
	gfx->setTextColor(UI_LIB_COLOR_FOREGROUND);
	gfx->setFont(UI_LIB_DEFAULT_FONT);
	gfx->setTextWrap(false);
}

void UI_Manager::Draw(Adafruit_GFX* gfx)
{
	if(_wasTreeRootChanged)
	{
		gfx->fillScreen(UI_LIB_COLOR_BACKGROUND);
	}

	if (_visualTreeRoot == NULL) { return; }
	
	_visualTreeRoot->Draw(gfx, _wasTreeRootChanged);
	
	if(_focusElement != NULL && _focusElement->Visible && _focusElement->Type != UI_INDICATOR) 
	{ 
		gfx->drawRect(_focusElement->LocX - 1, _focusElement->LocY - 1, _focusElement->Width + 2, _focusElement->Height + 2, UI_LIB_COLOR_FOREGROUND); 
	}

	if(_wasTreeRootChanged)
	{
		_wasTreeRootChanged = false;
	}
}

void UI_Manager::ChangeVisualTreeRoot(UIElement* visualTreeRoot)
{
	_visualTreeRoot = visualTreeRoot;
	setFocusToLeaf();
	_wasTreeRootChanged = true;
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