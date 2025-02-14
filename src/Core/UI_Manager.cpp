/*
 * UI_Manager.cpp
 */ 

#include "Core/UI_Manager.h"

UI_Manager UiManager = UI_Manager::getInstance();

UI_Manager::UI_Manager()
{
	_visualTreeRoot = NULL;
	_focusElement = NULL;
	_lastDrawnFocusElement = NULL;

	SetColors();		// use default colors

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

void UI_Manager::SetColors(uint16_t colorBackground, uint16_t colorForeground, uint16_t colorForegroundEditMode, uint16_t colorFocusFrame)
{
	ColorBackground = colorBackground;
	ColorForeground = colorForeground;
	ColorForegroundEditMode = colorForegroundEditMode;
	ColorFocusFrame = colorFocusFrame;
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
		UiManager.Gfx->drawRect(_lastDrawnFocusElement->LocX + UiManager.ElementMargin - 2, _lastDrawnFocusElement->LocY + UiManager.ElementMargin - 2, _lastDrawnFocusElement->Width - 2 * UiManager.ElementMargin + 4, _lastDrawnFocusElement->Height - 2 * UiManager.ElementMargin + 4, ColorBackground); 
	}

	_visualTreeRoot->Draw(CompleteRedrawRequested);
	
	if(_focusElement != NULL && _focusElement->Visible && _focusElement->Type != UI_INDICATOR) 
	{
		// Upper left corner
		UiManager.Gfx->drawFastHLine(_focusElement->LocX + UiManager.ElementMargin - 2, _focusElement->LocY + UiManager.ElementMargin - 2, 5, UiManager.ColorFocusFrame);
		UiManager.Gfx->drawFastVLine(_focusElement->LocX + UiManager.ElementMargin - 2, _focusElement->LocY + UiManager.ElementMargin - 2, 5, UiManager.ColorFocusFrame);
		// Upper right corner
		UiManager.Gfx->drawFastHLine(_focusElement->LocX + _focusElement->Width - UiManager.ElementMargin - 4, _focusElement->LocY + UiManager.ElementMargin - 2, 5, UiManager.ColorFocusFrame);
		UiManager.Gfx->drawFastVLine(_focusElement->LocX + _focusElement->Width - UiManager.ElementMargin + 1, _focusElement->LocY + UiManager.ElementMargin - 2, 5, UiManager.ColorFocusFrame);
		// Lower left corner
		UiManager.Gfx->drawFastHLine(_focusElement->LocX + UiManager.ElementMargin - 2, _focusElement->LocY + _focusElement->Height - UiManager.ElementMargin + 1, 5, UiManager.ColorFocusFrame);
		UiManager.Gfx->drawFastVLine(_focusElement->LocX + UiManager.ElementMargin - 2, _focusElement->LocY + _focusElement->Height - UiManager.ElementMargin - 4, 5, UiManager.ColorFocusFrame);
		// Lower right corner
		UiManager.Gfx->drawFastHLine(_focusElement->LocX + _focusElement->Width - UiManager.ElementMargin - 4, _focusElement->LocY + _focusElement->Height - UiManager.ElementMargin + 1, 5, UiManager.ColorFocusFrame);		
		UiManager.Gfx->drawFastVLine(_focusElement->LocX + _focusElement->Width - UiManager.ElementMargin + 1, _focusElement->LocY + _focusElement->Height - UiManager.ElementMargin - 3, 5, UiManager.ColorFocusFrame);
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
	visualTreeRoot->RecalculateDimensions();
	visualTreeRoot->RecalculateLayout();
	setFocusToLeaf();
	CompleteRedrawRequested = true;
}

UIElement* UI_Manager::GetVisualTreeRoot()
{
	return _visualTreeRoot;
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

bool UI_Manager::TouchInput(uint16_t x, uint16_t y, TouchTypes touchType)
{
	if (_visualTreeRoot == NULL) { return false; }
	bool touchProcessed = _visualTreeRoot->TouchInput(x, y, touchType);
	setFocusToLeaf();

	// The focus element change to an UIElement other than the one that was in edit mode before
	if(_focusElement != _lastElementInEditMode && _focusElement != NULL && _lastElementInEditMode != NULL && !UiManager.AreVirtualKeysShown)
	{
		_lastElementInEditMode->IsInEditMode = false;
	}
	
	return touchProcessed;
}

void UI_Manager::UpdateIsInEditModeElement(UIElement* element, bool newEditModeState)
{
	if(element == NULL) { return; }
	if(_lastElementInEditMode != NULL)
	{
		_lastElementInEditMode->IsInEditMode = false;		// Disable the edit mode in the last element
	}
	element->IsInEditMode = newEditModeState;				// Set the edit mode in the current element

	if(newEditModeState)
	{
		_lastElementInEditMode = element;
	}
	else
	{
		_lastElementInEditMode = NULL;
	}
}