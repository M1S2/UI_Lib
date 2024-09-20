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
bool boolVal1 = false;
bool boolVal2 = true;
TestEnum enumVal1;
float numVal1 = 223.456;
int numVal2 = 123;

void OnBoolVal1Changed(void* context);
void OnBoolVal2Changed(void* context);
void OnNumVal1Changed(void* context);
void OnButtonReset(void* context);
void OnMsgOk(void* context);
void OnShowError(void* context);

#define DISPLAY_WIDTH	320
#define DISPLAY_HEIGHT	240

#define COLOR_WHITE		RGB565(0xFF, 0xFF, 0xFF)
#define COLOR_ORANGE	RGB565(0xFF, 0x88, 0x00)

#define TAB_WIDTH	70
#define X_COLUMN1	(TAB_WIDTH + 5)
#define X_COLUMN2	(X_COLUMN1 + 100)
#define Y_ROW1		5
#define Y_ROW2		30
#define Y_ROW3		55
#define Y_ROW4		80
#define Y_ROW5		105

Label<10> labelBool(X_COLUMN1, Y_ROW1, "Boolean", UI_LIB_DEFAULT_FONT, COLOR_WHITE);
BoolIndicator boolInd1(X_COLUMN1, Y_ROW2, &boolVal1);
BoolControl boolCtrl1(X_COLUMN1, Y_ROW3, &boolVal1, &boolVal1, &OnBoolVal1Changed);
BoolControl boolCtrl2(X_COLUMN1, Y_ROW5, &boolVal2, &boolVal2, &OnBoolVal2Changed);
ContainerPage page_boolean;
Label<15> labelEnum(X_COLUMN1, Y_ROW1, "Enumerations", UI_LIB_DEFAULT_FONT, COLOR_WHITE);
EnumIndicator<TestEnum> enumInd1(X_COLUMN1 + 16, Y_ROW2, &enumVal1, TestEnumNames, 3);
EnumControl<TestEnum> enumCtrl1(X_COLUMN1 + 16, Y_ROW3, &enumVal1, TestEnumNames, 3);
Icon enumCtrl1Icon(X_COLUMN1, Y_ROW3, ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
ContainerPage page_enum;
ContainerList list1;

Label<10> labelNum(X_COLUMN1, Y_ROW1, "Numerics", UI_LIB_DEFAULT_FONT, COLOR_WHITE);
NumericIndicator<int> numInd2(X_COLUMN1, Y_ROW2, &numVal2, "A", 5000, 0);
NumericIndicator<float> numInd1(X_COLUMN2, Y_ROW2, &numVal1, "V", 2000, 3);
NumericControl<float> numCtrl1(X_COLUMN2, Y_ROW3, &numVal1, "V", -10, 2000, 3, &numVal1, &OnNumVal1Changed);
ProgressBar<float> progress1(X_COLUMN2, Y_ROW4, 70, 20, &numVal1, -10, 2000, PROGRESSBAR_ORIGIN_ZERO, 0);
ContainerPage page_numeric;

Label<10> labelButtons(X_COLUMN1, Y_ROW1, "Buttons", UI_LIB_DEFAULT_FONT, COLOR_WHITE);
ButtonControlDefault buttonReset(X_COLUMN1, Y_ROW2, DEFAULT_UI_ELEMENT_WIDTH, DEFAULT_UI_ELEMENT_HEIGHT, "Reset", NULL, &OnButtonReset);
ButtonControlDefault buttonShowTestError(X_COLUMN1, Y_ROW4, 125, DEFAULT_UI_ELEMENT_HEIGHT, "Show Error", NULL, &OnShowError);
MessageDialogDefault msgReset(5, Y_ROW1, DISPLAY_WIDTH - 5, DISPLAY_HEIGHT - Y_ROW1, "Reset sucessful.", MSG_INFO, MSG_BTN_OK, NULL, &OnMsgOk);
MessageDialogDefault msgTestWarning(5, Y_ROW1, DISPLAY_WIDTH - 5, DISPLAY_HEIGHT - Y_ROW1, "Warning message.\nWith Newline.", MSG_WARNING, MSG_BTN_OK, NULL, &OnMsgOk);
MessageDialogDefault msgTestError(5, Y_ROW1, DISPLAY_WIDTH - 5, DISPLAY_HEIGHT - Y_ROW1, "Error message.", MSG_ERROR, MSG_BTN_OK, NULL, &OnMsgOk);
ContainerPage page_dialogs;

Icon icon1(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
Icon icon2(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
Icon icon3(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
Icon icon4(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
Icon icon5(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
Icon icon6(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
Icon icon7(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
Icon icon8(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
Icon icon9(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
ContainerGrid grid_icons;

TabControl tabControl(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, TAB_WIDTH);

ContainerPage mainPage;
LabelDefault labelUILib(X_COLUMN1, DISPLAY_HEIGHT - settings_window_height - 10, "UI LIB", &FreeMono18pt7b, COLOR_ORANGE);
Icon globalIcon(DISPLAY_WIDTH - settings_window_width - 15, DISPLAY_HEIGHT - settings_window_height - 10, settings_window_width, settings_window_height, settings_window_bits);

void OnBoolVal1Changed(void* context)
{
	ui_Manager.ChangeVisualTreeRoot(&msgTestWarning);
}

void OnBoolVal2Changed(void* context)
{
	bool boolVal = *((bool*)context);
	globalIcon.Visible = boolVal;
}

void OnNumVal1Changed(void* context)
{
	numVal2++;	
}

void OnButtonReset(void* context)
{
	boolVal1 = false;
	boolVal2 = true;
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
	mainPage.AddItem(&tabControl);
	mainPage.AddItem(&labelUILib);
	mainPage.AddItem(&globalIcon);
	
	tabControl.AddTab("Tab1", &list1);
	tabControl.AddTab("Tab2", &page_numeric);
	tabControl.AddTab("Tab3", &page_dialogs);
	tabControl.AddTab("Tab4", &grid_icons);
	tabControl.SelectTab(0);

	list1.AddItem(&page_boolean);
	list1.AddItem(&page_enum);

	page_boolean.AddItem(&labelBool);
	page_boolean.AddItem(&boolInd1);
	page_boolean.AddItem(&boolCtrl1);
	page_boolean.AddItem(&boolCtrl2);
	page_boolean.InitItems();
	page_enum.AddItem(&labelEnum);
	page_enum.AddItem(&enumInd1);
	page_enum.AddItem(&enumCtrl1);
	page_enum.AddItem(&enumCtrl1Icon);
	page_enum.InitItems();	
	
	page_numeric.AddItem(&labelNum);
	page_numeric.AddItem(&numInd1);
	page_numeric.AddItem(&numCtrl1);
	page_numeric.AddItem(&numInd2);
	page_numeric.AddItem(&progress1);
	page_numeric.InitItems();
	
	page_dialogs.AddItem(&labelButtons);
	page_dialogs.AddItem(&buttonReset);
	page_dialogs.AddItem(&buttonShowTestError);
	page_dialogs.InitItems();
	
	grid_icons.SetRowHeight(0, 30);
	grid_icons.SetRowHeight(1, 30);
	grid_icons.SetRowHeight(2, 40);
	grid_icons.SetColumnWidth(0, 80);
	grid_icons.SetColumnWidth(1, 60);
	grid_icons.SetColumnWidth(2, 40);
	grid_icons.AddItemToCell(&icon1, 0, 0, GRID_CELL_ALIGNMENT_TOP_LEFT);
	grid_icons.AddItemToCell(&icon2, 0, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_icons.AddItemToCell(&icon3, 0, 2, GRID_CELL_ALIGNMENT_BOTTOM_LEFT);
	grid_icons.AddItemToCell(&icon4, 1, 0, GRID_CELL_ALIGNMENT_TOP);
	grid_icons.AddItemToCell(&icon5, 1, 1, GRID_CELL_ALIGNMENT_MIDDLE);
	grid_icons.AddItemToCell(&icon6, 1, 2, GRID_CELL_ALIGNMENT_BOTTOM);
	grid_icons.AddItemToCell(&icon7, 2, 0, GRID_CELL_ALIGNMENT_TOP_RIGHT);
	grid_icons.AddItemToCell(&icon8, 2, 1, GRID_CELL_ALIGNMENT_RIGHT);
	grid_icons.AddItemToCell(&icon9, 2, 2, GRID_CELL_ALIGNMENT_BOTTOM_RIGHT);
	grid_icons.InitItems();
	
	mainPage.InitItems();
	
	ui_Manager.ChangeVisualTreeRoot(&mainPage);	
}

void UI_Test_Init(Adafruit_GFX* gfx)
{
	ui_Manager.Init(gfx);
}

void UI_Test_Draw(Adafruit_GFX* gfx)
{
	ui_Manager.Draw(gfx);
}

void UI_Test_KeyInput(Keys_t key)
{
	ui_Manager.KeyInput(key);
}