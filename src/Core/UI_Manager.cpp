/*
 * UI_Manager.cpp
 */ 

#include "Core/UI_Manager.h"
#include "Core/UI_Elements.h"

UI_Manager UiManager;

UI_Manager::UI_Manager()
{
	_visualTreeRoot = NULL;
	_focusElement = NULL;
	
	ColorBackground = UI_LIB_DEFAULT_COLOR_BACKGROUND;
	ColorForeground = UI_LIB_DEFAULT_COLOR_FOREGROUND;
	ColorForegroundEditMode = UI_LIB_DEFAULT_COLOR_FOREGROUND_EDIT_MODE;
	DefaultFont = UI_LIB_DEFAULT_FONT;
}

void UI_Manager::Init(Adafruit_GFX* gfx)
{
	gfx->fillScreen(ColorBackground);
	gfx->setTextColor(ColorForeground);
	gfx->setFont(DefaultFont);
	gfx->setTextWrap(false);
}

void UI_Manager::Draw(Adafruit_GFX* gfx)
{
	if(WasTreeRootChanged)
	{
		gfx->fillScreen(ColorBackground);
	}

	if (_visualTreeRoot == NULL) { return; }
	
	_visualTreeRoot->Draw(gfx);
	
	if(_focusElement != NULL && _focusElement->Visible && _focusElement->Type != UI_INDICATOR) 
	{ 
		gfx->drawRect(_focusElement->LocX - 1, _focusElement->LocY - 1, _focusElement->Width + 2, _focusElement->Height + 2, ColorForeground); 
	}

	if(WasTreeRootChanged)
	{
		WasTreeRootChanged = false;
	}
}

void UI_Manager::ChangeVisualTreeRoot(UIElement* visualTreeRoot)
{
	_visualTreeRoot = visualTreeRoot;
	setFocusToLeaf();
	WasTreeRootChanged = true;
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