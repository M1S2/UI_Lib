/*
 * UI_Lib_Test.cpp
 *
 */ 

#include "UI_Lib_Test_Touch_Demo.h"
#include <Fonts/FreeMono18pt7b.h>

bool boolVal1 = false;
bool boolVal2 = true;

void OnButtonClick(void* context);
void OnMsgOk(void* context);

#define DISPLAY_WIDTH	320
#define DISPLAY_HEIGHT	240
#define ELEMENT_MARGIN	10

#define COLOR_WHITE		RGB565(0xFF, 0xFF, 0xFF)
#define COLOR_ORANGE	RGB565(0xFF, 0x88, 0x00)

Label labelBool("Boolean", COLOR_WHITE, NULL, 0, 0, 10);
BoolIndicator boolInd1(&boolVal1);
BoolControl boolCtrl1(&boolVal1, &boolVal1);
BoolControl boolCtrl2(&boolVal2, &boolVal2);
ButtonControl buttonCtrl("Dialog", NULL, &OnButtonClick);
ContainerStack stack_boolean(STACK_LAYOUT_VERTICAL_LEFT);
ContainerPage page_boolean;

MessageDialog msgTest(ELEMENT_MARGIN, ELEMENT_MARGIN, DISPLAY_WIDTH - 2 * ELEMENT_MARGIN, DISPLAY_HEIGHT - 2 * ELEMENT_MARGIN, "Test message.", MSG_INFO, MSG_BTN_OK, NULL, &OnMsgOk);

ContainerPage mainPage(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
Label labelUILib("UI LIB TOUCH", COLOR_ORANGE, &FreeMono18pt7b);

void OnButtonClick(void* context)
{
	UiManager.ChangeVisualTreeRoot(&msgTest);
}

void OnMsgOk(void* context)
{
	UiManager.ChangeVisualTreeRoot(&mainPage);	
}

UIElement* build_screen_boolean()
{
	stack_boolean.AddItem(&labelBool);
	stack_boolean.AddItem(&boolInd1);
	stack_boolean.AddItem(&boolCtrl1);
	stack_boolean.AddItem(&boolCtrl2);
	stack_boolean.AddItem(&buttonCtrl);
	stack_boolean.InitItems();
	page_boolean.AddItem(&stack_boolean);
	page_boolean.InitItems();
	return &page_boolean;
}

void UI_Test_BuildTree()
{
	mainPage.AddItem(build_screen_boolean());
	mainPage.AddItem(&labelUILib);
	
	UiManager.ChangeVisualTreeRoot(&mainPage);	
}

void UI_Test_Init(Adafruit_GFX* gfx)
{
	// Use the global UiManager object to access the singleton class.
	UiManager.SetColors(RGB565(0x00, 0x00, 0x00), RGB565(0x00, 0xF7, 0x00), RGB565(0x00, 0x00, 0x00));
	UiManager.Init(gfx);
	UiManager.ElementMargin = 5;
}

void UI_Test_Draw()
{
	UiManager.Draw();
}

void UI_Test_KeyInput(Keys_t key)
{
	UiManager.KeyInput(key);
}

void UI_Test_TouchInput(uint16_t x, uint16_t y)
{
	UiManager.TouchInput(x, y);
}