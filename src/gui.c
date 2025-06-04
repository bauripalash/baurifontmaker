#include "include/gui.h"
#include "include/colors.h"
#include "include/ext/raylib.h"
#include "include/fontvalue.h"
#include "include/strlist.h"
#include <string.h>
#define RAYGUI_IMPLEMENTATION
#include "include/ext/raygui.h"

#include "include/ext/lightbfm.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define NW_MARGIN 30
#define NW_WIDTH 500
#define NW_HEIGHT 200

#define MARGIN 20 // Canvas Margin

#define IL_MARGIN 10
#define NEWITEM_BTN_HEIGHT 40
#define ITEM_CTRL_BTN_PADDING 5

#define ITEM_PANEL_PADDING 10
#define ITEM_PANEL_MARGIN 20

#define TOOL_MARGIN 5

void Layout(Gui *ui);
void StatusBarLayout(Gui *ui);
void ToolBarLayout(Gui *ui);
void NewItemWindow(Gui *ui);
void ItemSelectorLayout(Gui *ui);

int gridPosX = 0;
int gridPosY = 0;

bool NewItemWindowActive = false;

char hexCode[120] = "0x0";

Gui *NewGUI() {
    Gui *ui = (Gui *)malloc(sizeof(Gui));
    flipset_init(ui->flipped);
    itemlist_init(ui->items);
    ui->winWidth = 640;
    ui->winHeight = 640;
    ui->btnSize = 50;
    ui->value = NewFontValue();
    ui->title = TextFormat("BauriFontMaker");
    ui->enableGrid = true;
    ui->gridThickness = 2;
    ui->toolbarHeight = 50;
    ui->statusbarHeight = 30;
    ui->canvasAnchor = (Vector2){.x = 0, .y = ui->toolbarHeight};
    ui->itemListAnchor = (Vector2){.x = 0, .y = 0};
    ui->itemListWidth = 200;
    ui->itemListActive = 0;
    ui->itemListIndex = 0;
    ui->gridSize = (Vector2){.x = 8, .y = 8};
    ui->listItems = NewStrList();

    AddStrList(ui->listItems, "0x0000 (A)");

    return ui;
}

void FreeGui(Gui *ui) {

    FreeFontValue(ui->value);
    FreeStrList(ui->listItems);
    flipset_clear(ui->flipped);
    itemlist_clear(ui->items);
    free(ui);
}

void UpdateData(Gui *ui) {
    ui->winHeight = GetScreenHeight();
    ui->winWidth = GetScreenWidth();
}

void FreeStyles() {
    Font f = GuiGetFont();
    UnloadFont(f);
}

void GuiMain() {
    Gui *ui = NewGUI();
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

    FreeGui(ui);
    FreeStyles();
    CloseWindow();
}

Vector2 canvasBackground(Gui *ui) {

    int W = (ui->btnSize * 8);
    int H = (ui->btnSize * 8);

    if (ui->enableGrid) {
        W = ((ui->gridThickness + ui->btnSize) * 8);
        H = ((ui->gridThickness + ui->btnSize) * 8);
    }

    Rectangle rect = (Rectangle){
        MARGIN + ui->canvasAnchor.x,
        MARGIN + ui->canvasAnchor.y,
        W,
        H,
    };

    if (ui->enableGrid) {
        rect.width += ui->gridThickness;
        rect.height += ui->gridThickness;
    }

    DrawRectangleRec(rect, ColorBlack);

    if (ui->enableGrid) {

        rect.x += ui->gridThickness;
        rect.y += ui->gridThickness;
    }

    return (Vector2){.x = rect.x, .y = rect.y};
}

bool isCanvasBtnClicked(Rectangle rect) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
        CheckCollisionPointRec(GetMousePosition(), rect)) {
        return true;
    }

    return false;
}

bool isHoverBtn(Rectangle rect) {
    return CheckCollisionPointRec(GetMousePosition(), rect);
}

void canvasLayout(Gui *ui) {
    Vector2 begin = canvasBackground(ui);

    int buttonSize = ui->btnSize;

    if (ui->enableGrid) {
        buttonSize = ui->btnSize + ui->gridThickness;
    }

    for (int row = 0; row < (int)ui->gridSize.y; row++) {
        for (int col = 0; col < (int)ui->gridSize.x; col++) {
            int X = begin.x + (col * buttonSize);
            int Y = begin.y + (row * buttonSize);
            Rectangle rect = (Rectangle){
                X,
                Y,
                ui->btnSize,
                ui->btnSize,
            };

            if (isCanvasBtnClicked(rect)) {
                printf("[%d,%d]\n", col, row);
            }

            if (isHoverBtn(rect)) {
                gridPosX = col;
                gridPosY = row;
            }

            DrawRectangleRec(rect, ColorWhite);
        }
    }
}

void Layout(Gui *ui) {
    GuiPanel((Rectangle){0, 0, ui->winWidth, ui->winHeight}, NULL);
    ToolBarLayout(ui);
    ui->canvasAnchor.x = 300;
    ui->canvasAnchor.y = ui->toolbarHeight + 10;
    canvasLayout(ui);
    ItemSelectorLayout(ui);
    StatusBarLayout(ui);

    if (NewItemWindowActive) {

        NewItemWindow(ui);
    }
}

