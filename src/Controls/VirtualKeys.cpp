/*
 * VirtualKeys.cpp
 */ 

#include "Controls/VirtualKeys.h"
#include "Core/UI_Manager.h"

#define VIRTUAL_KEYS_ELEMENT_MARGIN     5
#define VIRTUAL_KEYS_ELEMENT_PADDING    5

VirtualKeys::VirtualKeys(void(*onVirtualKeyPressed)(Keys_t key)) : UIElement(UI_CONTROL), 
    _grid(21, 5, 5),
    _button0("0", this, _onButton0Click),
    _button1("1", this, _onButton1Click),
    _button2("2", this, _onButton2Click),
    _button3("3", this, _onButton3Click),
    _button4("4", this, _onButton4Click),
    _button5("5", this, _onButton5Click),
    _button6("6", this, _onButton6Click),
    _button7("7", this, _onButton7Click),
    _button8("8", this, _onButton8Click),
    _button9("9", this, _onButton9Click),
    _buttonLeft("<-", this, _onButtonLeftClick),
    _buttonRight("->", this, _onButtonRightClick),
    _buttonMilli("m", this, _onButtonMilliClick),
    _buttonKilo("k", this, _onButtonKiloClick),
    _buttonX1("x1", this, _onButtonX1Click),
    _buttonMinus("-", this, _onButtonMinusClick),
    _buttonUp("UP", this, _onButtonUpClick),
    _buttonOk("OK", this, _onButtonOkClick),
    _buttonDown("DOWN", this, _onButtonDownClick),
    _buttonExitVirtualKeys("EXIT", this, _onButtonExitVirtualKeysClick)
{
    OnVirtualKeyPressed = onVirtualKeyPressed;
    ActiveChild = &_grid;
}

void VirtualKeys::_handleButtonPress(Keys_t key)
{
    if(OnVirtualKeyPressed != NULL) { OnVirtualKeyPressed(key); }
    if(_targetElement != NULL) { _targetElement->KeyInput(key); }
}

void VirtualKeys::Draw(bool redraw)
{ 
    uint16_t _previousMargin = UiManager.ElementMargin;
    uint16_t _previousPadding = UiManager.ElementPadding;
    UiManager.ElementMargin = VIRTUAL_KEYS_ELEMENT_MARGIN;
    UiManager.ElementPadding = VIRTUAL_KEYS_ELEMENT_PADDING;

    _grid.Draw(redraw);

    UiManager.ElementMargin = _previousMargin;
    UiManager.ElementPadding = _previousPadding;
}

void VirtualKeys::ShowVirtualKeys(UIElement* targetElement)
{
    if(targetElement->Type == UI_CONTROL)
    {
        // Save old visual tree root for later return
        _previousVisualTreeRoot = UiManager.GetVisualTreeRoot();
        _targetElement = targetElement;

        _grid.ClearAllItems();
        _grid.AddItem(&_button7, 0, 1, GRID_CELL_ALIGNMENT_MIDDLE);
        _grid.AddItem(&_button8, 1, 1, GRID_CELL_ALIGNMENT_MIDDLE);
        _grid.AddItem(&_button9, 2, 1, GRID_CELL_ALIGNMENT_MIDDLE);
        _grid.AddItem(&_buttonKilo, 3, 1, GRID_CELL_ALIGNMENT_MIDDLE);
        _grid.AddItem(&_buttonUp, 4, 1, GRID_CELL_ALIGNMENT_MIDDLE);
        _grid.AddItem(&_button4, 0, 2, GRID_CELL_ALIGNMENT_MIDDLE);
        _grid.AddItem(&_button5, 1, 2, GRID_CELL_ALIGNMENT_MIDDLE);
        _grid.AddItem(&_button6, 2, 2, GRID_CELL_ALIGNMENT_MIDDLE);
        _grid.AddItem(&_buttonMilli, 3, 2, GRID_CELL_ALIGNMENT_MIDDLE);
        _grid.AddItem(&_buttonOk, 4, 2, GRID_CELL_ALIGNMENT_MIDDLE);
        _grid.AddItem(&_button1, 0, 3, GRID_CELL_ALIGNMENT_MIDDLE);
        _grid.AddItem(&_button2, 1, 3, GRID_CELL_ALIGNMENT_MIDDLE);
        _grid.AddItem(&_button3, 2, 3, GRID_CELL_ALIGNMENT_MIDDLE);
        _grid.AddItem(&_buttonMinus, 3, 3, GRID_CELL_ALIGNMENT_MIDDLE);
        _grid.AddItem(&_buttonDown, 4, 3, GRID_CELL_ALIGNMENT_MIDDLE);
        _grid.AddItem(&_buttonLeft, 0, 4, GRID_CELL_ALIGNMENT_MIDDLE);
        _grid.AddItem(&_button0, 1, 4, GRID_CELL_ALIGNMENT_MIDDLE);
        _grid.AddItem(&_buttonRight, 2, 4, GRID_CELL_ALIGNMENT_MIDDLE);
        _grid.AddItem(&_buttonX1, 3, 4, GRID_CELL_ALIGNMENT_MIDDLE);
        _grid.AddItem(&_buttonExitVirtualKeys, 4, 4, GRID_CELL_ALIGNMENT_MIDDLE);
        _grid.AddItem(_targetElement, 0, 0, GRID_CELL_ALIGNMENT_MIDDLE, 5);
        _grid.InitItems();
    
        uint16_t _previousMargin = UiManager.ElementMargin;
        uint16_t _previousPadding = UiManager.ElementPadding;
        UiManager.ElementMargin = VIRTUAL_KEYS_ELEMENT_MARGIN;
        UiManager.ElementPadding = VIRTUAL_KEYS_ELEMENT_PADDING;
        UiManager.ChangeVisualTreeRoot(this);               // RecalculateLayout() is triggered by ChangeVisualTreeRoot()
        UiManager.ElementMargin = _previousMargin;
        UiManager.ElementPadding = _previousPadding;

        UiManager.AreVirtualKeysShown = true;
    }
}

void VirtualKeys::ExitVirtualKeys()
{
    if(_previousVisualTreeRoot != NULL)
    {
        UiManager.AreVirtualKeysShown = false;
        UiManager.ChangeVisualTreeRoot(_previousVisualTreeRoot);
    }
}

bool VirtualKeys::KeyInput(Keys_t key)
{
    return _grid.KeyInput(key);
}

bool VirtualKeys::TouchInput(uint16_t x, uint16_t y, TouchTypes touchType)
{
    return _grid.TouchInput(x, y, touchType);
}

void VirtualKeys::RecalculateDimensions()
{
    _grid.RecalculateDimensions();
}

void VirtualKeys::RecalculateLayout()
{
    _grid.RecalculateLayout();
}
