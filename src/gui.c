#include "include/gui.h"
#include "include/colors.h"
#include "include/ext/raylib.h"
#include "include/fontitem.h"
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

#define CANVAS_PANEL_MARGIN 10

void Layout(Gui *ui);
void StatusBarLayout(Gui *ui);
void ToolBarLayout(Gui *ui);
void NewItemWindow(Gui *ui);
void EditItemWindow(Gui *ui);

void ItemSelectorLayout(Gui *ui);

void canvasPanel(Gui *ui);
void canvasLayout(Gui *ui, Rectangle panel);

int gridPosX = 0;
int gridPosY = 0;

int canvasWidth = 0;
int canvasHeight = 0;

bool NewItemWindowActive = false;
bool EditItemWindowActive = false;

char hexCode[120] = "0x0";

Gui *NewGUI() {
    Gui *ui = (Gui *)malloc(sizeof(Gui));
    ui->conf = (UiConfig *)malloc(sizeof(UiConfig));

    ui->conf->gridBtnSize = 50;
    ui->conf->enableGrid = true;
    ui->conf->gridThickness = 2;
    ui->conf->toolbarHeight = 50;
    ui->conf->statusbarHeight = 30;
    ui->conf->itemListWidth = 200;
    ui->conf->gridSize = (Vector2){.x = 8, .y = 8};

    flipset_init(ui->flipped);

    ui->winWidth = 800;
    ui->winHeight = 640;
    ui->title = TextFormat("BauriFontMaker");
    ui->itemListAnchor = (Vector2){.x = 0, .y = 0};
    ui->itemListActive = 0;
    ui->itemListIndex = 0;
    ui->items = NewFontItemList();
    ui->currentItem = NULL;

    ui->items = NewFontItemList();
    AddToFontItemList(ui->items, NewFontItem("0x00000"));
    ui->currentItem = ui->items->items[0];

    return ui;
}