void ItemCtrlBtns(Gui *ui, float x, float y, float maxWidth) {
    float widthForNewBtn = maxWidth * (2.0 / 3.0) - 5;
    if (GuiButton(
            (Rectangle){
                x,
                y,
                widthForNewBtn,
                NEWITEM_BTN_HEIGHT,

            },
            GuiIconText(ICON_KEY, "new"))) {
        NewItemWindowActive = true;
    }

    GuiButton(
        (Rectangle){
            x + widthForNewBtn + ITEM_CTRL_BTN_PADDING,
            y,
            maxWidth - widthForNewBtn - ITEM_CTRL_BTN_PADDING,
            NEWITEM_BTN_HEIGHT,
        },
        GuiIconText(ICON_PENCIL_BIG, NULL));
}
int listItemFocus = 0;

void ItemSelectorLayout(Gui *ui) {
    Rectangle ItemPanelRect = (Rectangle){
        ui->itemListAnchor.x + ITEM_PANEL_MARGIN,
        ui->itemListAnchor.y + ui->toolbarHeight + ITEM_PANEL_MARGIN,
        ui->itemListWidth,
        ui->winHeight - ui->statusbarHeight - ui->toolbarHeight -
            (ITEM_PANEL_MARGIN * 2)};
    GuiGroupBox((ItemPanelRect), "~items");
    ItemCtrlBtns(ui, ItemPanelRect.x + ITEM_PANEL_PADDING,
                 ItemPanelRect.y + ITEM_PANEL_PADDING,
                 ui->itemListWidth - ITEM_PANEL_PADDING * 2);

    float btnsY = ItemPanelRect.y + ITEM_PANEL_PADDING * 2 + NEWITEM_BTN_HEIGHT;

    GuiListViewEx(
        (Rectangle){
            ItemPanelRect.x + ITEM_PANEL_PADDING,
            btnsY,
            ui->itemListWidth - (ITEM_PANEL_PADDING * 2),
            ItemPanelRect.height - NEWITEM_BTN_HEIGHT - ITEM_PANEL_PADDING * 3,
        },
        ui->listItems->items, ui->listItems->len, &ui->itemListIndex,
        &ui->itemListActive, &listItemFocus);
}

void ToolBarLayout(Gui *ui) {
    GuiPanel(
        (Rectangle){
            0,
            0,
            ui->winWidth,
            ui->toolbarHeight,
        },
        NULL);

    int btnX = TOOL_MARGIN;
    int btnY = TOOL_MARGIN;

    int btnW = 80;
    int btnH = ui->toolbarHeight - (TOOL_MARGIN * 2);

    GuiButton(
        (Rectangle){
            btnX,
            btnY,
            btnW,
            btnH,
        },
        GuiIconText(ICON_FILE_NEW, "New"));

    btnX += btnW + TOOL_MARGIN * 2;

    GuiButton(
        (Rectangle){
            btnX,
            btnY,
            btnW,
            btnH,
        },
        GuiIconText(ICON_FILE_OPEN, "Open"));

    btnX += btnW + TOOL_MARGIN * 2;

    GuiButton(
        (Rectangle){
            btnX,
            btnY,
            btnW,
            btnH,
        },
        GuiIconText(ICON_FILE_SAVE_CLASSIC, "Save"));

    btnX += btnW + TOOL_MARGIN * 2;

    DrawLine(btnX, 0, btnX, ui->toolbarHeight, GRAY);
    btnW = 50;

    btnX = ui->winWidth - TOOL_MARGIN - btnW;

    GuiButton(
        (Rectangle){
            btnX,
            btnY,
            btnW,
            btnH,
        },
        GuiIconText(ICON_HELP, NULL));

    btnX -= TOOL_MARGIN * 2 + btnW;

    GuiButton(
        (Rectangle){
            btnX,
            btnY,
            btnW,
            btnH,
        },
        GuiIconText(ICON_GEAR, NULL));

    btnX -= TOOL_MARGIN * 2;

    DrawLine(btnX, 0, btnX, ui->toolbarHeight, GRAY);
}

void StatusBarLayout(Gui *ui) {
    GuiStatusBar(
        (Rectangle){
            0,
            ui->winHeight - ui->statusbarHeight,
            ui->winWidth,
            ui->statusbarHeight,
        },
        TextFormat("[%d, %d] | ", gridPosX, gridPosY));
}

void NewItemWindow(Gui *ui) {
    Vector2 winPos = (Vector2){
        (ui->winWidth - NW_WIDTH) * 0.5f,
        (ui->winHeight - NW_HEIGHT) * 0.5f,
    };

    NewItemWindowActive = !GuiWindowBox(
        (Rectangle){
            winPos.x,
            winPos.y,
            NW_WIDTH,
            NW_HEIGHT,

        },
        "Create New Font Item");
    GuiLabel((Rectangle){winPos.x + NW_MARGIN, winPos.y + NW_MARGIN, 200, 32},
             "Font Item Hex : ");

    GuiTextBox(
        (Rectangle){winPos.x + NW_MARGIN + 200, winPos.y + NW_MARGIN, 200, 32},
        hexCode, 120, true);
}
