/*
 * UI_Lib_Test.cpp
 *
 */ 

#include "UI_Lib_Test.h"
#include <Fonts/FreeMono18pt7b.h>

enum TestEnum
{
	Test_A,
	Test_B,
	Test_C
};
const char* TestEnumNames[] = { "Test A", "Test B", "Test C" };

UI_Manager ui_Manager;
bool boolVal1;
TestEnum enumVal1;
float numVal1 = 223.456;
int numVal2 = 123;

void OnBoolVal1Changed(void* context);
void OnNumVal1Changed(void* context);
void OnButtonReset(void* context);
void OnMsgOk(void* context);
void OnShowError(void* context);

#define DISPLAY_WIDTH	320
#define DISPLAY_HEIGHT	240

#define TAB_WIDTH	70
#define X_COLUMN1	TAB_WIDTH + 5
#define X_COLUMN2	X_COLUMN1 + 100
#define Y_ROW1		5
#define Y_ROW2		30
#define Y_ROW3		55		
#define Y_ROW4		80		

Label<10> labelBool(X_COLUMN1, Y_ROW1, "Boolean");
BoolIndicator boolInd1(X_COLUMN1, Y_ROW2, &boolVal1);
BoolControl boolCtrl1(X_COLUMN1, Y_ROW3, &boolVal1, &boolVal1, &OnBoolVal1Changed);
ContainerPage page_boolean;
Label<15> labelEnum(X_COLUMN1, Y_ROW1, "Enumerations");
EnumIndicator<TestEnum> enumInd1(X_COLUMN1 + 16, Y_ROW2, &enumVal1, TestEnumNames, 3);
EnumControl<TestEnum> enumCtrl1(X_COLUMN1 + 16, Y_ROW3, &enumVal1, TestEnumNames, 3);
Icon enumCtrl1Icon(X_COLUMN1, Y_ROW3, ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
ContainerPage page_enum;
ContainerList list1(X_COLUMN1, 0, DISPLAY_WIDTH - TAB_WIDTH, DISPLAY_HEIGHT);

Label<10> labelNum(X_COLUMN1, Y_ROW1, "Numerics");
NumericIndicator<int> numInd2(X_COLUMN1, Y_ROW2, &numVal2, "A", 5000, 0);
NumericIndicator<float> numInd1(X_COLUMN2, Y_ROW2, &numVal1, "V", 2000, 3);
NumericControl<float> numCtrl1(X_COLUMN2, Y_ROW3, &numVal1, "V", -10, 2000, 3, &numVal1, &OnNumVal1Changed);
ProgressBar<float> progress1(X_COLUMN2, Y_ROW4, 70, 20, &numVal1, -10, 2000, PROGRESSBAR_ORIGIN_ZERO, 0);
ContainerPage page_numeric;

ButtonControlDefault buttonReset(X_COLUMN1, Y_ROW1, DEFAULT_UI_ELEMENT_WIDTH, DEFAULT_UI_ELEMENT_HEIGHT, "Reset", NULL, &OnButtonReset);
ButtonControlDefault buttonShowTestError(X_COLUMN1, Y_ROW3, 125, DEFAULT_UI_ELEMENT_HEIGHT, "Show Error", NULL, &OnShowError);
MessageDialogDefault msgReset(5, Y_ROW1, DISPLAY_WIDTH - 5, DISPLAY_HEIGHT - Y_ROW1, "Reset sucessful.", MSG_INFO, MSG_BTN_OK, NULL, &OnMsgOk);
MessageDialogDefault msgTestWarning(5, Y_ROW1, DISPLAY_WIDTH - 5, DISPLAY_HEIGHT - Y_ROW1, "Warning message.\nWith Newline.", MSG_WARNING, MSG_BTN_OK, NULL, &OnMsgOk);
MessageDialogDefault msgTestError(5, Y_ROW1, DISPLAY_WIDTH - 5, DISPLAY_HEIGHT - Y_ROW1, "Error message.", MSG_ERROR, MSG_BTN_OK, NULL, &OnMsgOk);
ContainerPage page_dialogs;

TabControl tabControl(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, TAB_WIDTH);

ContainerPage mainPage;
LabelDefault labelUILib(185, Y_ROW1, "UI LIB", &FreeMono18pt7b, RGB565(0xFF, 0x88, 00));
Icon speedIcon(DISPLAY_WIDTH - 25, Y_ROW3, ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);

void OnBoolVal1Changed(void* context)
{
	bool boolVal = *((bool*)context);
	speedIcon.Visible = boolVal;
	ui_Manager.ChangeVisualTreeRoot(&msgTestWarning);
}

void OnNumVal1Changed(void* context)
{
	numVal2++;	
}

void OnButtonReset(void* context)
{
	boolVal1 = false;
	enumVal1 = Test_A;
	numVal1 = 0;
	numVal2 = 0;
	ui_Manager.ChangeVisualTreeRoot(&msgReset);
}

void OnShowError(void* context)
{
	ui_Manager.ChangeVisualTreeRoot(&msgTestError);
}

void OnMsgOk(void* context)
{
	ui_Manager.ChangeVisualTreeRoot(&mainPage);	
}

void UI_Test_BuildTree()
{
	page_boolean.AddItem(&labelBool);
	page_boolean.AddItem(&boolInd1);
	page_boolean.AddItem(&boolCtrl1);
	page_boolean.InitItems();
	page_enum.AddItem(&labelEnum);
	page_enum.AddItem(&enumInd1);
	page_enum.AddItem(&enumCtrl1);
	page_enum.AddItem(&enumCtrl1Icon);
	page_enum.InitItems();	
	list1.AddItem(&page_boolean);
	list1.AddItem(&page_enum);
	
	page_numeric.AddItem(&labelNum);
	page_numeric.AddItem(&numInd1);
	page_numeric.AddItem(&numCtrl1);
	page_numeric.AddItem(&numInd2);
	page_numeric.AddItem(&progress1);
	page_numeric.InitItems();
	
	page_dialogs.AddItem(&buttonReset);
	page_dialogs.AddItem(&buttonShowTestError);
	page_dialogs.InitItems();
	
	tabControl.AddTab("Tab1", &list1);
	tabControl.AddTab("Tab2", &page_numeric);
	tabControl.AddTab("Tab3", &page_dialogs);
	tabControl.SelectTab(0);
	
	mainPage.AddItem(&tabControl);
	mainPage.AddItem(&labelUILib);
	mainPage.AddItem(&speedIcon);
	mainPage.InitItems();
	
	ui_Manager.ChangeVisualTreeRoot(&mainPage);	
}

void UI_Test_Init(Adafruit_GFX* gfx)
{
	ui_Manager.Init(gfx);
}

void UI_Test_Draw(Adafruit_GFX* gfx, bool isFirstPage)
{
	ui_Manager.Draw(gfx, isFirstPage);
}

void UI_Test_KeyInput(Keys_t key)
{
	ui_Manager.KeyInput(key);
}