/*
 * UI_Lib_Test.cpp
 *
 */ 

#include "UI_Lib_Test.h"
#include <Fonts/FreeMono18pt7b.h>

#include <Fonts/FreeSerifItalic12pt7b.h>

enum TestEnum
{
	Test_A,
	Test_B,
	Test_C
};
const char* TestEnumNames[] = { "Test A", "Test B", "Test C" };

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
#define X_COLUMN2	(X_COLUMN1 + 30)
#define Y_ROW1		5
#define Y_ROW2		30
#define Y_ROW3		55
#define Y_ROW4		85
#define Y_ROW5		105

#define ELEMENT_MARGIN	10

Label<10> labelBool("Boolean", COLOR_WHITE);
BoolIndicator boolInd1(&boolVal1);
BoolControl boolCtrl1(&boolVal1, &boolVal1, &OnBoolVal1Changed);
BoolControl boolCtrl2(&boolVal2, &boolVal2, &OnBoolVal2Changed);
ContainerStackDefault stack_boolean(STACK_LAYOUT_VERTICAL_LEFT, ELEMENT_MARGIN);
Label<15> labelEnum("Enumerations", COLOR_WHITE);
EnumIndicator<TestEnum> enumInd1(&enumVal1, TestEnumNames, 3);
EnumControl<TestEnum> enumCtrl1(&enumVal1, TestEnumNames, 3);
Icon enumCtrl1Icon(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
ContainerStackDefault stack_enumCtrl1(STACK_LAYOUT_HORIZONTAL_TOP, ELEMENT_MARGIN);
EnumControl<TestEnum> enumCtrl2(&enumVal1, TestEnumNames, 3);
Icon enumCtrl2Icon(icon_info_width, icon_info_height, icon_info_bits);
ContainerStackDefault stack_enumCtrl2(STACK_LAYOUT_HORIZONTAL_CENTER, ELEMENT_MARGIN);
ContainerStackDefault stack_enum(STACK_LAYOUT_VERTICAL_CENTER, ELEMENT_MARGIN);
ContainerListDefault list1;

Label<10> labelNum(ELEMENT_MARGIN, Y_ROW1, "Numerics", COLOR_WHITE);
NumericIndicator<int> numInd2(ELEMENT_MARGIN, Y_ROW2, &numVal2, "A", 5000, 0);
NumericIndicator<float> numInd1(X_COLUMN2, Y_ROW2, &numVal1, "V", 2000, 3);
NumericControl<float> numCtrl1(X_COLUMN2, Y_ROW3, &numVal1, "V", -10, 2000, 3, &numVal1, &OnNumVal1Changed);
ProgressBar<float> progress1(X_COLUMN2, Y_ROW4, 70, 20, &numVal1, -10, 2000, PROGRESSBAR_ORIGIN_ZERO, 0);
ContainerPageDefault page_numeric;

Label<10> labelButtons(ELEMENT_MARGIN, Y_ROW1, "Buttons", COLOR_WHITE);
ButtonControlDefault buttonReset(ELEMENT_MARGIN, Y_ROW2, 0, 0, "Reset", NULL, &OnButtonReset);
ButtonControlDefault buttonShowTestError(ELEMENT_MARGIN, Y_ROW4, 0, 0, "Show Error", NULL, &OnShowError);
MessageDialogDefault msgReset(ELEMENT_MARGIN, ELEMENT_MARGIN, DISPLAY_WIDTH - 2 * ELEMENT_MARGIN, DISPLAY_HEIGHT - 2 * ELEMENT_MARGIN, "Reset sucessful.", MSG_INFO, MSG_BTN_OK, NULL, &OnMsgOk);
MessageDialogDefault msgTestWarning(ELEMENT_MARGIN, ELEMENT_MARGIN, DISPLAY_WIDTH - 2 * ELEMENT_MARGIN, DISPLAY_HEIGHT - 2 * ELEMENT_MARGIN, "Warning message.\nWith Newline.", MSG_WARNING, MSG_BTN_OK, NULL, &OnMsgOk);
MessageDialogDefault msgTestError(ELEMENT_MARGIN, ELEMENT_MARGIN, DISPLAY_WIDTH - 2 * ELEMENT_MARGIN, DISPLAY_HEIGHT - 2 * ELEMENT_MARGIN, "Error message.", MSG_ERROR, MSG_BTN_OK, NULL, &OnMsgOk);
ContainerPageDefault page_dialogs;

Label<10> labelGrid("Grid", COLOR_WHITE);
Icon icon1(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
Icon icon2(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
Icon icon3(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
Icon icon4(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
Icon icon5(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
Icon icon6(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
Icon icon7(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
Icon icon8(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
Icon icon9(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
ContainerGridDefault grid_icons;

TabControlDefault tabControl(DISPLAY_WIDTH, DISPLAY_HEIGHT, TAB_WIDTH);

ContainerPageDefault mainPage(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
LabelDefault labelUILib(X_COLUMN1, DISPLAY_HEIGHT - settings_window_height - 10, "UI LIB", &FreeMono18pt7b, COLOR_ORANGE);
Icon globalIcon(DISPLAY_WIDTH - settings_window_width - 25, DISPLAY_HEIGHT - settings_window_height - 10, settings_window_width, settings_window_height, settings_window_bits);

void OnBoolVal1Changed(void* context)
{
	UiManager.ChangeVisualTreeRoot(&msgTestWarning);
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
	UiManager.ChangeVisualTreeRoot(&msgReset);
}

void OnShowError(void* context)
{
	UiManager.ChangeVisualTreeRoot(&msgTestError);
}

void OnMsgOk(void* context)
{
	UiManager.ChangeVisualTreeRoot(&mainPage);	
}

void UI_Test_BuildTree()
{
	mainPage.AddItem(&tabControl);
	mainPage.AddItem(&labelUILib);
	mainPage.AddItem(&globalIcon);
	
	tabControl.AddItem("Tab1", &list1);
	tabControl.AddItem("Tab2", &page_numeric);
	tabControl.AddItem("Tab3", &page_dialogs);
	tabControl.AddItem("Tab4", &grid_icons);
	tabControl.SelectTab(0);

	list1.AddItem(&stack_boolean);
	list1.AddItem(&stack_enum);

	stack_boolean.AddItem(&labelBool);
	stack_boolean.AddItem(&boolInd1);
	stack_boolean.AddItem(&boolCtrl1);
	stack_boolean.AddItem(&boolCtrl2);
	stack_boolean.InitItems();

	stack_enumCtrl1.AddItem(&enumCtrl1Icon);
	stack_enumCtrl1.AddItem(&enumCtrl1);
	stack_enumCtrl1.InitItems();	
	stack_enumCtrl2.AddItem(&enumCtrl2Icon);
	stack_enumCtrl2.AddItem(&enumCtrl2);
	stack_enumCtrl2.InitItems();
	stack_enum.AddItem(&labelEnum);
	stack_enum.AddItem(&enumInd1);
	stack_enum.AddItem(&stack_enumCtrl1);
	stack_enum.AddItem(&stack_enumCtrl2);
	stack_enum.InitItems();
	
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
	grid_icons.SetRowHeight(2, 30);
	grid_icons.SetRowHeight(3, 40);
	grid_icons.SetColumnWidth(0, 80);
	grid_icons.SetColumnWidth(1, 60);
	grid_icons.SetColumnWidth(2, 40);
	grid_icons.AddItem(&labelGrid, 0, 0, GRID_CELL_ALIGNMENT_TOP_LEFT);
	grid_icons.AddItem(&icon1, 0, 1, GRID_CELL_ALIGNMENT_TOP_LEFT);
	grid_icons.AddItem(&icon2, 0, 2, GRID_CELL_ALIGNMENT_LEFT);
	grid_icons.AddItem(&icon3, 0, 3, GRID_CELL_ALIGNMENT_BOTTOM_LEFT);
	grid_icons.AddItem(&icon4, 1, 1, GRID_CELL_ALIGNMENT_TOP);
	grid_icons.AddItem(&icon5, 1, 2, GRID_CELL_ALIGNMENT_MIDDLE);
	grid_icons.AddItem(&icon6, 1, 3, GRID_CELL_ALIGNMENT_BOTTOM);
	grid_icons.AddItem(&icon7, 2, 1, GRID_CELL_ALIGNMENT_TOP_RIGHT);
	grid_icons.AddItem(&icon8, 2, 2, GRID_CELL_ALIGNMENT_RIGHT);
	grid_icons.AddItem(&icon9, 2, 3, GRID_CELL_ALIGNMENT_BOTTOM_RIGHT);
	grid_icons.InitItems();
	
	mainPage.InitItems();
	
	UiManager.ChangeVisualTreeRoot(&mainPage);	
}

void UI_Test_Init(Adafruit_GFX* gfx)
{
	// Use the global UiManager object and don't create a new instace !!!
	UiManager.SetColors(RGB565(0x00, 0x00, 0x00), RGB565(0x00, 0xF7, 0x00), RGB565(0x00, 0x00, 0x00));
	UiManager.Init(gfx);
	//UiManager.SetFont(&FreeSerifItalic12pt7b);
	//UiManager.SetFont(&FreeMono18pt7b);
}

void UI_Test_Draw()
{
	UiManager.Draw();
}

void UI_Test_KeyInput(Keys_t key)
{
	UiManager.KeyInput(key);
}