void FreeGui(Gui *ui) {
    FreeFontItemList(ui->items);
    flipset_clear(ui->flipped);
    free(ui->conf);
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
    SetWindowMinSize(ui->winWidth, ui->winHeight);
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

void Layout(Gui *ui) {
    GuiPanel((Rectangle){0, 0, ui->winWidth, ui->winHeight}, NULL);
    ToolBarLayout(ui);
    canvasPanel(ui);
    ItemSelectorLayout(ui);
    StatusBarLayout(ui);

    if (EditItemWindowActive) {
        EditItemWindow(ui);
    }

    if (NewItemWindowActive) {

        NewItemWindow(ui);
    }
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

void canvasLayout(Gui *ui, Rectangle panel) {

    int gridW = (int)ui->conf->gridSize.x * ui->conf->gridBtnSize +
                ((int)ui->conf->gridSize.x + 1) * ui->conf->gridThickness;
    int gridH = (int)ui->conf->gridSize.y * ui->conf->gridBtnSize +
                ((int)ui->conf->gridSize.y + 1) * ui->conf->gridThickness;
    int panelCenterX = panel.x + panel.width * 0.5f;
    int panelCenterY = panel.y + panel.height * 0.5f;

    int gridX = panelCenterX - gridW * 0.5f;
    int gridY = panelCenterY - gridH * 0.5f;

    if (ui->conf->enableGrid) {
        DrawRectangleRec((Rectangle){gridX, gridY, gridW, gridH}, ColorBlack);
    }

    int btnIndex = 0;
    for (int row = 0; row < (int)ui->conf->gridSize.y; row++) {
        for (int col = 0; col < (int)ui->conf->gridSize.x; col++) {
            int btnX =
                (gridX + ui->conf->gridThickness) +
                (col * (ui->conf->gridBtnSize + ui->conf->gridThickness));
            int btnY =
                (gridY + ui->conf->gridThickness) +
                (row * (ui->conf->gridBtnSize + ui->conf->gridThickness));

            Rectangle btnRect = {btnX, btnY, ui->conf->gridBtnSize,
                                 ui->conf->gridBtnSize};

            if (isCanvasBtnClicked(btnRect)) {
                printf("[%d,%d]\n", col, row);
                FontItemFlipBit(ui->currentItem, col, row);
                LogBinaryFontItem(ui->currentItem);
                LogFontItem(ui->currentItem);
            }

            if (isHoverBtn(btnRect)) {
                gridPosX = col;
                gridPosY = row;
            }
            Color clr = ColorWhite;

            if (FontItemGetFlip(ui->currentItem, col, row)) {
                clr = ColorBlack;
            }
            DrawRectangleRec(btnRect, clr);
            btnIndex++;
        }
    }
}

void canvasPanel(Gui *ui) {

    int xpos = ui->itemListAnchor.x + ITEM_PANEL_MARGIN +
               ui->conf->itemListWidth + ITEM_PANEL_MARGIN +
               CANVAS_PANEL_MARGIN;

    Rectangle canvasPanelRect =
        (Rectangle){xpos, ui->conf->toolbarHeight + ITEM_PANEL_MARGIN,
                    ui->winWidth - xpos - (ITEM_PANEL_MARGIN),
                    ui->winHeight - ui->conf->toolbarHeight -
                        ui->conf->statusbarHeight - ITEM_PANEL_MARGIN * 2

        };

    canvasLayout(ui, canvasPanelRect);

    GuiGroupBox(canvasPanelRect, "~canvas");
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

    bool editClicked = GuiButton(
        (Rectangle){
            x + widthForNewBtn + ITEM_CTRL_BTN_PADDING,
            y,
            maxWidth - widthForNewBtn - ITEM_CTRL_BTN_PADDING,
            NEWITEM_BTN_HEIGHT,
        },
        GuiIconText(ICON_PENCIL_BIG, NULL));

    if (editClicked) {
        EditItemWindowActive = true;
    }
}
int listItemFocus = 0;

void ItemSelectorLayout(Gui *ui) {
    Rectangle ItemPanelRect = (Rectangle){
        ui->itemListAnchor.x + ITEM_PANEL_MARGIN,
        ui->itemListAnchor.y + ui->conf->toolbarHeight + ITEM_PANEL_MARGIN,
        ui->conf->itemListWidth,
        ui->winHeight - ui->conf->statusbarHeight - ui->conf->toolbarHeight -
            (ITEM_PANEL_MARGIN * 2)};
    GuiGroupBox((ItemPanelRect), "~items");
    ItemCtrlBtns(ui, ItemPanelRect.x + ITEM_PANEL_PADDING,
                 ItemPanelRect.y + ITEM_PANEL_PADDING,
                 ui->conf->itemListWidth - ITEM_PANEL_PADDING * 2);

    float btnsY = ItemPanelRect.y + ITEM_PANEL_PADDING * 2 + NEWITEM_BTN_HEIGHT;

    GuiListViewEx(
        (Rectangle){
            ItemPanelRect.x + ITEM_PANEL_PADDING,
            btnsY,
            ui->conf->itemListWidth - (ITEM_PANEL_PADDING * 2),
            ItemPanelRect.height - NEWITEM_BTN_HEIGHT - ITEM_PANEL_PADDING * 3,
        },
        ui->items->names, ui->items->len, &ui->itemListIndex,
        &ui->itemListActive, &listItemFocus);

    // printf("selected item -> %d\n", ui->itemListActive);
    ui->currentItem = ui->items->items[ui->itemListActive];
}

void ToolBarLayout(Gui *ui) {
    GuiPanel(
        (Rectangle){
            0,
            0,
            ui->winWidth,
            ui->conf->toolbarHeight,
        },
        NULL);

    int btnX = TOOL_MARGIN;
    int btnY = TOOL_MARGIN;

    int btnW = 80;
    int btnH = ui->conf->toolbarHeight - (TOOL_MARGIN * 2);

    GuiButton(
        (Rectangle){
            btnX,
            btnY,
            btnW,
            btnH,
        },
        GuiIconText(ICON_FILE_NEW, "~new"));

    btnX += btnW + TOOL_MARGIN * 2;

    GuiButton(
        (Rectangle){
            btnX,
            btnY,
            btnW,
            btnH,
        },
        GuiIconText(ICON_FILE_OPEN, "~open"));

    btnX += btnW + TOOL_MARGIN * 2;

    GuiButton(
        (Rectangle){
            btnX,
            btnY,
            btnW,
            btnH,
        },
        GuiIconText(ICON_FILE_SAVE_CLASSIC, "~save"));

    btnX += btnW + TOOL_MARGIN * 2;

    DrawLine(btnX, 0, btnX, ui->conf->toolbarHeight, GRAY);
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

    DrawLine(btnX, 0, btnX, ui->conf->toolbarHeight, GRAY);
}

void StatusBarLayout(Gui *ui) {
    GuiStatusBar(
        (Rectangle){
            0,
            ui->winHeight - ui->conf->statusbarHeight,
            ui->winWidth,
            ui->conf->statusbarHeight,
        },
        TextFormat("[%d, %d] | ", gridPosX, gridPosY));
}

#define NEWWIN_BG (Color){0, 0, 0, 200}

void FillScrenForPopup(Gui *ui) {
    DrawRectangleRec(
        (Rectangle){
            0,
            0,
            ui->winWidth,
            ui->winHeight,
        },
        NEWWIN_BG);
}

void NewItemWindow(Gui *ui) {
    FillScrenForPopup(ui);
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

    int val = GuiTextBox(
        (Rectangle){winPos.x + NW_MARGIN + 200, winPos.y + NW_MARGIN, 200, 32},
        hexCode, 120, true);

    bool createClicked = GuiButton(
        (Rectangle){winPos.x + NW_MARGIN, winPos.y + NW_MARGIN + 100, 80, 32},
        "Create");

    bool cancelClicked =
        GuiButton((Rectangle){winPos.x + NW_MARGIN + 80 + NW_MARGIN,
                              winPos.y + NW_MARGIN + 100, 80, 32},
                  "Cancel");

    if (createClicked) {
        printf("Create New Item -> %s | val -> %d\n", hexCode, val);
        AddToFontItemList(ui->items, NewFontItem(hexCode));
        memcpy(hexCode, "0x0", 3);
        NewItemWindowActive = false;
    }
    if (cancelClicked) {

        memcpy(hexCode, "0x0", 3);
        NewItemWindowActive = false;
    }
}

void EditItemWindow(Gui *ui) {
    FillScrenForPopup(ui);
    Vector2 winPos = (Vector2){
        (ui->winWidth - NW_WIDTH) * 0.5f,
        (ui->winHeight - NW_HEIGHT) * 0.5f,
    };

    EditItemWindowActive = !GuiWindowBox(
        (Rectangle){
            winPos.x,
            winPos.y,
            NW_WIDTH,
            NW_HEIGHT,

        },
        TextFormat("Edit Font Item : %s", ui->currentItem->name));
}
