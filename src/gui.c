#include "include/gui.h"
#include "include/colors.h"
#include "include/fontvalue.h"
#include "include/raylib.h"
#include <string.h>
#define RAYGUI_IMPLEMENTATION
#include "include/raygui.h"

#include "include/lightbfm.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>



void Layout(Gui *ui);
void StatusBarLayout(Gui * ui);
void ToolBarLayout(Gui * ui);
void NewItemWindow(Gui * ui);

int gridPosX = 0;
int gridPosY = 0;

bool NewItemWindowActive = false;


Gui * NewGUI() {
	Gui * ui = (Gui*)malloc(sizeof(Gui));
	flipset_init(ui->flipped);
	itemlist_init(ui->items);
	ui->winWidth = 640;
	ui->winHeight = 640;
	ui->btnHeight = 50;
	ui->btnWidth = 50;
	ui->value = NewFontValue();
	ui->title = TextFormat("BauriFontMaker");
	ui->enableGrid = true;
	ui->gridGuideWidth = 2;
	return ui;
}

void FreeGui(Gui *ui){
	FreeFontValue(ui->value);
	flipset_clear(ui->flipped);
	itemlist_clear(ui->items);
	free(ui);
}

void UpdateData(Gui * ui) {
	ui->winHeight = GetScreenHeight();
	ui->winWidth = GetScreenWidth();
}
void GuiMain(){
	Gui * ui = NewGUI();
	SetTraceLogLevel(LOG_WARNING);
	
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(ui->winWidth, ui->winHeight, ui->title);
	GuiLoadStyleLightBFM();
	
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		BeginDrawing();
		{
			UpdateData(ui);
			ClearBackground(RAYWHITE);
			Layout(ui);
		}
		EndDrawing();
	}

	CloseWindow();
	FreeGui(ui);
}

#define BWIDTH 50
#define BHEIGHT 50

#define GRID_WIDTH 8
#define GRID_HEIGHT 8

#define MARGIN 20
#define SPACING 0
#define GRID_THICKNESS 5
#define GRID true
#define STATUSBAR_HEIGHT 30

#define TOOLBAR_HEIGHT 50

#define CANVAS_ANCHOR (Vector2){.x = 0, .y = TOOLBAR_HEIGHT}

Vector2 canvasBackground(Gui * ui){

	// '- SPACING' because we dont need the last buttons spacing,
	//
	int W = ((SPACING + BWIDTH) * 8) - SPACING;
	int H = ((SPACING + BHEIGHT) * 8) - SPACING;

	if (GRID) {
		W = ((SPACING + GRID_THICKNESS + BWIDTH) * 8) - SPACING;
		H = ((SPACING + GRID_THICKNESS + BHEIGHT) * 8) - SPACING;
	}

	Rectangle rect = (Rectangle){
		MARGIN + CANVAS_ANCHOR.x,
		MARGIN + CANVAS_ANCHOR.y,
		W,
		H,
	};

	if (GRID) {
		rect.width += GRID_THICKNESS;
		rect.height += GRID_THICKNESS;
	}


	DrawRectangleRec(rect, ColorBlack);
	
	if (GRID) {

		rect.x += GRID_THICKNESS;
		rect.y += GRID_THICKNESS;
	
	}

	return (Vector2){.x = rect.x, .y = rect.y};
}

bool isCanvasBtnClicked(Rectangle rect) {
	
	if (
		IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && 
		CheckCollisionPointRec(GetMousePosition(), rect)
	) {
		return true;
	}

	return false;


}

bool isHoverBtn(Rectangle rect) {
	return CheckCollisionPointRec(GetMousePosition(), rect);
}

void canvasLayout(Gui * ui) {
	Vector2 begin = canvasBackground(ui);

	int BSW = BWIDTH + SPACING;
	int BSH = BHEIGHT + SPACING;

	if (GRID) {
		BSW = BWIDTH + SPACING + GRID_THICKNESS;
		BSH = BHEIGHT + SPACING + GRID_THICKNESS;
	}


	for (int row = 0; row < GRID_HEIGHT; row++) {
		for (int col = 0; col < GRID_WIDTH; col++) {
			int X = begin.x + (col * BSW);
			int Y = begin.y + (row * BSH);
			Rectangle rect = (Rectangle){
				X, 
				Y, 
				BWIDTH, 
				BHEIGHT
			};

			if (isCanvasBtnClicked(rect)) {
				printf("[%d,%d]\n" , col, row);
			}

			if (isHoverBtn(rect)) {
				gridPosX = col;
				gridPosY = row;
			}

			DrawRectangleRec(rect, ColorWhite);

			
		}
	}

}

void ItemSelectorLayout(Gui * ui);

void Layout(Gui *ui) {





	

	GuiPanel((Rectangle){0,0,ui->winWidth, ui->winHeight}, NULL);
	ToolBarLayout(ui);
	//canvasLayout(ui);
	//
	ItemSelectorLayout(ui);
	
	StatusBarLayout(ui);
	StatusBarLayout(ui);

	if (NewItemWindowActive) {

		NewItemWindow(ui);
	}


}

