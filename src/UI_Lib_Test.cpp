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

#define X_COLUMN1	ELEMENT_MARGIN
#define X_COLUMN2	125
#define Y_ROW1		5
#define Y_ROW2		35
#define Y_ROW3		65
#define Y_ROW4		100
#define Y_ROW5		125

#define ELEMENT_MARGIN	10

Label labelBool("Boolean", COLOR_WHITE, NULL, 0, 0, 10);
BoolIndicator boolInd1(&boolVal1);
BoolControl boolCtrl1(&boolVal1, &boolVal1, &OnBoolVal1Changed);
BoolControl boolCtrl2(&boolVal2, &boolVal2, &OnBoolVal2Changed);
Icon boolIcon(settings_window_width, settings_window_height, settings_window_bits, COLOR_ORANGE, 100, 30);
ContainerStack stack_boolean(STACK_LAYOUT_VERTICAL_LEFT);
ContainerPage page_boolean;
Label labelEnum("Enumerations", COLOR_WHITE, NULL, 0, 0, 15);
EnumIndicator<TestEnum> enumInd1(&enumVal1, TestEnumNames, 3);
EnumControl<TestEnum> enumCtrl1(&enumVal1, TestEnumNames, 3);
Icon enumCtrl1Icon(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
ContainerStack stack_enumCtrl1(STACK_LAYOUT_HORIZONTAL_TOP);
EnumControl<TestEnum> enumCtrl2(&enumVal1, TestEnumNames, 3);
Icon enumCtrl2Icon(icon_info_width, icon_info_height, icon_info_bits);
ContainerStack stack_enumCtrl2(STACK_LAYOUT_HORIZONTAL_CENTER);
ContainerStack stack_enum(STACK_LAYOUT_VERTICAL_CENTER);
ContainerList list1;

Label labelNum("Numerics", COLOR_WHITE, NULL, X_COLUMN1, Y_ROW1, 10);
NumericIndicator<int> numInd2(X_COLUMN1, Y_ROW2, &numVal2, "x", 5000, 0);
Label lbl_numInd2_Text("Text...", LABEL_COLOR_NOTSET, NULL, X_COLUMN1, Y_ROW3);
NumericIndicator<float> numInd1(X_COLUMN2, Y_ROW2, &numVal1, "V", 2000, 2);
NumericControl<float> numCtrl1(X_COLUMN2, Y_ROW3, &numVal1, "V", -500, 2000, 3, &numVal1, &OnNumVal1Changed);
ProgressBar<float> progress1(X_COLUMN2, Y_ROW4, &numVal1, -500, 2000, PROGRESSBAR_ORIGIN_ZERO, 250, 70, 20);
ContainerPage page_numeric;

Label labelButtons("Buttons", COLOR_WHITE, NULL, ELEMENT_MARGIN, Y_ROW1, 10);
ButtonControl buttonReset(ELEMENT_MARGIN, Y_ROW2, 0, 0, "Reset", NULL, &OnButtonReset);
ButtonControl buttonShowTestError(ELEMENT_MARGIN, Y_ROW4, 0, 0, "Show Error", NULL, &OnShowError);
MessageDialog msgReset(ELEMENT_MARGIN, ELEMENT_MARGIN, DISPLAY_WIDTH - 2 * ELEMENT_MARGIN, DISPLAY_HEIGHT - 2 * ELEMENT_MARGIN, "Reset sucessful.", MSG_INFO, MSG_BTN_OK, NULL, &OnMsgOk);
MessageDialog msgTestWarning(ELEMENT_MARGIN, ELEMENT_MARGIN, DISPLAY_WIDTH - 2 * ELEMENT_MARGIN, DISPLAY_HEIGHT - 2 * ELEMENT_MARGIN, "Warning message.\nWith Newline.", MSG_WARNING, MSG_BTN_OK, NULL, &OnMsgOk);
MessageDialog msgTestError(ELEMENT_MARGIN, ELEMENT_MARGIN, DISPLAY_WIDTH - 2 * ELEMENT_MARGIN, DISPLAY_HEIGHT - 2 * ELEMENT_MARGIN, "Error message.", MSG_ERROR, MSG_BTN_OK, NULL, &OnMsgOk);
ContainerPage page_dialogs;

Label labelGrid("Grid", COLOR_WHITE);
Label labelGridSide("Grid Side");
Icon icon1(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
Icon icon2(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
Icon icon3(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
Icon icon4(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
Icon icon5(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
Icon icon6(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
Icon icon7(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
Icon icon8(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
Icon icon9(ui_icon_speed_width, ui_icon_speed_height, ui_icon_speed_bits);
ContainerGrid grid_icons(11, 5, 5, true, true);

Label labelTab1("#1", COLOR_WHITE);
Icon iconTab1(icon_info_width, icon_info_height, icon_info_bits);
ContainerStack stack_Tab1Header(STACK_LAYOUT_HORIZONTAL_CENTER);
Label labelTab2("#2", COLOR_WHITE);
Icon iconTab2(icon_info_width, icon_info_height, icon_info_bits);
ContainerStack stack_Tab2Header(STACK_LAYOUT_HORIZONTAL_CENTER);
Label labelTab3("#3", COLOR_WHITE);
Icon iconTab3(icon_info_width, icon_info_height, icon_info_bits);
ContainerStack stack_Tab3Header(STACK_LAYOUT_HORIZONTAL_CENTER);
Label labelTab4("#4", COLOR_WHITE);
Icon iconTab4(icon_info_width, icon_info_height, icon_info_bits);
ContainerStack stack_Tab4Header(STACK_LAYOUT_HORIZONTAL_CENTER);
TabControl tabControl(0, 35, DISPLAY_WIDTH, DISPLAY_HEIGHT - 35, TAB_POSITION_TOP);

ContainerPage mainPage(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
Label labelUILib("UI LIB", COLOR_ORANGE, &FreeMono18pt7b);

void OnBoolVal1Changed(void* context)
{
	UiManager.ChangeVisualTreeRoot(&msgTestWarning);
}

void OnBoolVal2Changed(void* context)
{
	bool boolVal = *((bool*)context);
	boolIcon.Visible = boolVal;
}

void OnNumVal1Changed(void* context)
{
	numVal2++;

	char buffer[20];
	itoa(numVal2, buffer, 10);
	lbl_numInd2_Text.SetText(buffer);
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

UIElement* build_screen_boolean_enum()
{
	stack_boolean.AddItem(&labelBool);
	stack_boolean.AddItem(&boolInd1);
	stack_boolean.AddItem(&boolCtrl1);
	stack_boolean.AddItem(&boolCtrl2);
	stack_boolean.InitItems();
	page_boolean.AddItem(&stack_boolean);
	page_boolean.AddItem(&boolIcon);
	page_boolean.InitItems();

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

	list1.AddItem(&page_boolean);
	list1.AddItem(&stack_enum);
	return &list1;
}

UIElement* build_screen_numeric()
{
	page_numeric.AddItem(&labelNum);
	page_numeric.AddItem(&numInd1);
	page_numeric.AddItem(&numCtrl1);
	page_numeric.AddItem(&numInd2);
	page_numeric.AddItem(&progress1);
	page_numeric.AddItem(&lbl_numInd2_Text);
	page_numeric.InitItems();
	return &page_numeric;
}

UIElement* build_screen_dialogs()
{
	page_dialogs.AddItem(&labelButtons);
	page_dialogs.AddItem(&buttonReset);
	page_dialogs.AddItem(&buttonShowTestError);
	page_dialogs.InitItems();
	return &page_dialogs;
}

UIElement* build_screen_grid()
{
	grid_icons.SetRowHeight(0, 35);	
	grid_icons.SetRowHeight(1, 35);
	grid_icons.SetRowHeight(2, 35);
	grid_icons.SetRowHeight(3, 45);
	//grid_icons.SetColumnWidth(0, 90);		// This line is commented out intentionally to show the auto-sizing of column 0. It is sized to the largest element assigned to this column.
	grid_icons.SetColumnWidth(1, 60);
	grid_icons.SetColumnWidth(2, 50);
	grid_icons.AddItem(&labelGrid, 0, 0, GRID_CELL_ALIGNMENT_TOP, 2, 1);
	grid_icons.AddItem(&icon1, 0, 1, GRID_CELL_ALIGNMENT_TOP_LEFT);
	grid_icons.AddItem(&icon2, 0, 2, GRID_CELL_ALIGNMENT_LEFT);
	grid_icons.AddItem(&icon3, 0, 3, GRID_CELL_ALIGNMENT_BOTTOM_LEFT);
	grid_icons.AddItem(&icon4, 1, 1, GRID_CELL_ALIGNMENT_TOP);
	grid_icons.AddItem(&icon5, 1, 2, GRID_CELL_ALIGNMENT_MIDDLE);
	grid_icons.AddItem(&icon6, 1, 3, GRID_CELL_ALIGNMENT_BOTTOM);
	grid_icons.AddItem(&icon7, 2, 1, GRID_CELL_ALIGNMENT_TOP_RIGHT);
	grid_icons.AddItem(&icon8, 2, 2, GRID_CELL_ALIGNMENT_RIGHT);
	grid_icons.AddItem(&icon9, 2, 3, GRID_CELL_ALIGNMENT_BOTTOM_RIGHT);
	grid_icons.AddItem(&labelGridSide, 3, 0, GRID_CELL_ALIGNMENT_MIDDLE, 1, 4);
	grid_icons.InitItems();
	return &grid_icons;
}

void UI_Test_BuildTree()
{
	mainPage.AddItem(&tabControl);
	mainPage.AddItem(&labelUILib);
	
	stack_Tab1Header.AddItem(&iconTab1);
	stack_Tab1Header.AddItem(&labelTab1);
	stack_Tab2Header.AddItem(&iconTab2);
	stack_Tab2Header.AddItem(&labelTab2);
	stack_Tab3Header.AddItem(&iconTab3);
	stack_Tab3Header.AddItem(&labelTab3);
	stack_Tab4Header.AddItem(&iconTab4);
	stack_Tab4Header.AddItem(&labelTab4);
	tabControl.AddItem(&stack_Tab1Header, build_screen_boolean_enum());
	tabControl.AddItem(&stack_Tab2Header, build_screen_numeric());
	tabControl.AddItem(&stack_Tab3Header, build_screen_dialogs());
	tabControl.AddItem(&stack_Tab4Header, build_screen_grid());
	tabControl.SelectTab(0);
	
	UiManager.ChangeVisualTreeRoot(&mainPage);	
}

void UI_Test_Init(Adafruit_GFX* gfx)
{
	// Use the global UiManager object to access the singleton class.
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