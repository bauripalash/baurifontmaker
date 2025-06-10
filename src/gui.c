#include "include/gui.h"
#include "include/colors.h"
#include "include/defaults.h"
#include "include/ext/raylib.h"
#include "include/filedialog.h"
#include "include/fontitem.h"
#include "include/fontitemlist.h"
#include "include/itemselector.h"
#include "include/toolbar.h"
#include "include/uiopts.h"
#include "include/utils.h"
#include "include/windows/edititem.h"
#include "include/windows/newitem.h"
#define RAYGUI_IMPLEMENTATION
#include "include/ext/raygui.h"

#include "include/ext/lightbfm.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MARGIN              20 // Canvas Margin

#define CANVAS_PANEL_MARGIN 10

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

bool hasError = false;

Gui *NewGUI() {
    Gui *ui = (Gui *)malloc(sizeof(Gui));
    ui->conf = NewUiConfig();

    ui->winWidth = 800;
    ui->winHeight = 640;
    ui->title = TextFormat("BauriFontMaker");
    ui->itemListAnchor = (Vector2){.x = 0, .y = 0};
    ui->currentItem = NULL;

    strcpy(ui->openFilename, "");
    strcpy(ui->saveFilename, "");

    FontItemList *flist = NewFontItemList();

    if (flist == NULL) {
        hasError = true;
        ui->items = NULL;
    } else {
        ui->items = flist;
    }

    // FontItem *tempItem = NewFontItem("0x00000");
    // SetNameValue(tempItem, 0);
    // AddToFontItemList(ui->items, tempItem);
    // ui->currentItem = ui->items->items[0];

    return ui;
}

void FreeGui(Gui *ui) {
    if (ui->items != NULL) {
        FreeFontItemList(ui->items);
    }
    FreeUiConfig(ui->conf);
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

    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(DEF_WIN_WIDTH, DEF_WIN_HEIGHT, DEF_WIN_TITLE);
    SetWindowMinSize(DEF_WIN_WIDTH, DEF_WIN_HEIGHT);
    GuiLoadStyleLightBFM();

    Gui *ui = NewGUI();

    SetTargetFPS(60);

    ui->states = NewUiStates();

    TraceLog(
        LOG_WARNING, "OS CHECK -> WIN[%d] | LINUX[%d] | MAC[%d] | WEB[%d]",
        IsWin(), IsLinux(), IsMac(), IsWeb()
    );
    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            if (hasError) {
                bool clicked =
                    GuiMessageBox(
                        GetCenteredRect(DEF_ERR_WIN_WIDTH, DEF_ERR_WIN_HEIGHT),
                        "Fatal Memory Error",
                        "Failed to allocate memory for font item list!", "Close"
                    ) != -1;

                EndDrawing();
                if (clicked) {
                    break;
                }
                continue;
            }
            UpdateData(ui);
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
        ColorTrBlack
    );
}

void handleNewItemWindow(Gui *ui) {
    if (ui->states->itemSelector.newBtnClicked) {
        ui->states->newItem.windowActive = true;
    }
    if (NewItemWindow(&ui->states->newItem)) {

        FontItem *tempItem = NewFontItem(ui->states->newItem.nameStr);
        SetNameValue(tempItem, ui->states->newItem.hexValue);
        AddToFontItemList(ui->items, tempItem);
    }
}

void handleEditItemWindow(Gui *ui) {
    if (ui->states->itemSelector.editBtnClicked) {
        ui->states->editItem.windowActive = true;
        // SetStateEditItemWindow(ui., FontItem *target, int itemLen);
    }

    if (EditItemWindow(
            &ui->states->editItem, ui->currentItem, ui->items->len
        )) {
        printf("Shoud Create Edit Window [%zu]\n", ui->items->len);
    }
}

void handleItemSelector(Gui *ui) {

    ui->currentItem = ui->items->items[ui->states->itemSelector.active];
}

void newFileDialog(Gui *ui) {}

void Layout(Gui *ui) {
    GuiEnableTooltip();

    ui->conf->isPopupActive =
        (ui->states->newItem.windowActive || ui->states->editItem.windowActive);

    if (ui->conf->isPopupActive) {

        GuiLock();
        FillScrenForPopup(ui);
        // TODO: Do something about ignoring inputs
    }

    GuiPanel((Rectangle){0, 0, ui->winWidth, ui->winHeight}, NULL);
    Toolbar(&ui->states->toolbar);
    canvasPanel(ui);
    ItemSelector(&ui->states->itemSelector, ui->items->names, ui->items->len);
    handleItemSelector(ui);
    StatusBarLayout(ui);

    GuiUnlock();

    handleNewItemWindow(ui);
    handleEditItemWindow(ui);

    if (ui->states->toolbar.openBtnClicked) {
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
                // printf("[%d,%d]\n", col, row);
                FontItemFlipBit(ui->currentItem, col, row);
                // LogBinaryFontItem(ui->currentItem);
                // LogFontItem(ui->currentItem);
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
    int hoverIndex = ui->states->itemSelector.focus;
    FontItem *hoveredFontItem = ui->items->items[hoverIndex];
    GuiStatusBar(
        (Rectangle){
            0,
            ui->winHeight - ui->conf->statusbarHeight,
            ui->winWidth,
            ui->conf->statusbarHeight,
        },
        TextFormat(
            "[%d, %d] | %s (0x%x)", gridPosX, gridPosY, hoveredFontItem->name,
            hoveredFontItem->nameValue
        )
    );
}