#define IL_MARGIN 10
#define IL_ANCHOR (Vector2){0,0}
#define IL_WIDTH 200

int listviewIndex = 0;
int listActiveItem = 0;

#define NEWITEM_BTN_HEIGHT 40

void newItemButton(Gui * ui) {
	if (GuiButton((Rectangle){
		IL_ANCHOR.x + IL_MARGIN,
		IL_ANCHOR.y + TOOLBAR_HEIGHT + IL_MARGIN,
		IL_WIDTH,
		NEWITEM_BTN_HEIGHT,
	}, "New Item")) {
		NewItemWindowActive = true;
	}
}

void ItemSelectorLayout(Gui * ui) {
	//GuiListViewEx(Rectangle bounds, const char **text, int count, int *scrollIndex, int *active, int *focus)
	//
	newItemButton(ui);
	GuiListView((Rectangle){
		IL_ANCHOR.x + IL_MARGIN,
		IL_ANCHOR.y + TOOLBAR_HEIGHT + IL_MARGIN * 2 + NEWITEM_BTN_HEIGHT,
		IL_WIDTH,
		ui->winHeight - STATUSBAR_HEIGHT - IL_MARGIN * 3 - TOOLBAR_HEIGHT - NEWITEM_BTN_HEIGHT,

	}, "APPLE;ORANGE", &listviewIndex, &listActiveItem);
}

#define TOOL_MARGIN 5
#define TOOLBAR_ACHOR (Vector2){.x = 0, .y = 0}

void ToolBarLayout(Gui * ui){
	GuiPanel((Rectangle){
		0,
		0,
		ui->winWidth,
		TOOLBAR_HEIGHT,
	}, NULL);

	int btnX = TOOLBAR_ACHOR.x + TOOL_MARGIN;
	int btnY = TOOLBAR_ACHOR.y + TOOL_MARGIN;

	int btnW = 80;
	int btnH = TOOLBAR_HEIGHT - (TOOL_MARGIN * 2);

	GuiButton((Rectangle){
		btnX,
		btnY,
		btnW,
		btnH,
	}, GuiIconText(ICON_FILE_NEW, "New"));

	btnX += btnW + TOOL_MARGIN * 2;

	GuiButton((Rectangle){
		btnX,
		btnY,
		btnW,
		btnH,
	}, GuiIconText(ICON_FILE_OPEN, "Open"));

	btnX += btnW + TOOL_MARGIN * 2;

	GuiButton((Rectangle){
		btnX,
		btnY,
		btnW,
		btnH,
	}, GuiIconText(ICON_FILE_SAVE_CLASSIC, "Save"));

	btnX += btnW + TOOL_MARGIN * 2;

	DrawLine(btnX, 0, btnX, TOOLBAR_HEIGHT, GRAY);
	btnW = 50;

	btnX = ui->winWidth - TOOL_MARGIN - btnW;

	GuiButton((Rectangle){
		btnX,
		btnY,
		btnW,
		btnH,
	}, GuiIconText(ICON_HELP, NULL));

	btnX -= TOOL_MARGIN * 2 + btnW;

	GuiButton((Rectangle){
		btnX,
		btnY,
		btnW,
		btnH,
	}, GuiIconText(ICON_GEAR, NULL));

	btnX -= TOOL_MARGIN * 2;

	DrawLine(btnX, 0, btnX, TOOLBAR_HEIGHT, GRAY);

	

}

void StatusBarLayout(Gui * ui){
	GuiStatusBar((Rectangle){
		0, 
		ui->winHeight - STATUSBAR_HEIGHT, 
		ui->winWidth, 
		STATUSBAR_HEIGHT
	}, TextFormat("[%d, %d] | " , gridPosX, gridPosY));

}

#define NW_MARGIN 30
#define NW_ANCHOR (Vector2){0,0}
#define NW_WIDTH 500
#define NW_HEIGHT 200

// ------------------
//
char hexCode[120] = "0x0";

void NewItemWindow(Gui * ui) {

	Vector2 winPos = (Vector2){
		(ui->winWidth - NW_WIDTH) * 0.5f,
		(ui->winHeight - NW_HEIGHT) * 0.5f,
	};

	NewItemWindowActive = !GuiWindowBox((Rectangle){
		winPos.x,
		winPos.y,
		NW_WIDTH,
		NW_HEIGHT,

	}, "Create New Font Item");
	GuiLabel((Rectangle){
		winPos.x + NW_ANCHOR.x + NW_MARGIN,
		winPos.y + NW_ANCHOR.y + NW_MARGIN,
		200,
		32
	}, "Font Item Hex : ");

	GuiTextBox((Rectangle){
		winPos.x + NW_ANCHOR.x + NW_MARGIN + 200,
		winPos.y + NW_ANCHOR.y + NW_MARGIN,
		200,
		32
	}, hexCode, 120, true);




}
