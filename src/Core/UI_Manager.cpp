/*
 * UI_Manager.cpp
 */ 

#include "Core/UI_Manager.h"
#include "Core/UI_Elements.h"

UI_Manager UiManager = UI_Manager::getInstance();

UI_Manager::UI_Manager()
{
	_visualTreeRoot = NULL;
	_focusElement = NULL;
	_lastDrawnFocusElement = NULL;

	SetColors(UI_LIB_DEFAULT_COLOR_BACKGROUND, UI_LIB_DEFAULT_COLOR_FOREGROUND, UI_LIB_DEFAULT_COLOR_FOREGROUND_EDIT_MODE);

	Font = UI_LIB_DEFAULT_FONT;
	ElementMargin = UI_LIB_DEFAULT_ELEMENT_MARGIN;
	ElementPadding = UI_LIB_DEFAULT_ELEMENT_PADDING;
}

void UI_Manager::Init(Adafruit_GFX* gfx)
{
	Gfx = gfx;
	UiManager.Gfx->fillScreen(ColorBackground);
	UiManager.Gfx->setTextColor(ColorForeground);
	UiManager.Gfx->setTextWrap(false);
	SetFont(Font);
}

void UI_Manager::SetColors(uint16_t colorBackground, uint16_t colorForeground, uint16_t colorForegroundEditMode)
{
	ColorBackground = colorBackground;
	ColorForeground = colorForeground;
	ColorForegroundEditMode = colorForegroundEditMode;
}

void UI_Manager::SetFont(const GFXfont* font)
{
	Font = font;
	UiManager.Gfx->setFont(font);

	int16_t x, y;
	uint16_t w, h;
	UiManager.Gfx->getTextBounds("Ag", 0, 0, &x, &y, &w, &h);
	FontHeight = h;
}

void UI_Manager::Draw()
{
	if(CompleteRedrawRequested)
	{
		UiManager.Gfx->fillScreen(ColorBackground);
	}

	if (_visualTreeRoot == NULL) { return; }
	
	if(_lastDrawnFocusElement != _focusElement)
	{
		UiManager.Gfx->drawRect(_lastDrawnFocusElement->LocX + UiManager.ElementMargin, _lastDrawnFocusElement->LocY + UiManager.ElementMargin, _lastDrawnFocusElement->Width - 2 * UiManager.ElementMargin, _lastDrawnFocusElement->Height - 2 * UiManager.ElementMargin, ColorBackground); 
	}

	_visualTreeRoot->Draw(CompleteRedrawRequested);
	
	if(_focusElement != NULL && _focusElement->Visible && _focusElement->Type != UI_INDICATOR) 
	{ 
		UiManager.Gfx->drawRect(_focusElement->LocX + UiManager.ElementMargin, _focusElement->LocY + UiManager.ElementMargin, _focusElement->Width - 2 * UiManager.ElementMargin, _focusElement->Height - 2 * UiManager.ElementMargin, ColorForeground); 
	}
	_lastDrawnFocusElement = _focusElement;

	if(CompleteRedrawRequested)
	{
		CompleteRedrawRequested = false;
	}
}

void UI_Manager::ChangeVisualTreeRoot(UIElement* visualTreeRoot)
{
	_visualTreeRoot = visualTreeRoot;
	setFocusToLeaf();
	visualTreeRoot->RecalculateDimensions();
	visualTreeRoot->RecalculateLayout();
	CompleteRedrawRequested = true;
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