#include "include/gui.h"
#include "include/colors.h"
#include "include/ext/raylib.h"
#include "include/filedialog.h"
#include "include/fontitem.h"
#include "include/fontitemlist.h"
#include "include/itemselector.h"
#include "include/toolbar.h"
#include "include/utils.h"
#include "include/windows/edititem.h"
#include "include/windows/newitem.h"
#include <string.h>
#define RAYGUI_IMPLEMENTATION
#include "include/ext/raygui.h"

#include "include/ext/lightbfm.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MARGIN              20 // Canvas Margin

#define CANVAS_PANEL_MARGIN 10

#define NEWWIN_BG           (Color){0, 0, 0, 200}

void Layout(Gui *ui);
void StatusBarLayout(Gui *ui);
void ToolBarLayout(Gui *ui);

void ItemSelectorLayout(Gui *ui);

void canvasPanel(Gui *ui);
void canvasLayout(Gui *ui, Rectangle panel);

int gridPosX = 0;
int gridPosY = 0;

int canvasWidth = 0;
int canvasHeight = 0;

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
    ui->conf->isPopupActive = false;

    ui->winWidth = 800;
    ui->winHeight = 640;
    ui->title = TextFormat("BauriFontMaker");
    ui->itemListAnchor = (Vector2){.x = 0, .y = 0};
    ui->items = NewFontItemList();
    ui->currentItem = NULL;

    strcpy(ui->openFilename, "");
    strcpy(ui->saveFilename, "");

    ui->items = NewFontItemList();
    FontItem *tempItem = NewFontItem("0x00000");
    SetNameValue(tempItem, 0);
    AddToFontItemList(ui->items, tempItem);
    ui->currentItem = ui->items->items[0];

    return ui;
}

void FreeGui(Gui *ui) {
    FreeFontItemList(ui->items);
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

    ui->conf->newItemWindowState = CreateNewItemWindow();
    ui->conf->editItemWindowState = CreateEditItemWindow();
    ui->conf->toolbarState = CreateToolbar(ui->conf->toolbarHeight);
    ui->conf->itemSelectorState = CreateItemSelector(
        ui->conf->itemListWidth, ui->conf->toolbarHeight,
        ui->conf->statusbarHeight
    );

    TraceLog(
        LOG_WARNING, "OS CHECK -> WIN[%d] | LINUX[%d] | MAC[%d] | WEB[%d]",
        IsWin(), IsLinux(), IsMac(), IsWeb()
    );
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

void FillScrenForPopup(Gui *ui) {
    DrawRectangleRec(
        (Rectangle){
            0,
            0,
            ui->winWidth,
            ui->winHeight,
        },
        NEWWIN_BG
    );
}

void handleNewItemWindow(Gui *ui) {
    if (ui->conf->itemSelectorState.newBtnClicked) {
        ui->conf->newItemWindowState.windowActive = true;
    }
    if (NewItemWindow(&ui->conf->newItemWindowState)) {

        FontItem *tempItem = NewFontItem(ui->conf->newItemWindowState.nameStr);
        SetNameValue(tempItem, ui->conf->newItemWindowState.hexValue);
        AddToFontItemList(ui->items, tempItem);
    }
}

void handleEditItemWindow(Gui *ui) {
    if (ui->conf->itemSelectorState.editBtnClicked) {
        ui->conf->editItemWindowState.windowActive = true;
    }

    if (EditItemWindow(&ui->conf->editItemWindowState, ui->currentItem)) {
        printf("Shoud Create Edit Window\n");
    }
}

void handleItemSelector(Gui *ui) {

    ui->currentItem = ui->items->items[ui->conf->itemSelectorState.active];
}

void newFileDialog(Gui *ui) {}

void Layout(Gui *ui) {
    GuiEnableTooltip();

    ui->conf->isPopupActive =
        (ui->conf->newItemWindowState.windowActive ||
         ui->conf->editItemWindowState.windowActive);

    if (ui->conf->isPopupActive) {

        GuiLock();
        FillScrenForPopup(ui);
        // TODO: Do something about ignoring inputs
    }

    GuiPanel((Rectangle){0, 0, ui->winWidth, ui->winHeight}, NULL);
    Toolbar(&ui->conf->toolbarState);
    canvasPanel(ui);
    ItemSelector(
        &ui->conf->itemSelectorState, ui->items->names, ui->items->len
    );
    handleItemSelector(ui);
    StatusBarLayout(ui);

    GuiUnlock();

    handleNewItemWindow(ui);
    handleEditItemWindow(ui);

    if (ui->conf->toolbarState.openBtnClicked) {
        bool ok = OpenFileDialog(
            "Open Font File", ui->openFilename, "*.bfont;*.baufnt;*.txt",
            "BauriFontMaker Font Files (*.bfont)"
        );

        if (ok) {
            printf("Open File -> %s\n", ui->openFilename);
        }
    }
}

bool isCanvasBtnClicked(Rectangle rect) {
    if (GuiIsLocked()) {
        return false;
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
        CheckCollisionPointRec(GetMousePosition(), rect)) {
        return true;
    }

    return false;
}

bool isHoverBtn(Rectangle rect) {
    if (GuiIsLocked()) {
        return false;
    }
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

            Rectangle btnRect = {
                btnX, btnY, ui->conf->gridBtnSize, ui->conf->gridBtnSize
            };

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

char cc[128] = {0};

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

void StatusBarLayout(Gui *ui) {
    int hoverIndex = ui->conf->itemSelectorState.focus;
    FontItem *hoveredFontItem = ui->items->items[hoverIndex];
    GuiStatusBar(
        (Rectangle){
            0,
            ui->winHeight - ui->conf->statusbarHeight,
            ui->winWidth,
            ui->conf->statusbarHeight,
        },
        TextFormat(
            "[%d, %d] | %s (%d)", gridPosX, gridPosY, hoveredFontItem->name,
            hoveredFontItem->nameValue
        )
    );
}
